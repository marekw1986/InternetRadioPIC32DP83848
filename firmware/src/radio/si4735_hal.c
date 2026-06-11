/**
 * @file si4735_hal.c
 * @brief Platform Abstraction Layer ? PIC32 / MPLAB X / XC32 stub
 *
 * @details Replace every TODO section with your project's actual peripheral
 * calls.  The rest of the SI4735 driver (si4735.c) never touches hardware
 * directly; it only calls the functions defined here.
 *
 * Common PIC32 I2C peripherals used in MPLAB Harmony v3 projects:
 *   - SERCOM I2C (SAME/SAMD)  ?  SERCOM_I2C_WriteRead() etc.
 *   - PIC32MX/MZ bare-metal   ?  I2CxTRN, I2CxRCV registers
 *   - Harmony v3 PLIB         ?  I2C1_Write(), I2C1_Read() etc.
 *
 * This stub shows the bare-metal PIC32MX/MZ register approach.
 * Swap the bodies for whatever driver stack your project already uses.
 */

#include "si4735_hal.h"

/* --------------------------------------------------------------------------
 * TODO: include your PIC32 device header and any peripheral drivers here.
 *
 * Examples:
 *   #include <xc.h>
 *   #include "peripheral/i2c/plib_i2c1.h"
 *   #include "system/time/sys_time.h"
 * -------------------------------------------------------------------------- */

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
 * I2C helpers ? replace with your PIC32 I2C PLIB or bare-metal calls
 * ========================================================================= */

/* --- Internal bare-metal helpers (PIC32MX example) -----------------------
 *
 * If you are using Harmony v3 PLIB, delete these helpers and replace the
 * bodies of the public hal_i2c_* functions below with PLIB calls instead.
 *
 * #define I2C_BUS   I2C1   -- change to whichever peripheral you wired to
 * ------------------------------------------------------------------------- */

/* TODO: uncomment and adapt one of the following blocks for your MCU       */

#if 0  /* ---- Harmony v3 PLIB example (I2C1) ------------------------------ */
#include "peripheral/i2c/plib_i2c1.h"

static uint8_t _tx_buf[16];
static uint8_t _tx_count = 0;

void hal_i2c_begin_transmission(uint8_t address)
{
    _i2c_addr  = address;
    _tx_count  = 0;
}

uint8_t hal_i2c_write(uint8_t data)
{
    if (_tx_count < sizeof(_tx_buf)) {
        _tx_buf[_tx_count++] = data;
        return 1;
    }
    return 0;
}

uint8_t hal_i2c_end_transmission(void)
{
    bool ok = I2C1_Write(_i2c_addr, _tx_buf, _tx_count);
    /* Blocking wait */
    while (I2C1_IsBusy()) { /* spin */ }
    return ok ? 0 : 1;
}

uint8_t hal_i2c_request_from(uint8_t address, uint8_t count)
{
    _rx_index = 0;
    _rx_count = 0;
    if (count > HAL_RX_BUF_SIZE) count = HAL_RX_BUF_SIZE;
    bool ok = I2C1_Read(address, _rx_buf, count);
    while (I2C1_IsBusy()) { /* spin */ }
    if (ok) _rx_count = count;
    return _rx_count;
}

uint8_t hal_i2c_read(void)
{
    if (_rx_index < _rx_count) return _rx_buf[_rx_index++];
    return 0xFF;
}

void hal_i2c_set_clock(uint32_t hz)
{
    /* Harmony v3 requires re-initialisation to change baud.
     * Usually not needed at runtime ? leave empty or re-init here. */
    (void)hz;
}

#endif /* Harmony v3 -------------------------------------------------------- */


#if 1  /* ---- Bare-metal PIC32MX/MZ stub ---------------------------------- *
        * Replace register names / macros with the ones from your device header.
        * This skeleton compiles but does nothing ? fill in real register ops. */

/* TODO: set this to I2C1, I2C2, I2C3 ? matching your schematic */
#define SI4735_I2C_MODULE   I2C1

static uint8_t _tx_buf[16];
static uint8_t _tx_count = 0;

/* Low-level helpers ? implement with your MCU's register map */
static void _i2c_start(void)
{
    /* TODO: assert START condition
     * Example PIC32MX:
     *   I2C1CONbits.SEN = 1;
     *   while (I2C1CONbits.SEN) {}
     */
}

static void _i2c_stop(void)
{
    /* TODO: assert STOP condition
     * Example PIC32MX:
     *   I2C1CONbits.PEN = 1;
     *   while (I2C1CONbits.PEN) {}
     */
}

/* Returns 0 = ACK received, 1 = NACK */
static uint8_t _i2c_write_byte(uint8_t byte)
{
    /* TODO: load byte, wait for TX complete, check ACK
     * Example PIC32MX:
     *   I2C1TRN = byte;
     *   while (I2C1STATbits.TRSTAT) {}
     *   return I2C1STATbits.ACKSTAT;
     */
    (void)byte;
    return 0;
}

static uint8_t _i2c_read_byte(bool ack)
{
    /* TODO: enable receiver, wait, send ACK/NACK
     * Example PIC32MX:
     *   I2C1CONbits.RCEN = 1;
     *   while (!I2C1STATbits.RBF) {}
     *   uint8_t d = I2C1RCV;
     *   I2C1CONbits.ACKDT = ack ? 0 : 1;
     *   I2C1CONbits.ACKEN = 1;
     *   while (I2C1CONbits.ACKEN) {}
     *   return d;
     */
    (void)ack;
    return 0xFF;
}

/* --- Public HAL functions ------------------------------------------------ */

void hal_i2c_begin_transmission(uint8_t address)
{
    _i2c_addr  = address;
    _tx_count  = 0;
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
    _i2c_start();

    /* Send address byte with W bit (bit 0 = 0) */
    uint8_t nack = _i2c_write_byte((_i2c_addr << 1) | 0x00);
    if (nack) { _i2c_stop(); return 1; }

    for (uint8_t i = 0; i < _tx_count; i++) {
        nack = _i2c_write_byte(_tx_buf[i]);
        if (nack) { _i2c_stop(); return 1; }
    }

    _i2c_stop();
    return 0;
}

uint8_t hal_i2c_request_from(uint8_t address, uint8_t count)
{
    if (count > HAL_RX_BUF_SIZE) count = HAL_RX_BUF_SIZE;

    _rx_index = 0;
    _rx_count = 0;

    _i2c_start();

    /* Send address byte with R bit (bit 0 = 1) */
    uint8_t nack = _i2c_write_byte(((uint8_t)(address << 1)) | 0x01);
    if (nack) { _i2c_stop(); return 0; }

    for (uint8_t i = 0; i < count; i++) {
        bool send_ack = (i < (count - 1));   /* NACK on last byte */
        _rx_buf[i] = _i2c_read_byte(send_ack);
        _rx_count++;
    }

    _i2c_stop();
    return _rx_count;
}

uint8_t hal_i2c_read(void)
{
    if (_rx_index < _rx_count) return _rx_buf[_rx_index++];
    return 0xFF;
}

void hal_i2c_set_clock(uint32_t hz)
{
    /* TODO: reconfigure I2C baud-rate register.
     * Example PIC32MX (Fpb = 40 MHz):
     *   I2C1BRG = (uint16_t)((40000000UL / (2UL * hz)) - 2UL);
     */
    (void)hz;
}

#endif /* bare-metal -------------------------------------------------------- */


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
 * ========================================================================= */

void hal_delay_ms(uint32_t ms)
{
    /* TODO: implement a millisecond busy-wait.
     *
     * Options:
     *  a) Harmony v3 SYS_TIME:  SYS_TIME_DelayMS(ms, &handle); ...poll...
     *  b) Core timer (PIC32):
     *       uint32_t ticks = (SYS_CLK_FREQ / 2000UL) * ms;
     *       uint32_t start = _CP0_GET_COUNT();
     *       while ((_CP0_GET_COUNT() - start) < ticks) {}
     *  c) Simple loop (inaccurate, but works for bring-up):
     *       volatile uint32_t i;
     *       for (i = 0; i < ms * 4000UL; i++) {}
     */
    (void)ms;
}

void hal_delay_us(uint32_t us)
{
    /* TODO: implement a microsecond busy-wait.
     *
     * Core timer example (80 MHz ? 40 MHz core timer):
     *   uint32_t ticks = (SYS_CLK_FREQ / 2000000UL) * us;
     *   uint32_t start = _CP0_GET_COUNT();
     *   while ((_CP0_GET_COUNT() - start) < ticks) {}
     */
    (void)us;
}