/**
 * @file si4735_hal.h
 * @brief Platform Abstraction Layer for SI4735 C port
 *
 * @details Fill in the implementations in si4735_hal.c with your
 * PIC32-specific I2C, GPIO and timing calls. Everything in si4735.c
 * calls only the functions declared here ? no Arduino/Wire code remains.
 *
 * Tested target: PIC32 / MPLAB X / XC32
 */

#ifndef SI4735_HAL_H
#define SI4735_HAL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================================
 * I2C
 * =========================================================================
 * The SI4735 is always the only target on the bus in typical designs, so a
 * simple blocking API is sufficient.  If you share the bus with other
 * devices just add a mutex/semaphore around each transaction in the HAL.
 * ========================================================================= */

/**
 * @brief Start an I2C write transaction to the given 7-bit address.
 * @details Call this once, then call hal_i2c_write() for each byte,
 *          then call hal_i2c_end_transmission().
 */
void hal_i2c_begin_transmission(uint8_t address);

/**
 * @brief Write one byte into the current I2C transaction buffer.
 * @return Number of bytes successfully queued (1 on success, 0 on failure).
 */
uint8_t hal_i2c_write(uint8_t data);

/**
 * @brief End the current I2C transaction and send a STOP condition.
 * @return 0 = success, non-zero = error code.
 */
uint8_t hal_i2c_end_transmission(void);

/**
 * @brief Request @p count bytes from @p address via I2C (repeated START or STOP+START).
 * @details Blocks until all bytes are received or a timeout occurs.
 *          Bytes are stored internally and retrieved with hal_i2c_read().
 * @return Number of bytes actually received.
 */
uint8_t hal_i2c_request_from(uint8_t address, uint8_t count);

/**
 * @brief Read one byte from the internal receive buffer filled by hal_i2c_request_from().
 * @return The next received byte, or 0xFF if the buffer is empty.
 */
uint8_t hal_i2c_read(void);

/**
 * @brief Set the I2C clock frequency in Hz.
 * @details Used by the optional speed-change helpers (100 kHz / 400 kHz / custom).
 */
void hal_i2c_set_clock(uint32_t hz);

/* =========================================================================
 * GPIO
 * ========================================================================= */

/**
 * @brief Configure the SI4735 RESET pin as a digital output and drive it HIGH.
 * @details Call once during initialisation before the first hal_gpio_reset_set().
 */
void hal_gpio_reset_init(void);

/**
 * @brief Drive the SI4735 RESET pin.
 * @param high  true  = drive HIGH (device released from reset)
 *              false = drive LOW  (device held in reset)
 */
void hal_gpio_reset_set(bool high);

/**
 * @brief (Optional) Configure a digital output pin used for hardware audio mute.
 * @param pin  An arbitrary identifier your HAL understands (e.g. a port/pin index).
 *             Pass -1 to indicate "no mute pin" and the mute functions become no-ops.
 */
void hal_gpio_mute_init(int8_t pin);

/**
 * @brief Drive the hardware audio mute pin.
 * @param mute  true = mute (e.g. pin HIGH), false = unmute.
 */
void hal_gpio_mute_set(bool mute);

/* =========================================================================
 * Timing
 * ========================================================================= */

/** @brief Busy-wait for at least @p ms milliseconds. */
void hal_delay_ms(uint32_t ms);

/** @brief Busy-wait for at least @p us microseconds. */
void hal_delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* SI4735_HAL_H */