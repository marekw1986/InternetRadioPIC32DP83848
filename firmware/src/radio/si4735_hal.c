/**
 * @file si4735_hal.c
 * @brief Platform Abstraction Layer ? PIC32MX / legacy Microchip plib.h
 *
 * @details I2C is implemented with the legacy Microchip Peripheral Library
 * (plib.h: I2CStart, I2CSendByte, I2CGetByte, etc.) on channel I2C1.
 * I2C1 is assumed to already be configured and enabled by MHC-generated
 * code, so this file only performs transactions ? no module init/clock
 * setup is duplicated here. GPIO and timing sections below still contain
 * TODOs ? fill those in with your project's reset-pin implementation.
 */

#include "si4735_hal.h"

/* -------------------------------------------------------------------------
 * Internal receive buffer
 * The SI4735 sends at most 13 bytes in a single response (RDS status).
 * 16 bytes gives comfortable headroom.
 * ------------------------------------------------------------------------- */
#define HAL_RX_BUF_SIZE 16

static uint8_t  _rx_buf[HAL_RX_BUF_SIZE];
static uint8_t  _rx_count  = 0;
static uint8_t  _rx_index  = 0;
static uint8_t  _i2c_addr  = 0;

/* =========================================================================
 * I2C ? legacy Microchip Peripheral Library (plib.h) implementation
 *
 * Uses I2C1 (this project's bug-free channel ? I2C1A/the FIXTRIS pin-glitch
 * workaround was only needed on a different, now-retired I2C3 setup and
 * has been removed). The module itself is assumed to already be
 * configured and enabled by MHC-generated init code, so this file only
 * performs transactions ? it does not call I2CConfigure/I2CEnable.
 * ========================================================================= */

#include "peripheral/coretimer/plib_coretimer.h"
//#include "i2c.h"
#include "plib.h"

static uint8_t _tx_buf[16];
static uint8_t _tx_count = 0;

void hal_i2c_begin_transmission(uint8_t address)
{
    _i2c_addr = address;
    _tx_count = 0;
}

uint8_t hal_i2c_write(uint8_t data)
{
    if (_tx_count < (uint8_t)sizeof(_tx_buf)) {
        _tx_buf[_tx_count++] = data;
        return 1;
    }
    return 0;
}

uint8_t hal_i2c_end_transmission(void)
{
    uint8_t i;

    I2CStart(I2C1);
    while (!(I2CGetStatus(I2C1) & I2C_START)) { /* spin */ }

    I2CSendByte(I2C1, (uint8_t)((_i2c_addr << 1) | 0)); /* address + W */
    while (!I2CTransmissionHasCompleted(I2C1)) { /* spin */ }

    if (!I2CByteWasAcknowledged(I2C1)) {
        I2CStop(I2C1);
        return 1; /* NACK on address */
    }

    for (i = 0; i < _tx_count; i++) {
        I2CSendByte(I2C1, _tx_buf[i]);
        while (!I2CTransmissionHasCompleted(I2C1)) { /* spin */ }
        if (!I2CByteWasAcknowledged(I2C1)) {
            I2CStop(I2C1);
            return 1; /* NACK on data byte */
        }
    }

    I2CStop(I2C1);
    return 0;
}

uint8_t hal_i2c_request_from(uint8_t address, uint8_t count)
{
    uint8_t i;

    if (count > HAL_RX_BUF_SIZE) count = HAL_RX_BUF_SIZE;
    _rx_index = 0;
    _rx_count = 0;

    I2CStart(I2C1);
    while (!(I2CGetStatus(I2C1) & I2C_START)) { /* spin */ }

    I2CSendByte(I2C1, (uint8_t)((address << 1) | 1)); /* address + R */
    while (!I2CTransmissionHasCompleted(I2C1)) { /* spin */ }

    if (!I2CByteWasAcknowledged(I2C1)) {
        I2CStop(I2C1);
        return 0; /* NACK on address ? device not responding */
    }

    for (i = 0; i < count; i++) {
        if (I2CReceiverEnable(I2C1, TRUE) == I2C_RECEIVE_OVERFLOW) {
            break;
        }
        while (!I2CReceivedDataIsAvailable(I2C1)) { /* spin */ }
        _rx_buf[i] = I2CGetByte(I2C1);

        /* ACK every byte except the last, per I2C protocol */
        I2CAcknowledgeByte(I2C1, (i < (uint8_t)(count - 1)) ? TRUE : FALSE);
        while (!I2CAcknowledgeHasCompleted(I2C1)) { /* spin */ }

        _rx_count++;
    }

    I2CStop(I2C1);
    return _rx_count;
}

uint8_t hal_i2c_read(void)
{
    if (_rx_index < _rx_count) return _rx_buf[_rx_index++];
    return 0xFF;
}

void hal_i2c_set_clock(uint32_t hz)
{
    /* I2C1 is configured once by MHC-generated init code at the project's
     * chosen baud rate; this driver does not re-touch the peripheral's
     * clock setup. If you need to change speed at runtime, call
     * I2CSetFrequency(I2C1, <your PBCLK>, hz) here yourself. */
    (void)hz;
}


/* =========================================================================
 * GPIO
 * ========================================================================= */

void hal_gpio_reset_init(void)
{
    /* TODO: configure your RESET pin as a digital output, drive HIGH.
     *
     * Example PIC32MX (RB5):
     *   TRISBbits.TRISB5 = 0;   // output
     *   LATBbits.LATB5   = 1;   // high = device released
     */
}

void hal_gpio_reset_set(bool high)
{
    /* TODO: drive your RESET pin.
     *
     * Example PIC32MX (RB5):
     *   LATBbits.LATB5 = high ? 1 : 0;
     */
    (void)high;
}

static int8_t _mute_pin = -1;

void hal_gpio_mute_init(int8_t pin)
{
    _mute_pin = pin;
    if (pin < 0) return;

    /* TODO: configure the mute pin as digital output, start unmuted.
     *
     * Example ? store pin as port/bit packed value and use a lookup table,
     * or simply hard-code the pin if you only ever use one mute output.
     */
}

void hal_gpio_mute_set(bool mute)
{
    if (_mute_pin < 0) return;

    /* TODO: drive the mute pin.
     *
     * Convention used in the driver: true = muted.
     * Adjust polarity here if your hardware is active-low.
     */
    (void)mute;
}

/* =========================================================================
 * Timing
 *
 * Implemented with CORETIMER_DelayUs(), already available in this
 * project's existing plib.h-based code, so no new dependency is needed.
 * ========================================================================= */

void hal_delay_ms(uint32_t ms)
{
    /* CORETIMER_DelayUs takes a 32-bit microsecond count; for delays longer
     * than ~4 seconds this would overflow, so loop in 1 ms chunks. This is
     * never an issue for the SI4735 driver (max single delay is 8 s seek
     * timeout, handled by the driver's own elapsed-time loop, not a single
     * blocking call here). */
    while (ms > 0) {
        CORETIMER_DelayUs(1000);
        ms--;
    }
}

void hal_delay_us(uint32_t us)
{
    CORETIMER_DelayUs(us);
}