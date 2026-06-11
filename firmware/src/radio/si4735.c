/**
 * @file si4735.c
 * @brief SI4735 / SI473X / SI474X C port ? implementation
 *
 * Ported from the Arduino C++ library by PU2CLR (Ricardo Lima Caratti).
 * Original: https://github.com/pu2clr/SI4735
 *
 * All hardware access is through si4735_hal.h ? no Arduino/Wire code.
 */

#include "si4735.h"
#include "si4735_hal.h"

/* =========================================================================
 * Module-level state  (replaces C++ class member variables)
 * ========================================================================= */

/* RDS text buffers */
static char rds_buffer2A[65]; /* Program Information (Radio Text) */
static char rds_buffer2B[33]; /* Station Information */
static char rds_buffer0A[9];  /* Station Name */
static char rds_time[25];     /* Date / time string */

static int rdsTextAdress2A = 0;
static int rdsTextAdress2B = 0;
static int rdsTextAdress0A = 0;

static bool rdsEndGroupA = false;
static bool rdsEndGroupB = false;

static int16_t deviceAddress = SI473X_ADDR_SEN_LOW;

/* Timing */
static uint16_t  maxDelaySetFrequency = MAX_DELAY_AFTER_SET_FREQUENCY;
static uint16_t  maxDelayAfterPowerUp = MAX_DELAY_AFTER_POWERUP;
static uint32_t  maxSeekTime          = MAX_SEEK_TIME;

static uint8_t lastTextFlagAB = 0;
static uint8_t resetPinId     = 0; /* logical pin ID passed to HAL */

static uint8_t currentTune = FM_TUNE_FREQ;

static uint16_t currentMinimumFrequency = 0;
static uint16_t currentMaximumFrequency = 0;
static uint16_t currentWorkFrequency    = 0;
static uint16_t currentStep             = 0;

static uint8_t lastMode = LAST_MODE_NONE;

static uint8_t currentAvcAmMaxGain = DEFAULT_CURRENT_AVC_AM_MAX_GAIN;
static uint8_t currentClockType    = XOSCEN_CRYSTAL;
static uint8_t ctsIntEnable        = 0;
static uint8_t gpo2Enable          = 0;

static uint16_t refClock          = 32768;
static uint16_t refClockPrescale  = 1;
static uint8_t  refClockSourcePin = 0;

static si47x_frequency        currentFrequency;
static si47x_set_frequency    currentFrequencyParams;
static si47x_rqs_status       currentRqsStatus;
static si47x_response_status  currentStatus;
static si47x_firmware_information firmwareInfo;
static si47x_rds_status       currentRdsStatus;
static si47x_agc_status       currentAgcStatus;
static si47x_ssb_mode         currentSSBMode;

static si473x_powerup powerUp;

static uint8_t volume         = 32;
static uint8_t currentAudioMode = SI473X_ANALOG_AUDIO;
static uint8_t currentSsbStatus = 0;
static int8_t  audioMuteMcuPin  = -1;

/* =========================================================================
 * Private helpers
 * ========================================================================= */

static void _sendSSBModeProperty(void);
static void _disableFmDebug(void);

/* =========================================================================
 * Initialisation
 * ========================================================================= */

void si4735_init(void)
{
    currentSsbStatus = 0;
}

/* =========================================================================
 * Hardware reset
 * ========================================================================= */

void si4735_reset(void)
{
    hal_gpio_reset_init();
    hal_delay_ms(10);
    hal_gpio_reset_set(false);  /* LOW ? device in reset */
    hal_delay_ms(10);
    hal_gpio_reset_set(true);   /* HIGH ? device released */
    hal_delay_ms(10);
}

/* =========================================================================
 * Wait until the device signals CTS (Clear To Send)
 * ========================================================================= */

void si4735_waitToSend(void)
{
    uint8_t status;
    do {
        hal_delay_us(MIN_DELAY_WAIT_SEND_LOOP);
        hal_i2c_request_from((uint8_t)deviceAddress, 1);
        status = hal_i2c_read();
    } while (!(status & 0x80));
}

/* =========================================================================
 * GPIO
 * ========================================================================= */

void si4735_setGpioCtl(uint8_t GPO1OEN, uint8_t GPO2OEN, uint8_t GPO3OEN)
{
    si473x_gpio gpio;
    gpio.arg.GPO1OEN = GPO1OEN;
    gpio.arg.GPO2OEN = GPO2OEN;
    gpio.arg.GPO3OEN = GPO3OEN;
    gpio.arg.DUMMY1  = 0;
    gpio.arg.DUMMY2  = 0;

    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(GPIO_CTL);
    hal_i2c_write(gpio.raw);
    hal_i2c_end_transmission();
}

void si4735_setGpio(uint8_t GPO1LEVEL, uint8_t GPO2LEVEL, uint8_t GPO3LEVEL)
{
    si473x_gpio gpio;
    gpio.arg.GPO1OEN = GPO1LEVEL;
    gpio.arg.GPO2OEN = GPO2LEVEL;
    gpio.arg.GPO3OEN = GPO3LEVEL;
    gpio.arg.DUMMY1  = 0;
    gpio.arg.DUMMY2  = 0;

    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(GPIO_SET);
    hal_i2c_write(gpio.raw);
    hal_i2c_end_transmission();
}

void si4735_setGpioIen(uint8_t STCIEN, uint8_t RSQIEN, uint8_t ERRIEN,
                        uint8_t CTSIEN, uint8_t STCREP, uint8_t RSQREP)
{
    si473x_gpio_ien gpio;
    gpio.arg.DUMMY1 = gpio.arg.DUMMY2 = gpio.arg.DUMMY3 = gpio.arg.DUMMY4 = 0;
    gpio.arg.STCIEN = STCIEN;
    gpio.arg.RSQIEN = RSQIEN;
    gpio.arg.ERRIEN = ERRIEN;
    gpio.arg.CTSIEN = CTSIEN;
    gpio.arg.STCREP = STCREP;
    gpio.arg.RSQREP = RSQREP;
    si4735_sendProperty(GPO_IEN, gpio.raw);
}

/* =========================================================================
 * I2C address management
 * ========================================================================= */

void si4735_setDeviceI2CAddress(uint8_t senPin)
{
    deviceAddress = senPin ? SI473X_ADDR_SEN_HIGH : SI473X_ADDR_SEN_LOW;
}

int16_t si4735_getDeviceI2CAddress(uint8_t resetPin)
{
    (void)resetPin;
    si4735_reset();

    /* Try low address */
    hal_i2c_begin_transmission(SI473X_ADDR_SEN_LOW);
    if (hal_i2c_end_transmission() == 0) {
        si4735_setDeviceI2CAddress(0);
        return SI473X_ADDR_SEN_LOW;
    }

    /* Try high address */
    hal_i2c_begin_transmission(SI473X_ADDR_SEN_HIGH);
    if (hal_i2c_end_transmission() == 0) {
        si4735_setDeviceI2CAddress(1);
        return SI473X_ADDR_SEN_HIGH;
    }

    return 0; /* not found */
}

void si4735_setDeviceOtherI2CAddress(uint8_t i2cAddr)
{
    deviceAddress = i2cAddr;
}

/* =========================================================================
 * Reference clock
 * ========================================================================= */

void si4735_setRefClock(uint16_t refclk)
{
    si4735_sendProperty(REFCLK_FREQ, refclk);
    refClock = refclk;
}

void si4735_setRefClockPrescaler(uint16_t prescale, uint8_t rclk_sel)
{
    si4735_sendProperty(REFCLK_PRESCALE, prescale);
    refClockPrescale  = prescale;
    refClockSourcePin = rclk_sel;
}

/* =========================================================================
 * Power Up / Down
 * ========================================================================= */

void si4735_setPowerUp(uint8_t CTSIEN, uint8_t GPO2OEN, uint8_t PATCH,
                        uint8_t XOSCEN, uint8_t FUNC, uint8_t OPMODE)
{
    powerUp.arg.CTSIEN  = CTSIEN;
    powerUp.arg.GPO2OEN = GPO2OEN;
    powerUp.arg.PATCH   = PATCH;
    powerUp.arg.XOSCEN  = XOSCEN;
    powerUp.arg.FUNC    = FUNC;
    powerUp.arg.OPMODE  = OPMODE;

    currentClockType = XOSCEN;

    if (FUNC == 0) {
        currentTune = FM_TUNE_FREQ;
        currentFrequencyParams.arg.FREEZE = 1;
    } else {
        currentTune = AM_TUNE_FREQ;
        currentFrequencyParams.arg.FREEZE = 0;
    }
    currentFrequencyParams.arg.FAST    = 1;
    currentFrequencyParams.arg.DUMMY1  = 0;
    currentFrequencyParams.arg.ANTCAPH = 0;
    currentFrequencyParams.arg.ANTCAPL = 1;
}

void si4735_radioPowerUp(void)
{
    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(POWER_UP);
    hal_i2c_write(powerUp.raw[0]);
    hal_i2c_write(powerUp.raw[1]);
    hal_i2c_end_transmission();

    si4735_waitToSend();
    hal_delay_ms(maxDelayAfterPowerUp);

    if (audioMuteMcuPin >= 0)
        si4735_setHardwareAudioMute(false);

    if (currentClockType == XOSCEN_RCLK) {
        si4735_setRefClock(refClock);
        si4735_setRefClockPrescaler(refClockPrescale, refClockSourcePin);
    }
}

void si4735_analogPowerUp(void)
{
    si4735_radioPowerUp();
}

void si4735_powerDown(void)
{
    if (audioMuteMcuPin >= 0)
        si4735_setHardwareAudioMute(true);

    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(POWER_DOWN);
    hal_i2c_end_transmission();
    hal_delay_us(2500);
}

/* =========================================================================
 * Setup entry points
 * ========================================================================= */

void si4735_detailed_setup(uint8_t resetPin, uint8_t _ctsIntEnable,
                            uint8_t defaultFunction, uint8_t audioMode,
                            uint8_t clockType, uint8_t _gpo2Enable)
{
    resetPinId       = resetPin;
    ctsIntEnable     = (_ctsIntEnable != 0) ? 1 : 0;
    gpo2Enable       = _gpo2Enable;
    currentAudioMode = audioMode;

    si4735_setPowerUp(ctsIntEnable, gpo2Enable, 0, clockType,
                      defaultFunction, audioMode);

    if (audioMuteMcuPin >= 0)
        si4735_setHardwareAudioMute(true);

    si4735_reset();
    si4735_radioPowerUp();
    si4735_setVolume(30);
    si4735_getFirmware();
}

void si4735_setup(uint8_t resetPin, uint8_t defaultFunction)
{
    si4735_detailed_setup(resetPin, 0, defaultFunction,
                          SI473X_ANALOG_AUDIO, XOSCEN_CRYSTAL, 0);
    hal_delay_ms(250);
}

/* =========================================================================
 * Firmware query
 * ========================================================================= */

void si4735_getFirmware(void)
{
    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(GET_REV);
    hal_i2c_end_transmission();

    do {
        si4735_waitToSend();
        hal_i2c_request_from((uint8_t)deviceAddress, 9);
        for (int i = 0; i < 9; i++)
            firmwareInfo.raw[i] = hal_i2c_read();
    } while (firmwareInfo.resp.ERR);
}

uint8_t si4735_getFirmwarePN(void)      { return firmwareInfo.resp.PN;       }
uint8_t si4735_getFirmwareFWMAJOR(void) { return firmwareInfo.resp.FWMAJOR;  }
uint8_t si4735_getFirmwareFWMINOR(void) { return firmwareInfo.resp.FWMINOR;  }
uint8_t si4735_getFirmwarePATCHH(void)  { return firmwareInfo.resp.PATCHH;   }
uint8_t si4735_getFirmwarePATCHL(void)  { return firmwareInfo.resp.PATCHL;   }
uint8_t si4735_getFirmwareCMPMAJOR(void){ return firmwareInfo.resp.CMPMAJOR; }
uint8_t si4735_getFirmwareCMPMINOR(void){ return firmwareInfo.resp.CMPMINOR; }
uint8_t si4735_getFirmwareCHIPREV(void) { return firmwareInfo.resp.CHIPREV;  }

/* =========================================================================
 * Tune frequency
 * ========================================================================= */

void si4735_setTuneFrequencyAntennaCapacitor(uint16_t capacitor)
{
    si47x_antenna_capacitor cap;
    cap.value = capacitor;
    currentFrequencyParams.arg.DUMMY1 = 0;

    if (currentTune != AM_TUNE_FREQ) {
        currentFrequencyParams.arg.ANTCAPH = (capacitor <= 191) ? cap.raw.ANTCAPL : 0;
    } else {
        if (capacitor <= 6143) {
            currentFrequencyParams.arg.FREEZE  = 0;
            currentFrequencyParams.arg.ANTCAPH = cap.raw.ANTCAPH;
            currentFrequencyParams.arg.ANTCAPL = cap.raw.ANTCAPL;
        }
    }
    si4735_setFrequency(currentWorkFrequency);
}

void si4735_setFrequency(uint16_t freq)
{
    si4735_waitToSend();
    currentFrequency.value             = freq;
    currentFrequencyParams.arg.FREQH   = currentFrequency.raw.FREQH;
    currentFrequencyParams.arg.FREQL   = currentFrequency.raw.FREQL;

    if (currentSsbStatus != 0) {
        currentFrequencyParams.arg.DUMMY1  = 0;
        currentFrequencyParams.arg.USBLSB  = currentSsbStatus;
        currentFrequencyParams.arg.FAST    = 1;
        currentFrequencyParams.arg.FREEZE  = 0;
    }

    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(currentTune);
    hal_i2c_write(currentFrequencyParams.raw[0]);
    hal_i2c_write(currentFrequencyParams.arg.FREQH);
    hal_i2c_write(currentFrequencyParams.arg.FREQL);
    hal_i2c_write(currentFrequencyParams.arg.ANTCAPH);
    if (currentTune == AM_TUNE_FREQ)
        hal_i2c_write(currentFrequencyParams.arg.ANTCAPL);
    hal_i2c_end_transmission();

    si4735_waitToSend();
    currentWorkFrequency = freq;
    hal_delay_ms(maxDelaySetFrequency);
}

uint16_t si4735_getFrequency(void)
{
    si47x_frequency freq;
    si4735_getStatus(0, 1);
    freq.raw.FREQL       = currentStatus.resp.READFREQL;
    freq.raw.FREQH       = currentStatus.resp.READFREQH;
    currentWorkFrequency = freq.value;
    return freq.value;
}

uint16_t si4735_getCurrentFrequency(void) { return currentWorkFrequency; }

void si4735_frequencyUp(void)
{
    if (currentWorkFrequency >= currentMaximumFrequency)
        currentWorkFrequency = currentMinimumFrequency;
    else
        currentWorkFrequency += currentStep;
    si4735_setFrequency(currentWorkFrequency);
}

void si4735_frequencyDown(void)
{
    if (currentWorkFrequency <= currentMinimumFrequency)
        currentWorkFrequency = currentMaximumFrequency;
    else
        currentWorkFrequency -= currentStep;
    si4735_setFrequency(currentWorkFrequency);
}

void si4735_setFrequencyStep(uint16_t step) { currentStep = step; }

void si4735_setTuneFrequencyFast(uint8_t FAST)
{
    currentFrequencyParams.arg.FAST = FAST;
}
void si4735_setTuneFrequencyFreze(uint8_t FREEZE)
{
    currentFrequencyParams.arg.FREEZE = FREEZE;
}
uint8_t si4735_getTuneFrequecyFast(void)   { return currentFrequencyParams.arg.FAST;   }
uint8_t si4735_getTuneFrequecyFreeze(void) { return currentFrequencyParams.arg.FREEZE; }

/* =========================================================================
 * Status
 * ========================================================================= */

void si4735_getStatus(uint8_t INTACK, uint8_t CANCEL)
{
    si47x_tune_status status;
    uint8_t cmd      = FM_TUNE_STATUS;
    int limitResp    = 8;

    if      (currentTune == FM_TUNE_FREQ)   { cmd = FM_TUNE_STATUS;   }
    else if (currentTune == AM_TUNE_FREQ)   { cmd = AM_TUNE_STATUS;   }
    else if (currentTune == NBFM_TUNE_FREQ) { cmd = NBFM_TUNE_STATUS; limitResp = 6; }

    si4735_waitToSend();

    status.arg.INTACK    = INTACK;
    status.arg.CANCEL    = CANCEL;
    status.arg.RESERVED2 = 0;

    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(cmd);
    hal_i2c_write(status.raw);
    hal_i2c_end_transmission();

    do {
        si4735_waitToSend();
        hal_i2c_request_from((uint8_t)deviceAddress, (uint8_t)limitResp);
        for (int i = 0; i < limitResp; i++)
            currentStatus.raw[i] = hal_i2c_read();
    } while (currentStatus.resp.ERR);

    si4735_waitToSend();
}

void si4735_getStatusNoArgs(void) { si4735_getStatus(0, 1); }

bool    si4735_getSignalQualityInterrupt(void)      { return (bool)currentStatus.resp.RSQINT; }
bool    si4735_getRadioDataSystemInterrupt(void)    { return (bool)currentStatus.resp.RDSINT; }
bool    si4735_getTuneCompleteTriggered(void)       { return (bool)currentStatus.resp.STCINT; }
bool    si4735_getStatusError(void)                 { return (bool)currentStatus.resp.ERR;    }
bool    si4735_getStatusCTS(void)                   { return (bool)currentStatus.resp.CTS;    }
bool    si4735_getACFIndicator(void)                { return (bool)currentStatus.resp.AFCRL;  }
bool    si4735_getBandLimit(void)                   { return (bool)currentStatus.resp.BLTF;   }
bool    si4735_getStatusValid(void)                 { return (bool)currentStatus.resp.VALID;  }
uint8_t si4735_getReceivedSignalStrengthIndicator(void){ return currentStatus.resp.RSSI; }
uint8_t si4735_getStatusSNR(void)                   { return currentStatus.resp.SNR;  }
uint8_t si4735_getStatusMULT(void)                  { return currentStatus.resp.MULT; }

uint16_t si4735_getAntennaTuningCapacitor(void)
{
    si47x_antenna_capacitor cap;
    if (currentTune == FM_TUNE_FREQ)
        return currentStatus.resp.READANTCAP;
    cap.raw.ANTCAPL = currentStatus.resp.READANTCAP;
    cap.raw.ANTCAPH = currentStatus.resp.MULT;
    return cap.value;
}

/* =========================================================================
 * RSQ (Received Signal Quality)
 * ========================================================================= */

void si4735_getCurrentReceivedSignalQuality(uint8_t INTACK)
{
    uint8_t cmd;
    int sizeResponse;

    if      (currentTune == FM_TUNE_FREQ)   { cmd = FM_RSQ_STATUS;   sizeResponse = 8; }
    else if (currentTune == NBFM_TUNE_FREQ) { cmd = NBFM_RSQ_STATUS; sizeResponse = 8; }
    else                                    { cmd = AM_RSQ_STATUS;   sizeResponse = 6; }

    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(cmd);
    hal_i2c_write(INTACK);
    hal_i2c_end_transmission();

    si4735_waitToSend();
    hal_i2c_request_from((uint8_t)deviceAddress, (uint8_t)sizeResponse);
    for (int i = 0; i < sizeResponse; i++)
        currentRqsStatus.raw[i] = hal_i2c_read();
}

void si4735_getCurrentReceivedSignalQualityNoArgs(void)
{
    si4735_getCurrentReceivedSignalQuality(0);
}

uint8_t si4735_getCurrentRSSI(void)  { return currentRqsStatus.resp.RSSI;    }
uint8_t si4735_getCurrentSNR(void)   { return currentRqsStatus.resp.SNR;     }
bool si4735_getCurrentRssiDetectLow(void)  { return (bool)currentRqsStatus.resp.RSSIILINT; }
bool si4735_getCurrentRssiDetectHigh(void) { return (bool)currentRqsStatus.resp.RSSIHINT;  }
bool si4735_getCurrentSnrDetectLow(void)   { return (bool)currentRqsStatus.resp.SNRLINT;   }
bool si4735_getCurrentSnrDetectHigh(void)  { return (bool)currentRqsStatus.resp.SNRHINT;   }
bool si4735_getCurrentValidChannel(void)   { return (bool)currentRqsStatus.resp.VALID;     }
bool si4735_getCurrentAfcRailIndicator(void){ return (bool)currentRqsStatus.resp.AFCRL;   }
bool si4735_getCurrentSoftMuteIndicator(void){ return (bool)currentRqsStatus.resp.SMUTE;  }
uint8_t si4735_getCurrentStereoBlend(void) { return currentRqsStatus.resp.STBLEND; }
bool si4735_getCurrentPilot(void)          { return (bool)currentRqsStatus.resp.PILOT;    }
uint8_t si4735_getCurrentMultipath(void)   { return currentRqsStatus.resp.MULT;    }
uint8_t si4735_getCurrentSignedFrequencyOffset(void){ return currentRqsStatus.resp.FREQOFF; }
bool si4735_getCurrentMultipathDetectLow(void) { return (bool)currentRqsStatus.resp.MULTLINT; }
bool si4735_getCurrentMultipathDetectHigh(void){ return (bool)currentRqsStatus.resp.MULTHINT; }
bool si4735_getCurrentBlendDetectInterrupt(void){ return (bool)currentRqsStatus.resp.BLENDINT;}

/* =========================================================================
 * AGC
 * ========================================================================= */

void si4735_getAutomaticGainControl(void)
{
    uint8_t cmd;
    if      (currentTune == FM_TUNE_FREQ)   cmd = FM_AGC_STATUS;
    else if (currentTune == NBFM_TUNE_FREQ) cmd = NBFM_AGC_STATUS;
    else                                    cmd = AM_AGC_STATUS;

    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(cmd);
    hal_i2c_end_transmission();

    do {
        si4735_waitToSend();
        hal_i2c_request_from((uint8_t)deviceAddress, 3);
        currentAgcStatus.raw[0] = hal_i2c_read();
        currentAgcStatus.raw[1] = hal_i2c_read();
        currentAgcStatus.raw[2] = hal_i2c_read();
    } while (currentAgcStatus.refined.ERR);
}

void si4735_setAutomaticGainControl(uint8_t AGCDIS, uint8_t AGCIDX)
{
    si47x_agc_overrride agc;
    uint8_t cmd;

    if      (currentTune == FM_TUNE_FREQ)   cmd = FM_AGC_OVERRIDE;
    else if (currentTune == NBFM_TUNE_FREQ) cmd = NBFM_AGC_OVERRIDE;
    else                                    cmd = AM_AGC_OVERRIDE;

    agc.arg.DUMMY  = 0;
    agc.arg.AGCDIS = AGCDIS;
    agc.arg.AGCIDX = AGCIDX;

    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(cmd);
    hal_i2c_write(agc.raw[0]);
    hal_i2c_write(agc.raw[1]);
    hal_i2c_end_transmission();
    si4735_waitToSend();
}

bool    si4735_isAgcEnabled(void)  { return !currentAgcStatus.refined.AGCDIS; }
uint8_t si4735_getAgcGainIndex(void){ return currentAgcStatus.refined.AGCIDX; }

void si4735_setAvcAmMaxGain(uint8_t gain)
{
    if (gain < 12 || gain > 90) return;
    currentAvcAmMaxGain = gain;
    si4735_sendProperty(AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN, (uint16_t)(gain * 340));
}

uint8_t si4735_getCurrentAvcAmMaxGain(void) { return currentAvcAmMaxGain; }

void si4735_setAvcAmMaxGainMax(void)
{
    si4735_sendProperty(AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN, 0x7800);
    currentAvcAmMaxGain = 90;
}
void si4735_setAvcAmMaxGainMin(void)
{
    si4735_sendProperty(AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN, 0x1000);
    currentAvcAmMaxGain = 12;
}
void si4735_setAvcAmMaxGainDefault(void)
{
    si4735_sendProperty(AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN, 0x1543);
    currentAvcAmMaxGain = DEFAULT_CURRENT_AVC_AM_MAX_GAIN;
}

void si4735_setSsbAgcOverrite(uint8_t SSBAGCDIS, uint8_t SSBAGCNDX,
                               uint8_t reserved)
{
    si47x_agc_overrride agc;
    agc.arg.DUMMY  = reserved;
    agc.arg.AGCDIS = SSBAGCDIS;
    agc.arg.AGCIDX = SSBAGCNDX;

    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(SSB_AGC_OVERRIDE);
    hal_i2c_write(agc.raw[0]);
    hal_i2c_write(agc.raw[1]);
    hal_i2c_end_transmission();
    si4735_waitToSend();
}

/* =========================================================================
 * Mode selection ? AM / FM
 * ========================================================================= */

bool si4735_isCurrentTuneFM(void)  { return currentTune == FM_TUNE_FREQ;  }
bool si4735_isCurrentTuneAM(void)  { return currentTune == AM_TUNE_FREQ;  }
bool si4735_isCurrentTuneSSB(void) { return currentTune == SSB_TUNE_FREQ && currentSsbStatus != 0; }

void si4735_setAMNoArgs(void)
{
    if (lastMode != AM_CURRENT_MODE) {
        si4735_powerDown();
        si4735_setPowerUp(ctsIntEnable, 0, 0, currentClockType,
                          AM_CURRENT_MODE, currentAudioMode);
        si4735_radioPowerUp();
        si4735_setAvcAmMaxGain(currentAvcAmMaxGain);
        si4735_setVolume(volume);
    }
    currentSsbStatus = 0;
    lastMode         = AM_CURRENT_MODE;
}

void si4735_setFMNoArgs(void)
{
    si4735_powerDown();
    si4735_setPowerUp(ctsIntEnable, gpo2Enable, 0, currentClockType,
                      FM_CURRENT_MODE, currentAudioMode);
    si4735_radioPowerUp();
    si4735_setVolume(volume);
    currentSsbStatus = 0;
    _disableFmDebug();
    lastMode = FM_CURRENT_MODE;
}

void si4735_setAM(uint16_t fromFreq, uint16_t toFreq,
                  uint16_t initialFreq, uint16_t step)
{
    currentMinimumFrequency = fromFreq;
    currentMaximumFrequency = toFreq;
    currentStep             = step;
    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;
    si4735_setAMNoArgs();
    currentWorkFrequency = initialFreq;
    si4735_setFrequency(currentWorkFrequency);
}

void si4735_setFM(uint16_t fromFreq, uint16_t toFreq,
                  uint16_t initialFreq, uint16_t step)
{
    currentMinimumFrequency = fromFreq;
    currentMaximumFrequency = toFreq;
    currentStep             = step;
    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;
    si4735_setFMNoArgs();
    currentWorkFrequency = initialFreq;
    si4735_setFrequency(currentWorkFrequency);
}

/* =========================================================================
 * Bandwidth (AM / SSB only)
 * ========================================================================= */

void si4735_setBandwidth(uint8_t AMCHFLT, uint8_t AMPLFLT)
{
    si47x_bandwidth_config filter;
    si47x_property property;

    if (currentTune != AM_TUNE_FREQ) return;
    if (AMCHFLT > 6) return;

    filter.raw[0]       = 0;
    filter.raw[1]       = 0;
    property.value      = AM_CHANNEL_FILTER;
    filter.param.AMCHFLT = AMCHFLT;
    filter.param.AMPLFLT = AMPLFLT;

    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(SET_PROPERTY);
    hal_i2c_write(0x00);
    hal_i2c_write(property.raw.byteHigh);
    hal_i2c_write(property.raw.byteLow);
    hal_i2c_write(filter.raw[1]);
    hal_i2c_write(filter.raw[0]);
    hal_i2c_end_transmission();
    si4735_waitToSend();
}

/* =========================================================================
 * AM soft-mute / noise-blank / de-emphasis / AGC helpers
 * ========================================================================= */

void si4735_setAmSoftMuteMaxAttenuation(uint8_t smattn)
{
    si4735_sendProperty(AM_SOFT_MUTE_MAX_ATTENUATION, smattn);
}
void si4735_setAMSoftMuteSlop(uint8_t parameter)
{
    si4735_sendProperty(AM_SOFT_MUTE_SLOPE, parameter);
}
void si4735_setAMSoftMuteRate(uint8_t parameter)
{
    si4735_sendProperty(AM_SOFT_MUTE_RATE, parameter);
}
void si4735_setAMSoftMuteSnrThreshold(uint8_t parameter)
{
    si4735_sendProperty(AM_SOFT_MUTE_SNR_THRESHOLD, parameter);
}
void si4735_setAMSoftMuteReleaseRate(uint8_t parameter)
{
    si4735_sendProperty(AM_SOFT_MUTE_RELEASE_RATE, parameter);
}
void si4735_setAMSoftMuteAttackRate(uint16_t parameter)
{
    si4735_sendProperty(AM_SOFT_MUTE_ATTACK_RATE, parameter);
}
void si4735_setAMDeEmphasis(uint8_t parameter)
{
    si4735_sendProperty(AM_DEEMPHASIS, parameter);
}
void si4735_setAmAgcAttackRate(uint16_t parameter)
{
    si4735_sendProperty(AM_AGC_ATTACK_RATE, parameter);
}
void si4735_setAmAgcReleaseRate(uint16_t parameter)
{
    si4735_sendProperty(AM_AGC_RELEASE_RATE, parameter);
}
void si4735_setAMFrontEndAgcControl(uint8_t MIN_GAIN_INDEX, uint8_t ATTN_BACKUP)
{
    si47x_frontend_agc_control param;
    param.field.MIN_GAIN_INDEX = MIN_GAIN_INDEX;
    param.field.ATTN_BACKUP    = ATTN_BACKUP;
    si4735_sendProperty(AM_FRONTEND_AGC_CONTROL, param.word);
}
void si4735_setAmNoiseBlank(uint16_t nb_rate, uint16_t nb_interval,
                             uint16_t nb_irr_filter)
{
    si4735_sendProperty(AM_NB_RATE,       nb_rate);
    si4735_sendProperty(AM_NB_INTERVAL,   nb_interval);
    si4735_sendProperty(AM_NB_IIR_FILTER, nb_irr_filter);
}
void si4735_setAmDelayNB(uint16_t value)
{
    si4735_sendProperty(AM_NB_DELAY, value);
}

/* =========================================================================
 * FM soft-mute / noise-blank / de-emphasis / bandwidth helpers
 * ========================================================================= */

void si4735_setFMDeEmphasis(uint8_t parameter)
{
    si4735_sendProperty(FM_DEEMPHASIS, parameter);
}
void si4735_setFmSoftMuteMaxAttenuation(uint8_t smattn)
{
    si4735_sendProperty(FM_SOFT_MUTE_MAX_ATTENUATION, smattn);
}
void si4735_setFmBandwidth(uint8_t filter_value)
{
    si4735_sendProperty(FM_CHANNEL_FILTER, filter_value);
}
void si4735_setFmNoiseBlankThreshold(uint16_t parameter)
{
    si4735_sendProperty(FM_NB_DETECT_THRESHOLD, parameter);
}
void si4735_setFmNoiseBlankInterval(uint16_t parameter)
{
    si4735_sendProperty(FM_NB_INTERVAL, parameter);
}
void si4735_setFmNoiseBlankRate(uint16_t parameter)
{
    si4735_sendProperty(FM_NB_RATE, parameter);
}
void si4735_setFmNoiseBlankDelay(uint16_t parameter)
{
    si4735_sendProperty(FM_NB_DELAY, parameter);
}
void si4735_setFmNoiseBlank_IIR_Filter(uint16_t parameter)
{
    si4735_sendProperty(FM_NB_IIR_FILTER, parameter);
}
void si4735_setFmNoiseBlank(uint16_t nb_rate, uint16_t nb_interval,
                             uint16_t nb_irr_filter)
{
    si4735_sendProperty(FM_NB_RATE,       nb_rate);
    si4735_sendProperty(FM_NB_INTERVAL,   nb_interval);
    si4735_sendProperty(FM_NB_IIR_FILTER, nb_irr_filter);
}

/* =========================================================================
 * FM stereo blend
 * ========================================================================= */

void si4735_setFmBlendStereoThreshold(uint8_t parameter)
{
    si4735_sendProperty(FM_BLEND_STEREO_THRESHOLD, parameter);
}
void si4735_setFmBlendMonoThreshold(uint8_t parameter)
{
    si4735_sendProperty(FM_BLEND_MONO_THRESHOLD, parameter);
}
void si4735_setFmBlendRssiStereoThreshold(uint8_t parameter)
{
    si4735_sendProperty(FM_BLEND_RSSI_STEREO_THRESHOLD, parameter);
}
void si4735_setFmBLendRssiMonoThreshold(uint8_t parameter)
{
    si4735_sendProperty(FM_BLEND_RSSI_MONO_THRESHOLD, parameter);
}
void si4735_setFmBlendSnrStereoThreshold(uint8_t parameter)
{
    si4735_sendProperty(FM_BLEND_SNR_STEREO_THRESHOLD, parameter);
}
void si4735_setFmBLendSnrMonoThreshold(uint8_t parameter)
{
    si4735_sendProperty(FM_BLEND_SNR_MONO_THRESHOLD, parameter);
}
void si4735_setFmBlendMultiPathStereoThreshold(uint8_t parameter)
{
    si4735_sendProperty(FM_BLEND_MULTIPATH_STEREO_THRESHOLD, parameter);
}
void si4735_setFmBlendMultiPathMonoThreshold(uint8_t parameter)
{
    si4735_sendProperty(FM_BLEND_MULTIPATH_MONO_THRESHOLD, parameter);
}
void si4735_setFmStereoOff(void) { /* TODO */ }
void si4735_setFmStereoOn(void)  { /* TODO */ }

static void _disableFmDebug(void)
{
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(0x12);
    hal_i2c_write(0x00);
    hal_i2c_write(0xFF);
    hal_i2c_write(0x00);
    hal_i2c_write(0x00);
    hal_i2c_write(0x00);
    hal_i2c_end_transmission();
    hal_delay_us(2500);
}

/* =========================================================================
 * SSB AGC helpers
 * ========================================================================= */

void si4735_setSsbSoftMuteMaxAttenuation(uint8_t smattn)
{
    si4735_sendProperty(SSB_SOFT_MUTE_MAX_ATTENUATION, smattn);
}
void si4735_setSsbIfAgcReleaseRate(uint8_t param)
{
    si4735_sendProperty(SSB_IF_AGC_RELEASE_RATE, param);
}
void si4735_setSsbIfAgcAttackRate(uint8_t param)
{
    si4735_sendProperty(SSB_IF_AGC_ATTACK_RATE, param);
}
void si4735_setSsbAgcAttackRate(uint16_t parameter)
{
    si4735_sendProperty(SSB_RF_AGC_ATTACK_RATE, parameter);
}
void si4735_setSsbAgcReleaseRate(uint16_t parameter)
{
    si4735_sendProperty(SSB_RF_AGC_RELEASE_RATE, parameter);
}

/* =========================================================================
 * Volume & audio mute
 * ========================================================================= */

void si4735_setVolume(uint8_t vol)
{
    si4735_sendProperty(RX_VOLUME, vol);
    volume = vol;
}
uint8_t si4735_getVolume(void)        { return volume; }
uint8_t si4735_getCurrentVolume(void) { return volume; }

void si4735_volumeUp(void)
{
    if (volume < 63) volume++;
    si4735_setVolume(volume);
}
void si4735_volumeDown(void)
{
    if (volume > 0) volume--;
    si4735_setVolume(volume);
}

void si4735_setAudioMute(bool off)
{
    uint16_t value = off ? 3u : 0u;
    si4735_sendProperty(RX_HARD_MUTE, value);
}

void si4735_setAudioMode(uint8_t audioMode)
{
    currentAudioMode = audioMode;
}

void si4735_setAudioMuteMcuPin(int8_t pin)
{
    audioMuteMcuPin = pin;
    hal_gpio_mute_init(pin);
}

void si4735_setHardwareAudioMute(bool on)
{
    hal_gpio_mute_set(on);
    hal_delay_us(300);
}

void si4735_digitalOutputFormat(uint8_t OSIZE, uint8_t OMONO,
                                  uint8_t OMODE, uint8_t OFALL)
{
    si4735_digital_output_format df;
    df.refined.OSIZE  = OSIZE;
    df.refined.OMONO  = OMONO;
    df.refined.OMODE  = OMODE;
    df.refined.OFALL  = OFALL;
    df.refined.dummy  = 0;
    si4735_sendProperty(DIGITAL_OUTPUT_FORMAT, df.raw);
}

void si4735_digitalOutputSampleRate(uint16_t DOSR)
{
    si4735_sendProperty(DIGITAL_OUTPUT_SAMPLE_RATE, DOSR);
}

/* =========================================================================
 * I2C speed helpers
 * ========================================================================= */

void si4735_setI2CLowSpeedMode(void)          { hal_i2c_set_clock(10000UL);  }
void si4735_setI2CStandardMode(void)          { hal_i2c_set_clock(100000UL); }
void si4735_setI2CFastMode(void)              { hal_i2c_set_clock(400000UL); }
void si4735_setI2CFastModeCustom(uint32_t hz) { hal_i2c_set_clock(hz);       }

/* =========================================================================
 * Timing delay setters
 * ========================================================================= */

void si4735_setMaxDelayPowerUp(uint16_t ms)      { maxDelayAfterPowerUp  = ms; }
void si4735_setMaxDelaySetFrequency(uint16_t ms) { maxDelaySetFrequency  = ms; }
void si4735_setMaxSeekTime(uint32_t time_in_ms)  { maxSeekTime           = time_in_ms; }

/* =========================================================================
 * Seek
 * ========================================================================= */

void si4735_seekStation(uint8_t SEEKUP, uint8_t WRAP)
{
    si47x_seek seek;
    si47x_seek_am_complement seek_am_complement;
    uint8_t seek_start_cmd = (currentTune == FM_TUNE_FREQ)
                             ? FM_SEEK_START : AM_SEEK_START;

    si4735_waitToSend();

    seek.arg.SEEKUP    = SEEKUP;
    seek.arg.WRAP      = WRAP;
    seek.arg.RESERVED1 = 0;
    seek.arg.RESERVED2 = 0;

    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(seek_start_cmd);
    hal_i2c_write(seek.raw);

    if (seek_start_cmd == AM_SEEK_START) {
        seek_am_complement.ARG2    = 0;
        seek_am_complement.ARG3    = 0;
        seek_am_complement.ANTCAPH = 0;
        seek_am_complement.ANTCAPL = (currentWorkFrequency > 1800) ? 1 : 0;
        hal_i2c_write(seek_am_complement.ARG2);
        hal_i2c_write(seek_am_complement.ARG3);
        hal_i2c_write(seek_am_complement.ANTCAPH);
        hal_i2c_write(seek_am_complement.ANTCAPL);
    }
    hal_i2c_end_transmission();
    hal_delay_ms((uint32_t)MAX_DELAY_AFTER_SET_FREQUENCY << 2);
}

void si4735_seekNextStation(void)
{
    si4735_seekStation(1, 1);
    hal_delay_ms(maxDelaySetFrequency);
    si4735_getFrequency();
}

void si4735_seekPreviousStation(void)
{
    si4735_seekStation(0, 1);
    hal_delay_ms(maxDelaySetFrequency);
    si4735_getFrequency();
}

void si4735_seekStationProgress(void (*showFunc)(uint16_t f), uint8_t up_down)
{
    si47x_frequency freq;
    uint32_t elapsed_seek;

    if (lastMode == SSB_CURRENT_MODE) return;

    elapsed_seek = 0; /* TODO: replace with hal_get_tick_ms() if available */

    do {
        si4735_seekStation(up_down, 0);
        hal_delay_ms(maxDelaySetFrequency);
        si4735_getStatus(0, 0);
        hal_delay_ms(maxDelaySetFrequency);
        freq.raw.FREQH       = currentStatus.resp.READFREQH;
        freq.raw.FREQL       = currentStatus.resp.READFREQL;
        currentWorkFrequency = freq.value;
        if (showFunc != NULL) showFunc(freq.value);
        elapsed_seek += (uint32_t)(maxDelaySetFrequency * 2 +
                                   (MAX_DELAY_AFTER_SET_FREQUENCY << 2));
    } while (!currentStatus.resp.VALID
          && !currentStatus.resp.BLTF
          && elapsed_seek < maxSeekTime);
}

void si4735_seekStationProgressWithStop(void (*showFunc)(uint16_t f),
                                         bool (*stopSeeking)(void),
                                         uint8_t up_down)
{
    si47x_frequency freq;
    uint32_t elapsed_seek = 0;

    if (lastMode == SSB_CURRENT_MODE) return;

    do {
        si4735_seekStation(up_down, 0);
        hal_delay_ms(maxDelaySetFrequency);
        si4735_getStatus(0, 0);
        hal_delay_ms(maxDelaySetFrequency);
        freq.raw.FREQH       = currentStatus.resp.READFREQH;
        freq.raw.FREQL       = currentStatus.resp.READFREQL;
        currentWorkFrequency = freq.value;
        if (showFunc != NULL) showFunc(freq.value);
        if (stopSeeking != NULL && stopSeeking()) return;
        elapsed_seek += (uint32_t)(maxDelaySetFrequency * 2 +
                                   (MAX_DELAY_AFTER_SET_FREQUENCY << 2));
    } while (!currentStatus.resp.VALID
          && !currentStatus.resp.BLTF
          && elapsed_seek < maxSeekTime);
}

void si4735_setSeekAmLimits(uint16_t bottom, uint16_t top)
{
    si4735_sendProperty(AM_SEEK_BAND_BOTTOM, bottom);
    si4735_sendProperty(AM_SEEK_BAND_TOP,    top);
}
void si4735_setSeekAmSpacing(uint16_t spacing)
{
    si4735_sendProperty(AM_SEEK_FREQ_SPACING, spacing);
}
void si4735_setSeekAmRssiThreshold(uint16_t value)
{
    si4735_sendProperty(AM_SEEK_RSSI_THRESHOLD, value);
}
void si4735_setSeekAmSNRThreshold(uint16_t value)
{
    si4735_sendProperty(AM_SEEK_SNR_THRESHOLD, value);
}
void si4735_setSeekFmLimits(uint16_t bottom, uint16_t top)
{
    si4735_sendProperty(FM_SEEK_BAND_BOTTOM, bottom);
    si4735_sendProperty(FM_SEEK_BAND_TOP,    top);
}
void si4735_setSeekFmSpacing(uint16_t spacing)
{
    si4735_sendProperty(FM_SEEK_FREQ_SPACING, spacing);
}
void si4735_setSeekFmRssiThreshold(uint16_t value)
{
    si4735_sendProperty(FM_SEEK_TUNE_RSSI_THRESHOLD, value);
}
void si4735_setSeekFmSNRThreshold(uint16_t value)
{
    si4735_sendProperty(FM_SEEK_TUNE_SNR_THRESHOLD, value);
}

/* =========================================================================
 * Generic property / command
 * ========================================================================= */

void si4735_sendProperty(uint16_t propertyNumber, uint16_t parameter)
{
    si47x_property property;
    si47x_property param;

    property.value = propertyNumber;
    param.value    = parameter;

    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(SET_PROPERTY);
    hal_i2c_write(0x00);
    hal_i2c_write(property.raw.byteHigh);
    hal_i2c_write(property.raw.byteLow);
    hal_i2c_write(param.raw.byteHigh);
    hal_i2c_write(param.raw.byteLow);
    hal_i2c_end_transmission();
    hal_delay_us(550);
}

void si4735_setProperty(uint16_t propertyNumber, uint16_t param)
{
    si4735_sendProperty(propertyNumber, param);
}

int32_t si4735_getProperty(uint16_t propertyNumber)
{
    si47x_property property;
    si47x_status   status;

    property.value = propertyNumber;
    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(GET_PROPERTY);
    hal_i2c_write(0x00);
    hal_i2c_write(property.raw.byteHigh);
    hal_i2c_write(property.raw.byteLow);
    hal_i2c_end_transmission();

    si4735_waitToSend();
    hal_i2c_request_from((uint8_t)deviceAddress, 4);
    status.raw = hal_i2c_read();
    if (status.refined.ERR) return -1;
    hal_i2c_read(); /* dummy */
    property.raw.byteHigh = hal_i2c_read();
    property.raw.byteLow  = hal_i2c_read();
    return (int32_t)property.value;
}

void si4735_sendCommand(uint8_t cmd, int parameter_size,
                         const uint8_t *parameter)
{
    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(cmd);
    for (int i = 0; i < parameter_size; i++)
        hal_i2c_write(parameter[i]);
    hal_i2c_end_transmission();
}

void si4735_getCommandResponse(int response_size, uint8_t *response)
{
    si4735_waitToSend();
    hal_i2c_request_from((uint8_t)deviceAddress, (uint8_t)response_size);
    for (int i = 0; i < response_size; i++)
        response[i] = hal_i2c_read();
}

si47x_status si4735_getStatusResponse(void)
{
    si47x_status status;
    hal_i2c_request_from((uint8_t)deviceAddress, 1);
    status.raw = hal_i2c_read();
    return status;
}

/* =========================================================================
 * RDS
 * ========================================================================= */

void si4735_clearRdsBuffer2A(void)
{
    memset(rds_buffer2A, 0, sizeof(rds_buffer2A));
}
void si4735_clearRdsProgramInformation(void) { si4735_clearRdsBuffer2A(); }
void si4735_clearRdsBuffer2B(void)
{
    memset(rds_buffer2B, 0, sizeof(rds_buffer2B));
}
void si4735_clearRdsStationInformation(void) { si4735_clearRdsBuffer2B(); }
void si4735_clearRdsBuffer0A(void)
{
    memset(rds_buffer0A, 0, sizeof(rds_buffer0A));
}
void si4735_clearRdsStationName(void) { si4735_clearRdsBuffer0A(); }

void si4735_RdsInit(void)
{
    si4735_clearRdsBuffer2A();
    si4735_clearRdsBuffer2B();
    si4735_clearRdsBuffer0A();
    rdsTextAdress2A = rdsTextAdress2B = rdsTextAdress0A = 0;
    lastTextFlagAB  = 0;
}

void si4735_clearRdsBuffer(void) { si4735_RdsInit(); }

void si4735_setRdsConfig(uint8_t RDSEN, uint8_t BLETHA, uint8_t BLETHB,
                          uint8_t BLETHC, uint8_t BLETHD)
{
    si47x_property   property;
    si47x_rds_config config;

    si4735_waitToSend();
    property.value = FM_RDS_CONFIG;

    config.arg.RDSEN  = RDSEN;
    config.arg.BLETHA = BLETHA;
    config.arg.BLETHB = BLETHB;
    config.arg.BLETHC = BLETHC;
    config.arg.BLETHD = BLETHD;
    config.arg.DUMMY1 = 0;

    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(SET_PROPERTY);
    hal_i2c_write(0x00);
    hal_i2c_write(property.raw.byteHigh);
    hal_i2c_write(property.raw.byteLow);
    hal_i2c_write(config.raw[1]);
    hal_i2c_write(config.raw[0]);
    hal_i2c_end_transmission();
    hal_delay_us(550);

    si4735_RdsInit();
}

void si4735_setRdsIntSource(uint8_t RDSRECV, uint8_t RDSSYNCLOST,
                             uint8_t RDSSYNCFOUND, uint8_t RDSNEWBLOCKA,
                             uint8_t RDSNEWBLOCKB)
{
    si47x_property      property;
    si47x_rds_int_source rds_int_source;

    if (currentTune != FM_TUNE_FREQ) return;

    rds_int_source.refined.RDSNEWBLOCKB = RDSNEWBLOCKB;
    rds_int_source.refined.RDSNEWBLOCKA = RDSNEWBLOCKA;
    rds_int_source.refined.RDSSYNCFOUND = RDSSYNCFOUND;
    rds_int_source.refined.RDSSYNCLOST  = RDSSYNCLOST;
    rds_int_source.refined.RDSRECV      = RDSRECV;
    rds_int_source.refined.DUMMY1       = 0;
    rds_int_source.refined.DUMMY2       = 0;
    property.value                      = FM_RDS_INT_SOURCE;

    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(SET_PROPERTY);
    hal_i2c_write(0x00);
    hal_i2c_write(property.raw.byteHigh);
    hal_i2c_write(property.raw.byteLow);
    hal_i2c_write(rds_int_source.raw[1]);
    hal_i2c_write(rds_int_source.raw[0]);
    hal_i2c_end_transmission();
    si4735_waitToSend();
}

void si4735_getRdsStatus(uint8_t INTACK, uint8_t MTFIFO, uint8_t STATUSONLY)
{
    si47x_rds_command rds_cmd;
    static uint16_t   lastFreq = 0;

    if (currentTune != FM_TUNE_FREQ) return;

    if (lastFreq != currentWorkFrequency) {
        lastFreq = currentWorkFrequency;
        si4735_clearRdsBuffer2A();
        si4735_clearRdsBuffer2B();
        si4735_clearRdsBuffer0A();
    }

    si4735_waitToSend();
    rds_cmd.arg.INTACK     = INTACK;
    rds_cmd.arg.MTFIFO     = MTFIFO;
    rds_cmd.arg.STATUSONLY = STATUSONLY;

    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(FM_RDS_STATUS);
    hal_i2c_write(rds_cmd.raw);
    hal_i2c_end_transmission();

    do {
        si4735_waitToSend();
        hal_i2c_request_from((uint8_t)deviceAddress, 13);
        for (uint8_t i = 0; i < 13; i++)
            currentRdsStatus.raw[i] = hal_i2c_read();
    } while (currentRdsStatus.resp.ERR);
    hal_delay_us(550);
}

void si4735_getRdsStatusSimple(void) { si4735_getRdsStatus(0, 0, 0); }
void si4735_rdsBeginQuery(void)      { si4735_getRdsStatus(0, 0, 0); }
void si4735_rdsClearFifo(void)       { si4735_getRdsStatus(0, 1, 0); }
void si4735_rdsClearInterrupt(void)  { si4735_getRdsStatus(1, 0, 0); }

bool    si4735_getRdsReceived(void)    { return (bool)currentRdsStatus.resp.RDSRECV;      }
bool    si4735_getRdsSyncLost(void)    { return (bool)currentRdsStatus.resp.RDSSYNCLOST;  }
bool    si4735_getRdsSyncFound(void)   { return (bool)currentRdsStatus.resp.RDSSYNCFOUND; }
bool    si4735_getRdsNewBlockA(void)   { return (bool)currentRdsStatus.resp.RDSNEWBLOCKA; }
bool    si4735_getRdsNewBlockB(void)   { return (bool)currentRdsStatus.resp.RDSNEWBLOCKB; }
bool    si4735_getRdsSync(void)        { return (bool)currentRdsStatus.resp.RDSSYNC;      }
bool    si4735_getGroupLost(void)      { return (bool)currentRdsStatus.resp.GRPLOST;      }
uint8_t si4735_getNumRdsFifoUsed(void) { return currentRdsStatus.resp.RDSFIFOUSED; }

void si4735_setFifoCount(uint16_t value)
{
    si4735_sendProperty(FM_RDS_INT_FIFO_COUNT, value);
}

bool si4735_getEndIndicatorGroupA(void) { return rdsEndGroupA; }
void si4735_resetEndIndicatorGroupA(void) { rdsEndGroupA = false; }
bool si4735_getEndIndicatorGroupB(void) { return rdsEndGroupB; }
void si4735_resetEndIndicatorGroupB(void) { rdsEndGroupB = false; }

uint16_t si4735_getRdsPI(void)
{
    if (si4735_getRdsReceived() && si4735_getRdsNewBlockA())
        return currentRdsStatus.resp.BLOCKAL;
    return 0;
}

uint8_t si4735_getRdsGroupType(void)
{
    si47x_rds_blockb blkb;
    blkb.raw.lowValue  = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    return blkb.refined.groupType;
}

uint8_t si4735_getRdsFlagAB(void)
{
    si47x_rds_blockb blkb;
    blkb.raw.lowValue  = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    return blkb.refined.textABFlag;
}

uint8_t si4735_getRdsVersionCode(void)
{
    si47x_rds_blockb blkb;
    blkb.raw.lowValue  = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    return blkb.refined.versionCode;
}

uint8_t si4735_getRdsProgramType(void)
{
    si47x_rds_blockb blkb;
    blkb.raw.lowValue  = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    return blkb.refined.programType;
}

uint8_t si4735_getRdsTextSegmentAddress(void)
{
    si47x_rds_blockb blkb;
    blkb.raw.lowValue  = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    return (uint8_t)blkb.refined.content;
}

void si4735_getNext2Block(char *c)
{
    c[0] = (char)currentRdsStatus.resp.BLOCKDH;
    c[1] = (char)currentRdsStatus.resp.BLOCKDL;
}

void si4735_getNext4Block(char *c)
{
    c[0] = (char)currentRdsStatus.resp.BLOCKCH;
    c[1] = (char)currentRdsStatus.resp.BLOCKCL;
    c[2] = (char)currentRdsStatus.resp.BLOCKDH;
    c[3] = (char)currentRdsStatus.resp.BLOCKDL;
}

char *si4735_getRdsText(void)
{
    if (rdsTextAdress2A >= 16) rdsTextAdress2A = 0;
    si4735_getNext4Block(&rds_buffer2A[rdsTextAdress2A * 4]);
    rdsTextAdress2A += 4;
    return rds_buffer2A;
}

char *si4735_getRdsText0A(void)
{
    si47x_rds_blockb blkB;

    if (!si4735_getRdsReceived()) return NULL;
    if (si4735_getRdsGroupType() != 0) return NULL;

    if (lastTextFlagAB != si4735_getRdsFlagAB()) {
        lastTextFlagAB = si4735_getRdsFlagAB();
        si4735_clearRdsBuffer0A();
    }

    blkB.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    blkB.raw.lowValue  = currentRdsStatus.resp.BLOCKBL;
    rdsTextAdress0A    = blkB.group0.address;

    if (rdsTextAdress0A >= 0 && rdsTextAdress0A < 4) {
        si4735_getNext2Block(&rds_buffer0A[rdsTextAdress0A * 2]);
        rds_buffer0A[8] = '\0';
        return rds_buffer0A;
    }
    return NULL;
}

char *si4735_getRdsText2A(void)
{
    si47x_rds_blockb blkB;

    if (!si4735_getRdsReceived()) return NULL;
    if (si4735_getRdsGroupType() != 2) return NULL;

    blkB.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    blkB.raw.lowValue  = currentRdsStatus.resp.BLOCKBL;
    rdsTextAdress2A    = blkB.group2.address;

    if (rdsTextAdress2A >= 0 && rdsTextAdress2A < 16) {
        si4735_getNext4Block(&rds_buffer2A[rdsTextAdress2A * 4]);
        rds_buffer2A[63] = '\0';
        return rds_buffer2A;
    }
    return NULL;
}

char *si4735_getRdsText2B(void)
{
    si47x_rds_blockb blkB;

    if (si4735_getRdsGroupType() != 2) return NULL;

    blkB.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    blkB.raw.lowValue  = currentRdsStatus.resp.BLOCKBL;
    rdsTextAdress2B    = blkB.group2.address;

    if (rdsTextAdress2B >= 0 && rdsTextAdress2B < 16) {
        si4735_getNext2Block(&rds_buffer2B[rdsTextAdress2B * 2]);
        rds_buffer2B[32] = '\0';
        return rds_buffer2B;
    }
    return NULL;
}

bool si4735_getRdsAllData(char **stationName, char **stationInformation,
                           char **programInformation, char **utcTime)
{
    si4735_rdsBeginQuery();
    if (!si4735_getRdsReceived()) return false;
    if (!si4735_getRdsSync() || si4735_getNumRdsFifoUsed() == 0) return false;

    *stationName        = si4735_getRdsText0A();
    *stationInformation = si4735_getRdsText2B();
    *programInformation = si4735_getRdsText2A();
    *utcTime            = si4735_getRdsTime();

    return (bool)(*stationName)
        || (bool)(*stationInformation)
        || (bool)(*programInformation)
        || (bool)(*utcTime);
}

void si4735_mjdConverter(uint32_t mjd, uint32_t *year,
                          uint32_t *month, uint32_t *day)
{
    uint32_t jd  = mjd + 2400001UL;
    uint32_t ljd = jd + 68569UL;
    uint32_t njd = (uint32_t)(4UL * ljd / 146097UL);
    ljd  = ljd - (uint32_t)((146097UL * njd + 3UL) / 4UL);
    *year  = (uint32_t)(4000UL * (ljd + 1UL) / 1461001UL);
    ljd  = ljd - (uint32_t)((1461UL * (*year) / 4UL)) + 31UL;
    *month = (uint32_t)(80UL * ljd / 2447UL);
    *day   = ljd - (uint32_t)(2447UL * (*month) / 80UL);
    ljd    = (uint32_t)(*month / 11UL);
    *month = (uint32_t)(*month + 2UL - 12UL * ljd);
    *year  = (uint32_t)(100UL * (njd - 49UL) + (*year) + ljd);
}

char *si4735_getRdsTime(void)
{
    si47x_rds_date_time dt;
    uint16_t minute, hour;

    if (si4735_getRdsGroupType() != 4) return NULL;

    dt.raw[4] = currentRdsStatus.resp.BLOCKBL;
    dt.raw[5] = currentRdsStatus.resp.BLOCKBH;
    dt.raw[2] = currentRdsStatus.resp.BLOCKCL;
    dt.raw[3] = currentRdsStatus.resp.BLOCKCH;
    dt.raw[0] = currentRdsStatus.resp.BLOCKDL;
    dt.raw[1] = currentRdsStatus.resp.BLOCKDH;

    minute = dt.refined.minute;
    hour   = dt.refined.hour;

    char offset_sign = (dt.refined.offset_sense == 1) ? '+' : '-';
    int  offset_h    = (int)((dt.refined.offset * 30) / 60);
    int  offset_m    = (int)((dt.refined.offset * 30) - (uint32_t)(offset_h * 60));

    if (offset_h > 12 || offset_m > 60 || hour > 24 || minute > 60)
        return NULL;

    si4735_convertToChar(hour,     rds_time,      2, 0, ' ', false);
    rds_time[2] = ':';
    si4735_convertToChar(minute,   &rds_time[3],  2, 0, ' ', false);
    rds_time[5] = ' ';
    rds_time[6] = offset_sign;
    si4735_convertToChar((uint16_t)offset_h, &rds_time[7],  2, 0, ' ', false);
    rds_time[9]  = ':';
    si4735_convertToChar((uint16_t)offset_m, &rds_time[10], 2, 0, ' ', false);
    rds_time[12] = '\0';
    return rds_time;
}

char *si4735_getRdsDateTime(void)
{
    si47x_rds_date_time dt;
    uint16_t minute, hour;
    uint32_t mjd = 0, day, month, year;

    if (si4735_getRdsGroupType() != 4) return NULL;

    dt.raw[4] = currentRdsStatus.resp.BLOCKBL;
    dt.raw[5] = currentRdsStatus.resp.BLOCKBH;
    dt.raw[2] = currentRdsStatus.resp.BLOCKCL;
    dt.raw[3] = currentRdsStatus.resp.BLOCKCH;
    dt.raw[0] = currentRdsStatus.resp.BLOCKDL;
    dt.raw[1] = currentRdsStatus.resp.BLOCKDH;

    mjd    = dt.refined.mjd;
    minute = dt.refined.minute;
    hour   = dt.refined.hour;

    si4735_mjdConverter(mjd, &year, &month, &day);

    char offset_sign = (dt.refined.offset_sense == 1) ? '+' : '-';
    int  offset_h    = (int)((dt.refined.offset * 30) / 60);
    int  offset_m    = (int)((dt.refined.offset * 30) - (uint32_t)(offset_h * 60));

    if (offset_h > 12 || offset_m > 60 || hour > 24 || minute > 60
            || day > 31 || month > 12)
        return NULL;

    si4735_convertToChar((uint16_t)year,   rds_time,       4, 0, ' ', false);
    rds_time[4]  = '-';
    si4735_convertToChar((uint16_t)month,  &rds_time[5],   2, 0, ' ', false);
    rds_time[7]  = '-';
    si4735_convertToChar((uint16_t)day,    &rds_time[8],   2, 0, ' ', false);
    rds_time[10] = ' ';
    si4735_convertToChar(hour,             &rds_time[11],  2, 0, ' ', false);
    rds_time[13] = ':';
    si4735_convertToChar(minute,           &rds_time[14],  2, 0, ' ', false);
    rds_time[16] = ' ';
    rds_time[17] = offset_sign;
    si4735_convertToChar((uint16_t)offset_h, &rds_time[18], 2, 0, ' ', false);
    rds_time[20] = ':';
    si4735_convertToChar((uint16_t)offset_m, &rds_time[21], 2, 0, ' ', false);
    rds_time[23] = '\0';
    return rds_time;
}

bool si4735_getRdsDateTimeParts(uint16_t *rYear, uint16_t *rMonth,
                                 uint16_t *rDay,  uint16_t *rHour,
                                 uint16_t *rMinute)
{
    si47x_rds_date_time dt;
    int16_t  local_minute;
    uint16_t minute, hour;
    uint32_t mjd, day, month, year;

    if (si4735_getRdsGroupType() != 4) return false;

    dt.raw[4] = currentRdsStatus.resp.BLOCKBL;
    dt.raw[5] = currentRdsStatus.resp.BLOCKBH;
    dt.raw[2] = currentRdsStatus.resp.BLOCKCL;
    dt.raw[3] = currentRdsStatus.resp.BLOCKCH;
    dt.raw[0] = currentRdsStatus.resp.BLOCKDL;
    dt.raw[1] = currentRdsStatus.resp.BLOCKDH;

    mjd    = dt.refined.mjd;
    minute = dt.refined.minute;
    hour   = dt.refined.hour;

    local_minute = (int16_t)(((int16_t)hour * 60 + (int16_t)minute)
                   + (int16_t)(dt.refined.offset * 30)
                     * (dt.refined.offset_sense == 1 ? -1 : 1));

    if (local_minute < 0)    { local_minute += 1440; mjd--; }
    else if (local_minute > 1440) { local_minute -= 1440; mjd++; }

    si4735_mjdConverter(mjd, &year, &month, &day);
    hour   = (uint16_t)local_minute / 60;
    minute = (uint16_t)local_minute - (hour * 60u);

    if (hour > 24 || minute > 60 || day > 31 || month > 12) return false;

    *rYear   = (uint16_t)year;
    *rMonth  = (uint16_t)month;
    *rDay    = (uint16_t)day;
    *rHour   = hour;
    *rMinute = minute;
    return true;
}

/* =========================================================================
 * SSB
 * ========================================================================= */

static void _sendSSBModeProperty(void)
{
    si47x_property property;
    property.value = SSB_MODE;
    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(SET_PROPERTY);
    hal_i2c_write(0x00);
    hal_i2c_write(property.raw.byteHigh);
    hal_i2c_write(property.raw.byteLow);
    hal_i2c_write(currentSSBMode.raw[1]);
    hal_i2c_write(currentSSBMode.raw[0]);
    hal_i2c_end_transmission();
    hal_delay_us(550);
}

void si4735_setSSBBfo(int offset)
{
    si47x_property  property;
    si47x_frequency bfo_offset;

    if (currentTune == FM_TUNE_FREQ) return;

    si4735_waitToSend();
    property.value   = SSB_BFO;
    bfo_offset.value = (uint16_t)offset;

    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(SET_PROPERTY);
    hal_i2c_write(0x00);
    hal_i2c_write(property.raw.byteHigh);
    hal_i2c_write(property.raw.byteLow);
    hal_i2c_write(bfo_offset.raw.FREQH);
    hal_i2c_write(bfo_offset.raw.FREQL);
    hal_i2c_end_transmission();
    hal_delay_us(550);
}

void si4735_setSSBConfig(uint8_t AUDIOBW, uint8_t SBCUTFLT,
                          uint8_t AVC_DIVIDER, uint8_t AVCEN,
                          uint8_t SMUTESEL, uint8_t DSP_AFCDIS)
{
    if (currentTune == FM_TUNE_FREQ) return;
    currentSSBMode.param.AUDIOBW     = AUDIOBW;
    currentSSBMode.param.SBCUTFLT    = SBCUTFLT;
    currentSSBMode.param.AVC_DIVIDER = AVC_DIVIDER;
    currentSSBMode.param.AVCEN       = AVCEN;
    currentSSBMode.param.SMUTESEL    = SMUTESEL;
    currentSSBMode.param.DUMMY1      = 0;
    currentSSBMode.param.DSP_AFCDIS  = DSP_AFCDIS;
    _sendSSBModeProperty();
}

void si4735_setSSBAudioBandwidth(uint8_t AUDIOBW)
{
    currentSSBMode.param.AUDIOBW = AUDIOBW;
    _sendSSBModeProperty();
}
void si4735_setSSBAutomaticVolumeControl(uint8_t AVCEN)
{
    currentSSBMode.param.AVCEN = AVCEN;
    _sendSSBModeProperty();
}
void si4735_setSSBSidebandCutoffFilter(uint8_t SBCUTFLT)
{
    currentSSBMode.param.SBCUTFLT = SBCUTFLT;
    _sendSSBModeProperty();
}
void si4735_setSSBAvcDivider(uint8_t AVC_DIVIDER)
{
    currentSSBMode.param.AVC_DIVIDER = AVC_DIVIDER;
    _sendSSBModeProperty();
}
void si4735_setSSBDspAfc(uint8_t DSP_AFCDIS)
{
    currentSSBMode.param.DSP_AFCDIS = DSP_AFCDIS;
    _sendSSBModeProperty();
}
void si4735_setSSBSoftMute(uint8_t SMUTESEL)
{
    currentSSBMode.param.SMUTESEL = SMUTESEL;
    _sendSSBModeProperty();
}

void si4735_setSSBMode(uint8_t usblsb)
{
    si4735_setPowerUp(ctsIntEnable, 0, 0, currentClockType, 1, currentAudioMode);
    si4735_radioPowerUp();
    si4735_setVolume(volume);
    currentSsbStatus = usblsb;
    lastMode         = SSB_CURRENT_MODE;
}

void si4735_setSSB(uint16_t fromFreq, uint16_t toFreq,
                   uint16_t initialFreq, uint16_t step, uint8_t usblsb)
{
    currentMinimumFrequency = fromFreq;
    currentMaximumFrequency = toFreq;
    currentStep             = step;
    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;
    si4735_setSSBMode(usblsb);
    currentWorkFrequency = initialFreq;
    si4735_setFrequency(currentWorkFrequency);
}

void si4735_getSsbAgcStatus(void)
{
    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(SSB_AGC_STATUS);
    hal_i2c_end_transmission();
    do {
        si4735_waitToSend();
        hal_i2c_request_from((uint8_t)deviceAddress, 3);
        currentAgcStatus.raw[0] = hal_i2c_read();
        currentAgcStatus.raw[1] = hal_i2c_read();
        currentAgcStatus.raw[2] = hal_i2c_read();
    } while (currentAgcStatus.refined.ERR);
}

/* =========================================================================
 * Patch support
 * ========================================================================= */

si47x_firmware_query_library si4735_queryLibraryId(void)
{
    si47x_firmware_query_library libraryID;

    si4735_powerDown();
    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(POWER_UP);
    hal_i2c_write(0b00011111);
    hal_i2c_write(SI473X_ANALOG_AUDIO);
    hal_i2c_end_transmission();

    do {
        si4735_waitToSend();
        hal_i2c_request_from((uint8_t)deviceAddress, 8);
        for (int i = 0; i < 8; i++)
            libraryID.raw[i] = hal_i2c_read();
    } while (libraryID.resp.ERR);

    hal_delay_us(2500);
    return libraryID;
}

void si4735_patchPowerUp(void)
{
    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(POWER_UP);
    hal_i2c_write(0b00110001);
    hal_i2c_write(SI473X_ANALOG_AUDIO);
    hal_i2c_end_transmission();
    hal_delay_ms(maxDelayAfterPowerUp);
}

void si4735_ssbPowerUp(void)
{
    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(POWER_UP);
    hal_i2c_write(0b00010001);
    hal_i2c_write(0b00000101);
    hal_i2c_end_transmission();
    hal_delay_us(2500);

    powerUp.arg.CTSIEN  = ctsIntEnable;
    powerUp.arg.GPO2OEN = 0;
    powerUp.arg.PATCH   = 0;
    powerUp.arg.XOSCEN  = currentClockType;
    powerUp.arg.FUNC    = 1;
    powerUp.arg.OPMODE  = 0b00000101;
}

bool si4735_downloadPatch(const uint8_t *ssb_patch_content,
                           uint16_t ssb_patch_content_size)
{
    uint8_t content;
    for (uint16_t offset = 0; offset < ssb_patch_content_size; offset += 8) {
        hal_i2c_begin_transmission((uint8_t)deviceAddress);
        for (uint16_t i = 0; i < 8; i++) {
            /* On PIC32, flash/const arrays are directly accessible */
            content = ssb_patch_content[i + offset];
            hal_i2c_write(content);
        }
        hal_i2c_end_transmission();
        hal_delay_us(MIN_DELAY_WAIT_SEND_LOOP);
    }
    hal_delay_us(250);
    return true;
}

bool si4735_downloadCompressedPatch(const uint8_t *ssb_patch_content,
                                     uint16_t ssb_patch_content_size,
                                     const uint16_t *cmd_0x15,
                                     int16_t cmd_0x15_size)
{
    uint8_t  cmd;
    uint16_t command_line = 0;
    int16_t  num_0x15_entries = cmd_0x15_size / (int16_t)sizeof(uint16_t);

    for (uint16_t offset = 0; offset < ssb_patch_content_size; offset += 7) {
        cmd = 0x16;
        for (int16_t i = 0; i < num_0x15_entries; i++) {
            if (cmd_0x15[i] == command_line) {
                cmd = 0x15;
                break;
            }
        }
        hal_i2c_begin_transmission((uint8_t)deviceAddress);
        hal_i2c_write(cmd);
        for (uint16_t i = 0; i < 7; i++)
            hal_i2c_write(ssb_patch_content[i + offset]);
        hal_i2c_end_transmission();
        hal_delay_us(MIN_DELAY_WAIT_SEND_LOOP);
        command_line++;
    }
    hal_delay_us(250);
    return true;
}

void si4735_loadPatch(const uint8_t *ssb_patch_content,
                       uint16_t ssb_patch_content_size,
                       uint8_t ssb_audiobw)
{
    si4735_queryLibraryId();
    si4735_patchPowerUp();
    hal_delay_ms(50);
    si4735_downloadPatch(ssb_patch_content, ssb_patch_content_size);
    si4735_setSSBConfig(ssb_audiobw, 1, 0, 0, 0, 1);
    hal_delay_ms(25);
}

void si4735_loadCompressedPatch(const uint8_t *ssb_patch_content,
                                 uint16_t ssb_patch_content_size,
                                 const uint16_t *cmd_0x15,
                                 int16_t cmd_0x15_size,
                                 uint8_t ssb_audiobw)
{
    si4735_queryLibraryId();
    si4735_patchPowerUp();
    hal_delay_ms(50);
    si4735_downloadCompressedPatch(ssb_patch_content, ssb_patch_content_size,
                                   cmd_0x15, cmd_0x15_size);
    si4735_setSSBConfig(ssb_audiobw, 1, 0, 0, 0, 1);
    hal_delay_ms(25);
}

si4735_eeprom_patch_header si4735_downloadPatchFromEeprom(int eeprom_i2c_address)
{
    si4735_eeprom_patch_header eep;
    const int header_size = (int)sizeof(eep);
    uint8_t   bufferAux[8];
    int       offset, i;

    hal_i2c_begin_transmission((uint8_t)eeprom_i2c_address);
    hal_i2c_write(0x00);
    hal_i2c_write(0x00);
    hal_i2c_end_transmission();
    hal_delay_ms(5);

    for (int k = 0; k < header_size; k += 8) {
        hal_i2c_request_from((uint8_t)eeprom_i2c_address, 8);
        for (i = k; i < (k + 8); i++)
            eep.raw[i] = hal_i2c_read();
    }

    offset = header_size;
    for (i = 0; i < (int)eep.refined.patch_size; i += 8) {
        hal_i2c_begin_transmission((uint8_t)eeprom_i2c_address);
        hal_i2c_write((uint8_t)((uint16_t)offset >> 8));
        hal_i2c_write((uint8_t)((uint16_t)offset & 0xFF));
        hal_i2c_end_transmission();

        hal_i2c_request_from((uint8_t)eeprom_i2c_address, 8);
        for (int j = 0; j < 8; j++)
            bufferAux[j] = hal_i2c_read();

        hal_i2c_begin_transmission((uint8_t)deviceAddress);
        for (int j = 0; j < 8; j++)
            hal_i2c_write(bufferAux[j]);
        hal_i2c_end_transmission();

        si4735_waitToSend();
        uint8_t cmd_status;
        hal_i2c_request_from((uint8_t)deviceAddress, 1);
        cmd_status = hal_i2c_read();
        if (cmd_status != 0x80) {
            /* Signal error in patch_id field */
            eep.refined.patch_id[0] = 'E';
            eep.refined.patch_id[1] = 'R';
            eep.refined.patch_id[2] = 'R';
            eep.refined.patch_id[3] = '\0';
            return eep;
        }
        offset += 8;
    }
    hal_delay_ms(50);
    return eep;
}

/* =========================================================================
 * NBFM
 * ========================================================================= */

void si4735_patchPowerUpNBFM(void)
{
    si4735_waitToSend();
    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(POWER_UP);
    hal_i2c_write(0b00110000);
    hal_i2c_write(SI473X_ANALOG_AUDIO);
    hal_i2c_end_transmission();
    hal_delay_ms(maxDelayAfterPowerUp);
}

void si4735_loadPatchNBFM(const uint8_t *patch_content,
                           uint16_t patch_content_size)
{
    si4735_queryLibraryId();
    si4735_patchPowerUpNBFM();
    hal_delay_ms(50);
    si4735_downloadPatch(patch_content, patch_content_size);
    hal_delay_ms(25);
}

void si4735_setNBFM(void)
{
    si4735_setPowerUp(ctsIntEnable, gpo2Enable, 0, currentClockType,
                      0, currentAudioMode);
    si4735_radioPowerUp();
    currentTune      = NBFM_TUNE_FREQ;
    si4735_setVolume(volume);
    currentSsbStatus = 0;
    lastMode         = NBFM_CURRENT_MODE;
}

void si4735_setNBFMBand(uint16_t fromFreq, uint16_t toFreq,
                         uint16_t initialFreq, uint16_t step)
{
    currentMinimumFrequency = fromFreq;
    currentMaximumFrequency = toFreq;
    currentStep             = step;
    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;
    si4735_setNBFM();
    currentWorkFrequency = initialFreq;
    si4735_setFrequency(currentWorkFrequency);
}

void si4735_setFrequencyNBFM(uint16_t freq)
{
    si4735_waitToSend();
    currentFrequency.value             = freq;
    currentFrequencyParams.arg.FREQH   = currentFrequency.raw.FREQH;
    currentFrequencyParams.arg.FREQL   = currentFrequency.raw.FREQL;

    hal_i2c_begin_transmission((uint8_t)deviceAddress);
    hal_i2c_write(0x50);
    hal_i2c_write(0x00);
    hal_i2c_write(currentFrequency.raw.FREQH);
    hal_i2c_write(currentFrequency.raw.FREQL);
    hal_i2c_end_transmission();

    si4735_waitToSend();
    currentWorkFrequency = freq;
    hal_delay_ms(250);
}

/* =========================================================================
 * Utility
 * ========================================================================= */

void si4735_convertToChar(uint16_t value, char *strValue, uint8_t len,
                           uint8_t dot, uint8_t separator,
                           bool remove_leading_zeros)
{
    char d;
    int i;
    for (i = (int)(len - 1); i >= 0; i--) {
        d         = (char)(value % 10);
        value    /= 10;
        strValue[i] = d + '0';
    }
    strValue[len] = '\0';

    if (dot > 0) {
        for (i = (int)len; i >= (int)dot; i--)
            strValue[i + 1] = strValue[i];
        strValue[dot] = (char)separator;
    }

    if (remove_leading_zeros) {
        if (strValue[0] == '0') {
            strValue[0] = ' ';
            if (strValue[1] == '0')
                strValue[1] = ' ';
        }
    }
}

void si4735_removeUnwantedChar(char *str, int size)
{
    for (int i = 0; str[i] != '\0' && i < size; i++)
        if (str[i] != 0 && str[i] < 32)
            str[i] = ' ';
    str[size - 1] = '\0';
}