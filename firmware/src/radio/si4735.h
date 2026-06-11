/**
 * @file si4735.h
 * @brief SI4735 / SI473X / SI474X Broadcast AM/FM/SW Radio Receiver ? C port
 *
 * @details Ported from the Arduino C++ library by PU2CLR (Ricardo Lima Caratti).
 *          Original: https://github.com/pu2clr/SI4735
 *
 * Changes from the original:
 *  - C++ class replaced by module-level global state + si4735_ prefixed functions
 *  - All hardware access routed through si4735_hal.h (no Arduino Wire/digitalWrite)
 *  - C++ default arguments replaced by wrapper macros
 *  - Function overloads renamed (seekStationProgress / setSSB / getRdsStatus / etc.)
 *  - PROGMEM macros removed (PIC32 has unified address space)
 *  - bool provided by <stdbool.h>
 *
 * Target: PIC32 / MPLAB X / XC32   (also portable to any C99 environment)
 */

#ifndef SI4735_H
#define SI4735_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================================
 * Power-up function selectors
 * ========================================================================= */
#define POWER_UP_FM   0   /**< FM receiver */
#define POWER_UP_AM   1   /**< AM/SW receiver (and SSB when patch applied) */
#define POWER_UP_WB   3   /**< Weather-band receiver */
#define POWER_PATCH  15   /**< Query library ID */

/* =========================================================================
 * I2C bus addresses
 * ========================================================================= */
#define SI473X_ADDR_SEN_LOW   0x11  /**< SEN pin tied LOW  (GND) */
#define SI473X_ADDR_SEN_HIGH  0x63  /**< SEN pin tied HIGH (+3.3 V) */

/* =========================================================================
 * Command bytes
 * ========================================================================= */
#define POWER_UP        0x01
#define GET_REV         0x10
#define POWER_DOWN      0x11
#define SET_PROPERTY    0x12
#define GET_PROPERTY    0x13
#define GET_INT_STATUS  0x14

/* FM commands */
#define FM_TUNE_FREQ    0x20
#define FM_SEEK_START   0x21
#define FM_TUNE_STATUS  0x22
#define FM_RSQ_STATUS   0x23
#define FM_RDS_STATUS   0x24
#define FM_AGC_STATUS   0x27
#define FM_AGC_OVERRIDE 0x28

/* AM commands */
#define AM_TUNE_FREQ    0x40
#define AM_SEEK_START   0x41
#define AM_TUNE_STATUS  0x42
#define AM_RSQ_STATUS   0x43
#define AM_AGC_STATUS   0x47
#define AM_AGC_OVERRIDE 0x48

/* SSB commands (share AM command codes ? patch required) */
#define SSB_TUNE_FREQ    0x40
#define SSB_TUNE_STATUS  0x42
#define SSB_RSQ_STATUS   0x43
#define SSB_AGC_STATUS   0x47
#define SSB_AGC_OVERRIDE 0x48

/* NBFM commands */
#define NBFM_TUNE_FREQ    0x50
#define NBFM_TUNE_STATUS  0x52
#define NBFM_RSQ_STATUS   0x53
#define NBFM_AGC_STATUS   0x57
#define NBFM_AGC_OVERRIDE 0x58

/* GPIO commands */
#define GPIO_CTL 0x80
#define GPIO_SET 0x81

/* =========================================================================
 * Property addresses ? FM
 * ========================================================================= */
#define FM_DEEMPHASIS                       0x1100
#define FM_CHANNEL_FILTER                   0x1102
#define FM_BLEND_STEREO_THRESHOLD           0x1105
#define FM_BLEND_MONO_THRESHOLD             0x1106
#define FM_SEEK_BAND_BOTTOM                 0x1400
#define FM_SEEK_BAND_TOP                    0x1401
#define FM_SEEK_FREQ_SPACING                0x1402
#define FM_SEEK_TUNE_SNR_THRESHOLD          0x1403
#define FM_SEEK_TUNE_RSSI_THRESHOLD         0x1404
#define FM_RDS_INT_SOURCE                   0x1500
#define FM_RDS_INT_FIFO_COUNT               0x1501
#define FM_RDS_CONFIG                       0x1502
#define FM_RDS_CONFIDENCE                   0x1503
#define FM_BLEND_RSSI_STEREO_THRESHOLD      0x1800
#define FM_BLEND_RSSI_MONO_THRESHOLD        0x1801
#define FM_BLEND_SNR_STEREO_THRESHOLD       0x1804
#define FM_BLEND_SNR_MONO_THRESHOLD         0x1805
#define FM_BLEND_MULTIPATH_STEREO_THRESHOLD 0x1808
#define FM_BLEND_MULTIPATH_MONO_THRESHOLD   0x1809
#define FM_NB_DETECT_THRESHOLD              0x1900
#define FM_NB_INTERVAL                      0x1901
#define FM_NB_RATE                          0x1902
#define FM_NB_IIR_FILTER                    0x1903
#define FM_NB_DELAY                         0x1904
#define FM_SOFT_MUTE_MAX_ATTENUATION        0x1302

/* =========================================================================
 * Property addresses ? AM / SW / LW
 * ========================================================================= */
#define AM_DEEMPHASIS                        0x3100
#define AM_CHANNEL_FILTER                    0x3102
#define AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN 0x3103
#define AM_MODE_AFC_SW_PULL_IN_RANGE         0x3104
#define AM_MODE_AFC_SW_LOCK_IN_RANGE         0x3105
#define AM_RSQ_INTERRUPTS                    0x3200
#define AM_RSQ_SNR_HIGH_THRESHOLD            0x3201
#define AM_RSQ_SNR_LOW_THRESHOLD             0x3202
#define AM_RSQ_RSSI_HIGH_THRESHOLD           0x3203
#define AM_RSQ_RSSI_LOW_THRESHOLD            0x3204
#define AM_SOFT_MUTE_RATE                    0x3300
#define AM_SOFT_MUTE_SLOPE                   0x3301
#define AM_SOFT_MUTE_MAX_ATTENUATION         0x3302
#define AM_SOFT_MUTE_SNR_THRESHOLD           0x3303
#define AM_SOFT_MUTE_RELEASE_RATE            0x3304
#define AM_SOFT_MUTE_ATTACK_RATE             0x3305
#define AM_SEEK_BAND_BOTTOM                  0x3400
#define AM_SEEK_BAND_TOP                     0x3401
#define AM_SEEK_FREQ_SPACING                 0x3402
#define AM_SEEK_SNR_THRESHOLD                0x3403
#define AM_SEEK_RSSI_THRESHOLD               0x3404
#define AM_AGC_ATTACK_RATE                   0x3702
#define AM_AGC_RELEASE_RATE                  0x3703
#define AM_FRONTEND_AGC_CONTROL              0x3705
#define AM_NB_DETECT_THRESHOLD               0x3900
#define AM_NB_INTERVAL                       0x3901
#define AM_NB_RATE                           0x3902
#define AM_NB_IIR_FILTER                     0x3903
#define AM_NB_DELAY                          0x3904

/* =========================================================================
 * Property addresses ? SSB (patch required)
 * ========================================================================= */
#define GPO_IEN                       0x0001
#define SSB_BFO                       0x0100
#define SSB_MODE                      0x0101
#define SSB_RSQ_INTERRUPTS            0x3200
#define SSB_RSQ_SNR_HI_THRESHOLD      0x3201
#define SSB_RSQ_SNR_LO_THRESHOLD      0x3202
#define SSB_RSQ_RSSI_HI_THRESHOLD     0x3203
#define SSB_RSQ_RSSI_LO_THRESHOLD     0x3204
#define SSB_SOFT_MUTE_RATE            0x3300
#define SSB_SOFT_MUTE_MAX_ATTENUATION 0x3302
#define SSB_SOFT_MUTE_SNR_THRESHOLD   0x3303
#define SSB_RF_AGC_ATTACK_RATE        0x3700
#define SSB_RF_AGC_RELEASE_RATE       0x3701
#define SSB_IF_AGC_RELEASE_RATE       0x3703
#define SSB_IF_AGC_ATTACK_RATE        0x3702

/* =========================================================================
 * Property addresses ? NBFM
 * ========================================================================= */
#define NBFM_MAX_TUNE_ERROR        0x5108
#define NBFM_RSQ_INT_SOURCE        0x5200
#define NBFM_RSQ_SNR_HI_THRESHOLD  0x5201
#define NBFM_RSQ_SNR_LO_THRESHOLD  0x5202
#define NBFM_RSQ_RSSI_HI_THRESHOLD 0x5203
#define NBFM_RSQ_RSSI_LO_THRESHOLD 0x5204
#define NBFM_VALID_SNR_THRESHOLD   0x5403
#define NBFM_VALID_RSSI_THRESHOLD  0x5404

/* =========================================================================
 * Property addresses ? common audio / digital output
 * ========================================================================= */
#define RX_VOLUME                  0x4000
#define RX_HARD_MUTE               0x4001
#define DIGITAL_OUTPUT_FORMAT      0x0102
#define DIGITAL_OUTPUT_SAMPLE_RATE 0x0104
#define REFCLK_FREQ                0x0201
#define REFCLK_PRESCALE            0x0202

/* =========================================================================
 * Audio output mode flags (OPMODE argument of POWER_UP)
 * ========================================================================= */
#define SI473X_RDS_OUTPUT_ONLY       0x00
#define SI473X_ANALOG_AUDIO          0x05
#define SI473X_DIGITAL_AUDIO1        0x0B
#define SI473X_DIGITAL_AUDIO2        0xB0
#define SI473X_ANALOG_DIGITAL_AUDIO  0xB5

/* =========================================================================
 * Current-mode identifiers (lastMode / currentTune)
 * ========================================================================= */
#define FM_CURRENT_MODE   0
#define AM_CURRENT_MODE   1
#define SSB_CURRENT_MODE  2
#define NBFM_CURRENT_MODE 3

/* =========================================================================
 * Seek direction
 * ========================================================================= */
#define SEEK_UP   1
#define SEEK_DOWN 0

/* =========================================================================
 * SSB sideband selection
 * ========================================================================= */
#define LSB_MODE 1
#define USB_MODE 2

/* =========================================================================
 * Clock source for POWER_UP
 * ========================================================================= */
#define XOSCEN_CRYSTAL 1  /**< Passive crystal on XTAL1/XTAL2 */
#define XOSCEN_RCLK    0  /**< Active clock on RCLK pin */

/* =========================================================================
 * Timing constants
 * ========================================================================= */
#define MAX_DELAY_AFTER_SET_FREQUENCY 30u   /**< ms after tune command */
#define MAX_DELAY_AFTER_POWERUP       10u   /**< ms after power-up command */
#define MIN_DELAY_WAIT_SEND_LOOP      300u  /**< µs per waitToSend loop */
#define MAX_SEEK_TIME                 8000u /**< ms maximum seek time */

/* =========================================================================
 * Misc defaults
 * ========================================================================= */
#define DEFAULT_CURRENT_AVC_AM_MAX_GAIN 36
#define LAST_MODE_NONE                  0xFF  /**< sentinel for lastMode */

/* =========================================================================
 * Data types (unions / structs mirroring SI47XX register layout)
 * ========================================================================= */

typedef union {
    struct {
        uint8_t FUNC    : 4;
        uint8_t XOSCEN  : 1;
        uint8_t PATCH   : 1;
        uint8_t GPO2OEN : 1;
        uint8_t CTSIEN  : 1;
        uint8_t OPMODE;
    } arg;
    uint8_t raw[2];
} si473x_powerup;

typedef union {
    struct {
        uint8_t DUMMY1  : 1;
        uint8_t GPO1OEN : 1;
        uint8_t GPO2OEN : 1;
        uint8_t GPO3OEN : 1;
        uint8_t DUMMY2  : 4;
    } arg;
    uint8_t raw;
} si473x_gpio;

typedef union {
    struct {
        uint8_t STCIEN : 1;
        uint8_t DUMMY1 : 2;
        uint8_t RSQIEN : 1;
        uint8_t DUMMY2 : 2;
        uint8_t ERRIEN : 1;
        uint8_t CTSIEN : 1;
        uint8_t STCREP : 1;
        uint8_t DUMMY3 : 2;
        uint8_t RSQREP : 1;
        uint8_t DUMMY4 : 4;
    } arg;
    uint16_t raw;
} si473x_gpio_ien;

typedef union {
    struct {
        uint8_t FREQL;
        uint8_t FREQH;
    } raw;
    uint16_t value;
} si47x_frequency;

typedef union {
    struct {
        uint8_t ANTCAPL;
        uint8_t ANTCAPH;
    } raw;
    uint16_t value;
} si47x_antenna_capacitor;

typedef union {
    struct {
        uint8_t FAST   : 1;
        uint8_t FREEZE : 1;
        uint8_t DUMMY1 : 4;
        uint8_t USBLSB : 2;
        uint8_t FREQH;
        uint8_t FREQL;
        uint8_t ANTCAPH;
        uint8_t ANTCAPL;
    } arg;
    uint8_t raw[5];
} si47x_set_frequency;

typedef union {
    struct {
        uint8_t RESERVED1 : 2;
        uint8_t WRAP      : 1;
        uint8_t SEEKUP    : 1;
        uint8_t RESERVED2 : 4;
    } arg;
    uint8_t raw;
} si47x_seek;

typedef struct {
    uint8_t ARG2;
    uint8_t ARG3;
    uint8_t ANTCAPH;
    uint8_t ANTCAPL;
} si47x_seek_am_complement;

typedef union {
    struct {
        uint8_t STCINT : 1;
        uint8_t DUMMY1 : 1;
        uint8_t RDSINT : 1;
        uint8_t RSQINT : 1;
        uint8_t DUMMY2 : 2;
        uint8_t ERR    : 1;
        uint8_t CTS    : 1;
    } refined;
    uint8_t raw;
} si47x_status;

typedef union {
    struct {
        uint8_t STCINT    : 1;
        uint8_t DUMMY1    : 1;
        uint8_t RDSINT    : 1;
        uint8_t RSQINT    : 1;
        uint8_t DUMMY2    : 2;
        uint8_t ERR       : 1;
        uint8_t CTS       : 1;
        uint8_t VALID     : 1;
        uint8_t AFCRL     : 1;
        uint8_t DUMMY3    : 5;
        uint8_t BLTF      : 1;
        uint8_t READFREQH;
        uint8_t READFREQL;
        uint8_t RSSI;
        uint8_t SNR;
        uint8_t MULT;
        uint8_t READANTCAP;
    } resp;
    uint8_t raw[8];
} si47x_response_status;

typedef union {
    struct {
        uint8_t STCINT   : 1;
        uint8_t DUMMY1   : 1;
        uint8_t RDSINT   : 1;
        uint8_t RSQINT   : 1;
        uint8_t DUMMY2   : 2;
        uint8_t ERR      : 1;
        uint8_t CTS      : 1;
        uint8_t PN;
        uint8_t FWMAJOR;
        uint8_t FWMINOR;
        uint8_t PATCHH;
        uint8_t PATCHL;
        uint8_t CMPMAJOR;
        uint8_t CMPMINOR;
        uint8_t CHIPREV;
    } resp;
    uint8_t raw[9];
} si47x_firmware_information;

typedef union {
    struct {
        uint8_t STCINT    : 1;
        uint8_t DUMMY1    : 1;
        uint8_t RDSINT    : 1;
        uint8_t RSQINT    : 1;
        uint8_t DUMMY2    : 2;
        uint8_t ERR       : 1;
        uint8_t CTS       : 1;
        uint8_t PN;
        uint8_t FWMAJOR;
        uint8_t FWMINOR;
        uint8_t RESERVED1;
        uint8_t RESERVED2;
        uint8_t CHIPREV;
        uint8_t LIBRARYID;
    } resp;
    uint8_t raw[8];
} si47x_firmware_query_library;

typedef union {
    struct {
        uint8_t INTACK    : 1;
        uint8_t CANCEL    : 1;
        uint8_t RESERVED2 : 6;
    } arg;
    uint8_t raw;
} si47x_tune_status;

typedef union {
    struct {
        uint8_t byteLow;
        uint8_t byteHigh;
    } raw;
    uint16_t value;
} si47x_property;

typedef union {
    struct {
        uint8_t STCINT    : 1;
        uint8_t DUMMY1    : 1;
        uint8_t RDSINT    : 1;
        uint8_t RSQINT    : 1;
        uint8_t DUMMY2    : 2;
        uint8_t ERR       : 1;
        uint8_t CTS       : 1;
        uint8_t RSSIILINT : 1;
        uint8_t RSSIHINT  : 1;
        uint8_t SNRLINT   : 1;
        uint8_t SNRHINT   : 1;
        uint8_t MULTLINT  : 1;
        uint8_t MULTHINT  : 1;
        uint8_t DUMMY3    : 1;
        uint8_t BLENDINT  : 1;
        uint8_t VALID     : 1;
        uint8_t AFCRL     : 1;
        uint8_t DUMMY4    : 1;
        uint8_t SMUTE     : 1;
        uint8_t DUMMY5    : 4;
        uint8_t STBLEND   : 7;
        uint8_t PILOT     : 1;
        uint8_t RSSI;
        uint8_t SNR;
        uint8_t MULT;
        uint8_t FREQOFF;
    } resp;
    uint8_t raw[8];
} si47x_rqs_status;

typedef union {
    struct {
        uint8_t ATTN_BACKUP;
        uint8_t MIN_GAIN_INDEX;
    } field;
    uint16_t word;
} si47x_frontend_agc_control;

typedef union {
    struct {
        uint8_t INTACK     : 1;
        uint8_t MTFIFO     : 1;
        uint8_t STATUSONLY : 1;
        uint8_t dummy      : 5;
    } arg;
    uint8_t raw;
} si47x_rds_command;

typedef union {
    struct {
        uint8_t STCINT        : 1;
        uint8_t DUMMY1        : 1;
        uint8_t RDSINT        : 1;
        uint8_t RSQINT        : 1;
        uint8_t DUMMY2        : 2;
        uint8_t ERR           : 1;
        uint8_t CTS           : 1;
        uint8_t RDSRECV       : 1;
        uint8_t RDSSYNCLOST   : 1;
        uint8_t RDSSYNCFOUND  : 1;
        uint8_t DUMMY3        : 1;
        uint8_t RDSNEWBLOCKA  : 1;
        uint8_t RDSNEWBLOCKB  : 1;
        uint8_t DUMMY4        : 2;
        uint8_t RDSSYNC       : 1;
        uint8_t DUMMY5        : 1;
        uint8_t GRPLOST       : 1;
        uint8_t DUMMY6        : 5;
        uint8_t RDSFIFOUSED;
        uint8_t BLOCKAH;
        uint8_t BLOCKAL;
        uint8_t BLOCKBH;
        uint8_t BLOCKBL;
        uint8_t BLOCKCH;
        uint8_t BLOCKCL;
        uint8_t BLOCKDH;
        uint8_t BLOCKDL;
        uint8_t BLED : 2;
        uint8_t BLEC : 2;
        uint8_t BLEB : 2;
        uint8_t BLEA : 2;
    } resp;
    uint8_t raw[13];
} si47x_rds_status;

typedef union {
    struct {
        uint8_t RDSRECV      : 1;
        uint8_t RDSSYNCLOST  : 1;
        uint8_t RDSSYNCFOUND : 1;
        uint8_t DUMMY1       : 1;
        uint8_t RDSNEWBLOCKA : 1;
        uint8_t RDSNEWBLOCKB : 1;
        uint8_t DUMMY2       : 5;
        uint8_t DUMMY3       : 5;
    } refined;
    uint8_t raw[2];
} si47x_rds_int_source;

typedef union {
    struct {
        uint8_t RDSEN  : 1;
        uint8_t DUMMY1 : 7;
        uint8_t BLETHD : 2;
        uint8_t BLETHC : 2;
        uint8_t BLETHB : 2;
        uint8_t BLETHA : 2;
    } arg;
    uint8_t raw[2];
} si47x_rds_config;

typedef union {
    struct { uint16_t pi; } refined;
    struct {
        uint8_t highValue;
        uint8_t lowValue;
    } raw;
} si47x_rds_blocka;

typedef union {
    struct {
        uint16_t address         : 2;
        uint16_t DI              : 1;
        uint16_t MS              : 1;
        uint16_t TA              : 1;
        uint16_t programType     : 5;
        uint16_t trafficProgramCode : 1;
        uint16_t versionCode     : 1;
        uint16_t groupType       : 4;
    } group0;
    struct {
        uint16_t address         : 4;
        uint16_t textABFlag      : 1;
        uint16_t programType     : 5;
        uint16_t trafficProgramCode : 1;
        uint16_t versionCode     : 1;
        uint16_t groupType       : 4;
    } group2;
    struct {
        uint16_t content         : 4;
        uint16_t textABFlag      : 1;
        uint16_t programType     : 5;
        uint16_t trafficProgramCode : 1;
        uint16_t versionCode     : 1;
        uint16_t groupType       : 4;
    } refined;
    struct {
        uint8_t lowValue;
        uint8_t highValue;
    } raw;
} si47x_rds_blockb;

typedef union {
    struct {
        uint32_t offset       : 5;
        uint32_t offset_sense : 1;
        uint32_t minute       : 6;
        uint32_t hour         : 5;
        uint32_t mjd          : 17;
    } refined;
    uint8_t raw[6];
} si47x_rds_date_time;

typedef union {
    struct {
        uint8_t STCINT : 1;
        uint8_t DUMMY1 : 1;
        uint8_t RDSINT : 1;
        uint8_t RSQINT : 1;
        uint8_t DUMMY2 : 2;
        uint8_t ERR    : 1;
        uint8_t CTS    : 1;
        uint8_t AGCDIS : 1;
        uint8_t DUMMY  : 7;
        uint8_t AGCIDX;
    } refined;
    uint8_t raw[3];
} si47x_agc_status;

typedef union {
    struct {
        uint8_t AGCDIS : 1;
        uint8_t DUMMY  : 7;
        uint8_t AGCIDX;
    } arg;
    uint8_t raw[2];
} si47x_agc_overrride;

typedef union {
    struct {
        uint8_t AMCHFLT : 4;
        uint8_t DUMMY1  : 4;
        uint8_t AMPLFLT : 1;
        uint8_t DUMMY2  : 7;
    } param;
    uint8_t raw[2];
} si47x_bandwidth_config;

typedef union {
    struct {
        uint8_t AUDIOBW    : 4;
        uint8_t SBCUTFLT   : 4;
        uint8_t AVC_DIVIDER: 4;
        uint8_t AVCEN      : 1;
        uint8_t SMUTESEL   : 1;
        uint8_t DUMMY1     : 1;
        uint8_t DSP_AFCDIS : 1;
    } param;
    uint8_t raw[2];
} si47x_ssb_mode;

typedef union {
    struct {
        uint8_t OSIZE  : 2;
        uint8_t OMONO  : 1;
        uint8_t OMODE  : 4;
        uint8_t OFALL  : 1;
        uint8_t dummy  : 8;
    } refined;
    uint16_t raw;
} si4735_digital_output_format;

typedef union {
    struct {
        uint8_t reserved[8];
        uint8_t status[8];
        uint8_t patch_id[14];
        uint16_t patch_size;
    } refined;
    uint8_t raw[32];
} si4735_eeprom_patch_header;

/* =========================================================================
 * Public API
 * ========================================================================= */

/* --- Initialisation & power ----------------------------------------------- */
void si4735_init(void);
void si4735_reset(void);
void si4735_waitToSend(void);

void si4735_setup(uint8_t resetPin, uint8_t defaultFunction);
void si4735_detailed_setup(uint8_t resetPin, uint8_t ctsIntEnable,
                           uint8_t defaultFunction, uint8_t audioMode,
                           uint8_t clockType, uint8_t gpo2Enable);

/** Convenience wrappers for si4735_detailed_setup */
#define si4735_detailed_setup_default(rPin, ctsIE, func, aMode) \
    si4735_detailed_setup((rPin), (ctsIE), (func), (aMode), XOSCEN_CRYSTAL, 0)

#define si4735_setup_analog(rPin, func) \
    si4735_detailed_setup((rPin), 0, (func), SI473X_ANALOG_AUDIO, XOSCEN_CRYSTAL, 0)

void si4735_setPowerUp(uint8_t CTSIEN, uint8_t GPO2OEN, uint8_t PATCH,
                       uint8_t XOSCEN, uint8_t FUNC, uint8_t OPMODE);
void si4735_radioPowerUp(void);
void si4735_analogPowerUp(void);  /* legacy alias for radioPowerUp */
void si4735_powerDown(void);

/* --- GPIO ----------------------------------------------------------------- */
void si4735_setGpioCtl(uint8_t GPO1OEN, uint8_t GPO2OEN, uint8_t GPO3OEN);
void si4735_setGpio(uint8_t GPO1LEVEL, uint8_t GPO2LEVEL, uint8_t GPO3LEVEL);
void si4735_setGpioIen(uint8_t STCIEN, uint8_t RSQIEN, uint8_t ERRIEN,
                       uint8_t CTSIEN, uint8_t STCREP, uint8_t RSQREP);

/* --- Reference clock ------------------------------------------------------ */
void si4735_setRefClock(uint16_t refclk);
void si4735_setRefClockPrescaler(uint16_t prescale, uint8_t rclk_sel);
#define si4735_setRefClockPrescaler_default(prescale) \
    si4735_setRefClockPrescaler((prescale), 0)

/* --- Frequency & band ----------------------------------------------------- */
void     si4735_setFrequency(uint16_t freq);
uint16_t si4735_getFrequency(void);
uint16_t si4735_getCurrentFrequency(void);

void si4735_frequencyUp(void);
void si4735_frequencyDown(void);

/** Aliases (same as frequencyUp/Down) */
#define si4735_setFrequencyUp()   si4735_frequencyUp()
#define si4735_setFrequencyDown() si4735_frequencyDown()

void si4735_setFrequencyStep(uint16_t step);
void si4735_setTuneFrequencyAntennaCapacitor(uint16_t capacitor);
void si4735_setTuneFrequencyFast(uint8_t FAST);
void si4735_setTuneFrequencyFreze(uint8_t FREEZE);
uint8_t si4735_getTuneFrequecyFast(void);
uint8_t si4735_getTuneFrequecyFreeze(void);

/* --- Mode selection ------------------------------------------------------- */
void si4735_setAMNoArgs(void);
void si4735_setFMNoArgs(void);
void si4735_setAM(uint16_t fromFreq, uint16_t toFreq,
                  uint16_t initialFreq, uint16_t step);
void si4735_setFM(uint16_t fromFreq, uint16_t toFreq,
                  uint16_t initialFreq, uint16_t step);

bool si4735_isCurrentTuneFM(void);
bool si4735_isCurrentTuneAM(void);
bool si4735_isCurrentTuneSSB(void);

/* --- Status --------------------------------------------------------------- */
void si4735_getStatus(uint8_t INTACK, uint8_t CANCEL);
void si4735_getStatusNoArgs(void);   /* calls getStatus(0,1) */

bool    si4735_getSignalQualityInterrupt(void);
bool    si4735_getRadioDataSystemInterrupt(void);
bool    si4735_getTuneCompleteTriggered(void);
bool    si4735_getStatusError(void);
bool    si4735_getStatusCTS(void);
bool    si4735_getACFIndicator(void);
bool    si4735_getBandLimit(void);
bool    si4735_getStatusValid(void);
uint8_t si4735_getReceivedSignalStrengthIndicator(void);
uint8_t si4735_getStatusSNR(void);
uint8_t si4735_getStatusMULT(void);
uint16_t si4735_getAntennaTuningCapacitor(void);

/* --- RSQ ------------------------------------------------------------------ */
void    si4735_getCurrentReceivedSignalQuality(uint8_t INTACK);
void    si4735_getCurrentReceivedSignalQualityNoArgs(void);
uint8_t si4735_getCurrentRSSI(void);
uint8_t si4735_getCurrentSNR(void);
bool    si4735_getCurrentRssiDetectLow(void);
bool    si4735_getCurrentRssiDetectHigh(void);
bool    si4735_getCurrentSnrDetectLow(void);
bool    si4735_getCurrentSnrDetectHigh(void);
bool    si4735_getCurrentValidChannel(void);
bool    si4735_getCurrentAfcRailIndicator(void);
bool    si4735_getCurrentSoftMuteIndicator(void);
uint8_t si4735_getCurrentStereoBlend(void);
bool    si4735_getCurrentPilot(void);
uint8_t si4735_getCurrentMultipath(void);
uint8_t si4735_getCurrentSignedFrequencyOffset(void);
bool    si4735_getCurrentMultipathDetectLow(void);
bool    si4735_getCurrentMultipathDetectHigh(void);
bool    si4735_getCurrentBlendDetectInterrupt(void);

/* --- AGC ------------------------------------------------------------------ */
void    si4735_getAutomaticGainControl(void);
void    si4735_getSsbAgcStatus(void);   /**< SSB-specific AGC status query */
void    si4735_setAutomaticGainControl(uint8_t AGCDIS, uint8_t AGCIDX);
#define si4735_setAGC(dis, idx) si4735_setAutomaticGainControl((dis), (idx))
void    si4735_setAvcAmMaxGain(uint8_t gain);
uint8_t si4735_getCurrentAvcAmMaxGain(void);
bool    si4735_isAgcEnabled(void);
uint8_t si4735_getAgcGainIndex(void);
void    si4735_setSsbAgcOverrite(uint8_t SSBAGCDIS, uint8_t SSBAGCNDX,
                                  uint8_t reserved);
#define si4735_setSsbAgcOverrite_default(dis, idx) \
    si4735_setSsbAgcOverrite((dis), (idx), 0)

/* --- AM filter / soft-mute ------------------------------------------------ */
void si4735_setBandwidth(uint8_t AMCHFLT, uint8_t AMPLFLT);
void si4735_setAmSoftMuteMaxAttenuation(uint8_t smattn);
#define si4735_setAmSoftMuteMaxAttenuation_default() \
    si4735_setAmSoftMuteMaxAttenuation(0)
void si4735_setAMSoftMuteSlop(uint8_t parameter);
void si4735_setAMSoftMuteRate(uint8_t parameter);
void si4735_setAMSoftMuteSnrThreshold(uint8_t parameter);
void si4735_setAMSoftMuteReleaseRate(uint8_t parameter);
void si4735_setAMSoftMuteAttackRate(uint16_t parameter);
void si4735_setAmSoftMuteMaxAttenuation(uint8_t smattn);
void si4735_setAMDeEmphasis(uint8_t parameter);
void si4735_setAMFrontEndAgcControl(uint8_t MIN_GAIN_INDEX, uint8_t ATTN_BACKUP);
void si4735_setAmAgcAttackRate(uint16_t parameter);
void si4735_setAmAgcReleaseRate(uint16_t parameter);
void si4735_setAmNoiseBlank(uint16_t nb_rate, uint16_t nb_interval,
                             uint16_t nb_irr_filter);
#define si4735_setAmNoiseBlank_default() si4735_setAmNoiseBlank(64, 55, 300)
void si4735_setAmDelayNB(uint16_t value);

/* --- FM filter / soft-mute ------------------------------------------------ */
void si4735_setFMDeEmphasis(uint8_t parameter);
void si4735_setFmSoftMuteMaxAttenuation(uint8_t smattn);
#define si4735_setFmSoftMuteMaxAttenuation_default() \
    si4735_setFmSoftMuteMaxAttenuation(0)
void si4735_setFmBandwidth(uint8_t filter_value);
#define si4735_setFmBandwidth_default() si4735_setFmBandwidth(0)
void si4735_setFmNoiseBlankThreshold(uint16_t parameter);
void si4735_setFmNoiseBlankInterval(uint16_t parameter);
void si4735_setFmNoiseBlankRate(uint16_t parameter);
void si4735_setFmNoiseBlankDelay(uint16_t parameter);
void si4735_setFmNoiseBlank_IIR_Filter(uint16_t parameter);
void si4735_setFmNoiseBlank(uint16_t nb_rate, uint16_t nb_interval,
                             uint16_t nb_irr_filter);
#define si4735_setFmNoiseBlank_default() si4735_setFmNoiseBlank(64, 55, 300)

/* --- FM stereo blend ------------------------------------------------------ */
void si4735_setFmBlendStereoThreshold(uint8_t parameter);
void si4735_setFmBlendMonoThreshold(uint8_t parameter);
void si4735_setFmBlendRssiStereoThreshold(uint8_t parameter);
void si4735_setFmBLendRssiMonoThreshold(uint8_t parameter);
void si4735_setFmBlendSnrStereoThreshold(uint8_t parameter);
void si4735_setFmBLendSnrMonoThreshold(uint8_t parameter);
void si4735_setFmBlendMultiPathStereoThreshold(uint8_t parameter);
void si4735_setFmBlendMultiPathMonoThreshold(uint8_t parameter);
void si4735_setFmStereoOn(void);
void si4735_setFmStereoOff(void);

/* --- Seek ----------------------------------------------------------------- */
void si4735_seekStation(uint8_t SEEKUP, uint8_t WRAP);
void si4735_seekNextStation(void);
void si4735_seekPreviousStation(void);

/**
 * Seek with progress callback.
 * @param showFunc  called each iteration with current frequency; may be NULL.
 * @param up_down   SEEK_UP or SEEK_DOWN.
 */
void si4735_seekStationProgress(void (*showFunc)(uint16_t f), uint8_t up_down);

/**
 * Seek with progress callback AND user-supplied stop predicate.
 * @param showFunc    called each iteration; may be NULL.
 * @param stopSeeking called each iteration; return true to abort; may be NULL.
 * @param up_down     SEEK_UP or SEEK_DOWN.
 */
void si4735_seekStationProgressWithStop(void (*showFunc)(uint16_t f),
                                         bool (*stopSeeking)(void),
                                         uint8_t up_down);

/** Convenience wrappers */
#define si4735_seekStationUp()   si4735_seekStationProgress(NULL, SEEK_UP)
#define si4735_seekStationDown() si4735_seekStationProgress(NULL, SEEK_DOWN)

void si4735_setSeekAmLimits(uint16_t bottom, uint16_t top);
void si4735_setSeekAmSpacing(uint16_t spacing);
void si4735_setSeekAmRssiThreshold(uint16_t value);
void si4735_setSeekAmSNRThreshold(uint16_t value);

void si4735_setSeekFmLimits(uint16_t bottom, uint16_t top);
void si4735_setSeekFmSpacing(uint16_t spacing);
void si4735_setSeekFmRssiThreshold(uint16_t value);
void si4735_setSeekFmSNRThreshold(uint16_t value);

void si4735_setMaxSeekTime(uint32_t time_in_ms);

/* --- Volume & audio ------------------------------------------------------- */
void    si4735_setVolume(uint8_t volume);
uint8_t si4735_getVolume(void);
uint8_t si4735_getCurrentVolume(void);
void    si4735_volumeUp(void);
void    si4735_volumeDown(void);
#define si4735_setVolumeUp()   si4735_volumeUp()
#define si4735_setVolumeDown() si4735_volumeDown()
void    si4735_setAudioMute(bool off);
void    si4735_setAudioMode(uint8_t audioMode);
void    si4735_digitalOutputFormat(uint8_t OSIZE, uint8_t OMONO,
                                    uint8_t OMODE, uint8_t OFALL);
void    si4735_digitalOutputSampleRate(uint16_t DOSR);

/* --- SSB AGC rates -------------------------------------------------------- */
void si4735_setSsbSoftMuteMaxAttenuation(uint8_t smattn);
#define si4735_setSsbSoftMuteMaxAttenuation_default() \
    si4735_setSsbSoftMuteMaxAttenuation(0)
void si4735_setSsbIfAgcReleaseRate(uint8_t param);
#define si4735_setSsbIfAgcReleaseRate_default() si4735_setSsbIfAgcReleaseRate(140)
void si4735_setSsbIfAgcAttackRate(uint8_t param);
#define si4735_setSsbIfAgcAttackRate_default() si4735_setSsbIfAgcAttackRate(4)
void si4735_setSsbAgcAttackRate(uint16_t parameter);
void si4735_setSsbAgcReleaseRate(uint16_t parameter);

/* --- AVC ------------------------------------------------------------------ */
void si4735_setAvcAmMaxGainMax(void);     /* 90 dB */
void si4735_setAvcAmMaxGainMin(void);     /* 12 dB */
void si4735_setAvcAmMaxGainDefault(void); /* DEFAULT_CURRENT_AVC_AM_MAX_GAIN */
/** Shorthand: sets AVC to 90 dB (same as setAvcAmMaxGainMax) */
#define si4735_setAvcAmMaxGainHigh() si4735_setAvcAmMaxGainMax()

/* --- Hardware mute -------------------------------------------------------- */
void si4735_setAudioMuteMcuPin(int8_t pin);
void si4735_setHardwareAudioMute(bool on);

/* --- I2C speed helpers ---------------------------------------------------- */
void si4735_setI2CLowSpeedMode(void);   /*  10 kHz */
void si4735_setI2CStandardMode(void);   /* 100 kHz */
void si4735_setI2CFastMode(void);       /* 400 kHz */
void si4735_setI2CFastModeCustom(uint32_t hz);

/* --- I2C address ---------------------------------------------------------- */
void    si4735_setDeviceI2CAddress(uint8_t senPin);
int16_t si4735_getDeviceI2CAddress(uint8_t resetPin);
void    si4735_setDeviceOtherI2CAddress(uint8_t i2cAddr);

/* --- Firmware ------------------------------------------------------------- */
void    si4735_getFirmware(void);
uint8_t si4735_getFirmwarePN(void);
uint8_t si4735_getFirmwareFWMAJOR(void);
uint8_t si4735_getFirmwareFWMINOR(void);
uint8_t si4735_getFirmwarePATCHH(void);
uint8_t si4735_getFirmwarePATCHL(void);
uint8_t si4735_getFirmwareCMPMAJOR(void);
uint8_t si4735_getFirmwareCMPMINOR(void);
uint8_t si4735_getFirmwareCHIPREV(void);

/* --- Generic property / command ------------------------------------------- */
void    si4735_sendProperty(uint16_t propertyNumber, uint16_t parameter);
void    si4735_setProperty(uint16_t propertyNumber, uint16_t param);
int32_t si4735_getProperty(uint16_t propertyValue);
void    si4735_sendCommand(uint8_t cmd, int parameter_size,
                            const uint8_t *parameter);
void    si4735_getCommandResponse(int num_of_bytes, uint8_t *response);
si47x_status si4735_getStatusResponse(void);

/* --- Timing delays -------------------------------------------------------- */
void si4735_setMaxDelayPowerUp(uint16_t ms);
void si4735_setMaxDelaySetFrequency(uint16_t ms);

/* --- RDS ------------------------------------------------------------------ */
void si4735_RdsInit(void);
void si4735_setRdsConfig(uint8_t RDSEN, uint8_t BLETHA, uint8_t BLETHB,
                          uint8_t BLETHC, uint8_t BLETHD);
void si4735_setRdsIntSource(uint8_t RDSRECV, uint8_t RDSSYNCLOST,
                             uint8_t RDSSYNCFOUND, uint8_t RDSNEWBLOCKA,
                             uint8_t RDSNEWBLOCKB);
void si4735_getRdsStatus(uint8_t INTACK, uint8_t MTFIFO, uint8_t STATUSONLY);
void si4735_getRdsStatusSimple(void);   /* calls getRdsStatus(0,0,0) */
void si4735_rdsBeginQuery(void);        /* alias for getRdsStatusSimple */
void si4735_rdsClearFifo(void);
void si4735_rdsClearInterrupt(void);
void si4735_clearRdsBuffer(void);       /* alias for RdsInit */

void si4735_clearRdsBuffer2A(void);
void si4735_clearRdsProgramInformation(void);
void si4735_clearRdsBuffer2B(void);
void si4735_clearRdsStationInformation(void);
void si4735_clearRdsBuffer0A(void);
void si4735_clearRdsStationName(void);

bool    si4735_getRdsReceived(void);
bool    si4735_getRdsSyncLost(void);
bool    si4735_getRdsSyncFound(void);
bool    si4735_getRdsNewBlockA(void);
bool    si4735_getRdsNewBlockB(void);
bool    si4735_getRdsSync(void);
bool    si4735_getGroupLost(void);
uint8_t si4735_getNumRdsFifoUsed(void);
void    si4735_setFifoCount(uint16_t value);
bool    si4735_getEndIndicatorGroupA(void);
void    si4735_resetEndIndicatorGroupA(void);
bool    si4735_getEndIndicatorGroupB(void);
void    si4735_resetEndIndicatorGroupB(void);

uint16_t si4735_getRdsPI(void);
uint8_t  si4735_getRdsGroupType(void);
uint8_t  si4735_getRdsFlagAB(void);
uint8_t  si4735_getRdsVersionCode(void);
uint8_t  si4735_getRdsProgramType(void);
uint8_t  si4735_getRdsTextSegmentAddress(void);

char *si4735_getRdsText(void);
char *si4735_getRdsText0A(void);
char *si4735_getRdsText2A(void);
char *si4735_getRdsText2B(void);

/** Convenience aliases */
#define si4735_getRdsStationName()      si4735_getRdsText0A()
#define si4735_getRdsProgramInformation() si4735_getRdsText2A()
#define si4735_getRdsStationInformation() si4735_getRdsText2B()

bool si4735_getRdsAllData(char **stationName, char **stationInformation,
                           char **programInformation, char **utcTime);

void   si4735_mjdConverter(uint32_t mjd, uint32_t *year,
                            uint32_t *month, uint32_t *day);
char  *si4735_getRdsTime(void);
char  *si4735_getRdsDateTime(void);
bool   si4735_getRdsDateTimeParts(uint16_t *year, uint16_t *month,
                                   uint16_t *day, uint16_t *hour,
                                   uint16_t *minute);

void si4735_getNext2Block(char *c);
void si4735_getNext4Block(char *c);

/* --- SSB (patch required) ------------------------------------------------- */
void si4735_setSSBBfo(int offset);
void si4735_setSSBConfig(uint8_t AUDIOBW, uint8_t SBCUTFLT,
                          uint8_t AVC_DIVIDER, uint8_t AVCEN,
                          uint8_t SMUTESEL, uint8_t DSP_AFCDIS);

/**
 * Switch to SSB mode only (no band limits).
 * @param usblsb  LSB_MODE (1) or USB_MODE (2)
 */
void si4735_setSSBMode(uint8_t usblsb);

/**
 * Switch to SSB mode and set band limits.
 * @param usblsb  LSB_MODE (1) or USB_MODE (2)
 */
void si4735_setSSB(uint16_t fromFreq, uint16_t toFreq,
                   uint16_t initialFreq, uint16_t step, uint8_t usblsb);

void si4735_setSSBAudioBandwidth(uint8_t AUDIOBW);
void si4735_setSSBAutomaticVolumeControl(uint8_t AVCEN);
void si4735_setSSBSidebandCutoffFilter(uint8_t SBCUTFLT);
#define si4735_setSBBSidebandCutoffFilter(x) si4735_setSSBSidebandCutoffFilter(x)
void si4735_setSSBAvcDivider(uint8_t AVC_DIVIDER);
void si4735_setSSBDspAfc(uint8_t DSP_AFCDIS);
void si4735_setSSBSoftMute(uint8_t SMUTESEL);

/* --- SSB / NBFM patch support --------------------------------------------- */
si47x_firmware_query_library si4735_queryLibraryId(void);
void si4735_patchPowerUp(void);
void si4735_ssbPowerUp(void);
bool si4735_downloadPatch(const uint8_t *ssb_patch_content,
                           uint16_t ssb_patch_content_size);
bool si4735_downloadCompressedPatch(const uint8_t *ssb_patch_content,
                                     uint16_t ssb_patch_content_size,
                                     const uint16_t *cmd_0x15,
                                     int16_t cmd_0x15_size);
void si4735_loadPatch(const uint8_t *ssb_patch_content,
                       uint16_t ssb_patch_content_size,
                       uint8_t ssb_audiobw);
#define si4735_loadPatch_default(content, size) \
    si4735_loadPatch((content), (size), 1)
void si4735_loadCompressedPatch(const uint8_t *ssb_patch_content,
                                 uint16_t ssb_patch_content_size,
                                 const uint16_t *cmd_0x15,
                                 int16_t cmd_0x15_size,
                                 uint8_t ssb_audiobw);
#define si4735_loadCompressedPatch_default(content, size, c15, c15sz) \
    si4735_loadCompressedPatch((content), (size), (c15), (c15sz), 1)
si4735_eeprom_patch_header si4735_downloadPatchFromEeprom(int eeprom_i2c_address);

/* --- NBFM ----------------------------------------------------------------- */
void si4735_setNBFM(void);
void si4735_setNBFMBand(uint16_t fromFreq, uint16_t toFreq,
                         uint16_t initialFreq, uint16_t step);
void si4735_patchPowerUpNBFM(void);
void si4735_loadPatchNBFM(const uint8_t *patch_content,
                           uint16_t patch_content_size);
void si4735_setFrequencyNBFM(uint16_t freq);

/* --- Utility -------------------------------------------------------------- */
void si4735_convertToChar(uint16_t value, char *strValue, uint8_t len,
                           uint8_t dot, uint8_t separator,
                           bool remove_leading_zeros);
#define si4735_convertToChar_default(val, str, len, dot, sep) \
    si4735_convertToChar((val), (str), (len), (dot), (sep), true)
void si4735_removeUnwantedChar(char *str, int size);

#ifdef __cplusplus
}
#endif

#endif /* SI4735_H */