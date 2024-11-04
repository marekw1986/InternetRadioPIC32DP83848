/*
 Copyright (C) 2012 Andy Karpov <andy.karpov@gmail.com>
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Modifications by Marek Wiecek (2023)
 */

#include <string.h>
#include "vs1053_low_level.h"
#include "ringbuffer.h"
#include "system/debug/sys_debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "peripheral/coretimer/plib_coretimer.h"
#include "../common.h"

#ifndef min
#define min(a,b) ((a<b) ?a:b)
#endif

#define vs1053_chunk_size 32

// VS1053 SCI Write Command byte is 0x02
#define VS_WRITE_COMMAND 0x02

// VS1053 SCI Read COmmand byte is 0x03
#define VS_READ_COMMAND  0x03

// Register names
typedef enum {
    reg_name_MODE = 0,
    reg_name_STATUS,
    reg_name_BASS,
    reg_name_CLOCKF,
    reg_name_DECODE_TIME,
    reg_name_AUDATA,
    reg_name_WRAM,
    reg_name_WRAMADDR,
    reg_name_HDAT0,
    reg_name_HDAT1,
    reg_name_AIADDR,
    reg_name_VOL,
    reg_name_AICTRL0,
    reg_name_AICTRL1,
    reg_name_AICTRL2,
    reg_name_AICTRL3
} register_names_t;

const char * register_names[] =
{
    "MODE",
    "STATUS",
    "BASS",
    "CLOCKF",
    "DECODE_TIME",
    "AUDATA",
    "WRAM",
    "WRAMADDR",
    "HDAT0",
    "HDAT1",
    "AIADDR",
    "VOL",
    "AICTRL0",
    "AICTRL1",
    "AICTRL2",
    "AICTRL3",
};

static void VS1053_print_byte_register(uint8_t reg);
static void VS1053_printDetails(void);
static inline void await_data_request(void);
static inline void control_mode_on(void);
static inline void control_mode_off(void);
static inline void data_mode_on(void);
static inline void data_mode_off(void);
static uint8_t VS1053_SPI_transfer(uint8_t outB);

void VS1053_low_level_init(void) {
    // Keep the chip in reset until we are ready
    VS_RESET_TRIS = 0;
    VS_RESET_PIN = 0;

    // The SCI and SDI will start deselected
    VS_CS_TRIS = 0;
    VS_CS_PIN = 1;
    VS_DCS_TRIS = 0;
    VS_DCS_PIN = 1;

    // DREQ is an input
    VS_DREQ_TRIS = 1;

    // Boot VS1053
    SYS_CONSOLE_PRINT("Booting VS1053...\r\n");

    vTaskDelay(1);

    // init SPI slow mode
    //SPI configuration     
    SPI1CON = (_SPI1CON_ON_MASK  | _SPI1CON_CKE_MASK | _SPI1CON_MSTEN_MASK);    //8 bit master mode, CKE=1, CKP=0
    SPI1BRG = 79; //(GetPeripheralClock()-1ul)/2ul/250000;       //250 kHz  

    // release from reset
    VS_RESET_PIN = 1;

    // Declick: Immediately switch analog off
    VS1053_write_register(SCI_VOL,0xffff); // VOL

    /* Declick: Slow sample rate for slow analog part startup */
    VS1053_write_register(SCI_AUDATA,10);

    vTaskDelay(100);

    /* Switch on the analog parts */
    VS1053_write_register(SCI_VOL,0xfefe); // VOL

    SYS_CONSOLE_PRINT("VS1053 still booting\r\n");

    VS1053_write_register(SCI_AUDATA,44101); // 44.1kHz stereo

    VS1053_write_register(SCI_VOL,0x2020); // VOL

    // soft reset
    VS1053_write_register(SCI_MODE, (1 << SM_SDINEW) | (1 << SM_RESET) );
    vTaskDelay(1);
    await_data_request();
    // VS1053_write_register(SCI_CLOCKF,0xF800); // Experimenting with highest clock settings (it was from VS1053)
    VS1053_write_register(SCI_CLOCKF,0xE800); // New setting for VS1053
    vTaskDelay(1);
    await_data_request();

    // Now you can set high speed SPI clock   
    SPI1CON = (_SPI1CON_ON_MASK  | _SPI1CON_CKE_MASK | _SPI1CON_MSTEN_MASK);    //8 bit master mode, CKE=1, CKP=0
    SPI1BRG = 2; //8MHz

    SYS_CONSOLE_PRINT("VS1053 Set\r\n");
    VS1053_printDetails();
    SYS_CONSOLE_PRINT("VS1053 OK\r\n");
}

void VS1053_soft_reset(void) {
    VS1053_write_register(SCI_MODE, (1 << SM_RESET));
    CORETIMER_DelayUs(2);
    while(!VS_DREQ_PIN) {}
}

uint16_t VS1053_read_register(uint8_t _reg) {
    uint16_t result;
    control_mode_on();
    CORETIMER_DelayUs(1); // tXCSS
    VS1053_SPI_transfer(VS_READ_COMMAND); // Read operation
    VS1053_SPI_transfer(_reg); // Which register
    result = VS1053_SPI_transfer(0xff) << 8; // read high byte
    result |= VS1053_SPI_transfer(0xff); // read low byte
    CORETIMER_DelayUs(1); // tXCSH
    await_data_request();
    control_mode_off();
    return result;
}

void VS1053_write_register(uint8_t _reg,uint16_t _value) {
    control_mode_on();
    CORETIMER_DelayUs(1); //delay_us(1); // tXCSS
    VS1053_SPI_transfer(VS_WRITE_COMMAND); // Write operation
    VS1053_SPI_transfer(_reg); // Which register
    VS1053_SPI_transfer(_value >> 8); // Send hi byte
    VS1053_SPI_transfer(_value & 0xff); // Send lo byte
    CORETIMER_DelayUs(1); //delay_us(1); // tXCSH
    await_data_request();
    control_mode_off();
}

void VS1053_sdi_send_buffer(const uint8_t* data, int len) {
    int chunk_length;

    data_mode_on();
    while ( len ) {
        await_data_request();
        CORETIMER_DelayUs(3); //delay_us(3);
        chunk_length = min(len, vs1053_chunk_size);
        len -= chunk_length;
        while ( chunk_length-- ) VS1053_SPI_transfer(*data++);
    }
    data_mode_off();
}

void VS1053_sdi_send_chunk(const uint8_t* data, int len) {
    if (len > 32) return;
    data_mode_on();
    await_data_request();
    while ( len-- ) VS1053_SPI_transfer(*data++);
    data_mode_off();
}

void VS1053_sdi_send_zeroes(int len) {
    int chunk_length;  
    data_mode_on();
    while ( len ) {
        await_data_request();
        chunk_length = min(len,vs1053_chunk_size);
        len -= chunk_length;
        while ( chunk_length-- ) VS1053_SPI_transfer(0);
    }
    data_mode_off();
}

feed_ret_t VS1053_feed_from_buffer (void) {
    uint8_t data[32];

    if (!VS_DREQ_PIN) {
        return FEED_RET_NO_DATA_NEEDED;
    }

    uint32_t timeout = millis();
    do {
        if (get_num_of_bytes_in_ringbuffer() < 32) return FEED_RET_BUFFER_EMPTY;

        uint16_t w = read_array_from_ringbuffer(data, 32);
        if (w == 32) VS1053_sdi_send_chunk(data, 32);
        asm("nop");
        asm("nop");
        asm("nop");
        if ((uint32_t)(millis()-timeout) > 100) {
            return FEED_RET_ERR_TMOUT;
        }
    } while(VS_DREQ_PIN);

    return FEED_RET_OK;
}

static void VS1053_print_byte_register(uint8_t reg) {
    char extra_tab = strlen(register_names[reg]) < 5 ? '\t' : 0;
    SYS_CONSOLE_PRINT("%02x %s\t%c = 0x%02x\r\n", reg, register_names[reg], extra_tab, VS1053_read_register(reg));
}

static void VS1053_printDetails(void) {
    SYS_CONSOLE_PRINT("VS1053 Configuration:\r\n");
    int i = 0;
    while ( i <= SCI_num_registers ) {
        VS1053_print_byte_register(i++);
    }
}

static inline void await_data_request(void) {
    while ( !VS_DREQ_PIN );
}

static inline void control_mode_on(void) {
    VS_DCS_PIN = 1;
    VS_CS_PIN = 0;
}

static inline void control_mode_off(void) {
    VS_CS_PIN = 1;
}

static inline void data_mode_on(void) {
    VS_CS_PIN = 1;
    VS_DCS_PIN = 0;
}

static inline void data_mode_off(void) {
    VS_DCS_PIN = 1;
}

static uint8_t VS1053_SPI_transfer(uint8_t outB) {
    SPI1BUF = outB;
    while (SPI1STATbits.SPITBF);
    while (!SPI1STATbits.SPIRBF);
    return SPI1BUF;
}

//void VS1053_loadUserCode(const uint16_t* buf, size_t len) {
//  while (len) {
//    uint16_t addr = *buf++; len--;
//    uint16_t n = *buf++; len--;
//    if (n & 0x8000U) { /* RLE run, replicate n samples */
//      n &= 0x7FFF;
//      uint16_t val = *buf++; len--;
//      while (n--) {
//	    SYS_CONSOLE_PRINT("W %02x: %04x\r\n", addr, val);
//        VS1053_write_register(addr, val);
//      }
//    } else {           /* Copy run, copy n samples */
//      while (n--) {
//        uint16_t val = *buf++; len--;
//        SYS_CONSOLE_PRINT("W %02x: %04x\r\n", addr, val);
//        VS1053_write_register(addr, val);
//      }
//    }
//  }
//}