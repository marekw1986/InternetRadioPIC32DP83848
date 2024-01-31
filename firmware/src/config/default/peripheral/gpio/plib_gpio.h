/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for GPIO_RG15 pin ***/
#define GPIO_RG15_Set()               (LATGSET = (1<<15))
#define GPIO_RG15_Clear()             (LATGCLR = (1<<15))
#define GPIO_RG15_Toggle()            (LATGINV= (1<<15))
#define GPIO_RG15_OutputEnable()      (TRISGCLR = (1<<15))
#define GPIO_RG15_InputEnable()       (TRISGSET = (1<<15))
#define GPIO_RG15_Get()               ((PORTG >> 15) & 0x1)
#define GPIO_RG15_PIN                  GPIO_PIN_RG15

/*** Macros for GPIO_RE5 pin ***/
#define GPIO_RE5_Set()               (LATESET = (1<<5))
#define GPIO_RE5_Clear()             (LATECLR = (1<<5))
#define GPIO_RE5_Toggle()            (LATEINV= (1<<5))
#define GPIO_RE5_OutputEnable()      (TRISECLR = (1<<5))
#define GPIO_RE5_InputEnable()       (TRISESET = (1<<5))
#define GPIO_RE5_Get()               ((PORTE >> 5) & 0x1)
#define GPIO_RE5_PIN                  GPIO_PIN_RE5

/*** Macros for VS_XDCS pin ***/
#define VS_XDCS_Set()               (LATESET = (1<<6))
#define VS_XDCS_Clear()             (LATECLR = (1<<6))
#define VS_XDCS_Toggle()            (LATEINV= (1<<6))
#define VS_XDCS_OutputEnable()      (TRISECLR = (1<<6))
#define VS_XDCS_InputEnable()       (TRISESET = (1<<6))
#define VS_XDCS_Get()               ((PORTE >> 6) & 0x1)
#define VS_XDCS_PIN                  GPIO_PIN_RE6

/*** Macros for VS_XCS pin ***/
#define VS_XCS_Set()               (LATESET = (1<<7))
#define VS_XCS_Clear()             (LATECLR = (1<<7))
#define VS_XCS_Toggle()            (LATEINV= (1<<7))
#define VS_XCS_OutputEnable()      (TRISECLR = (1<<7))
#define VS_XCS_InputEnable()       (TRISESET = (1<<7))
#define VS_XCS_Get()               ((PORTE >> 7) & 0x1)
#define VS_XCS_PIN                  GPIO_PIN_RE7

/*** Macros for VS_DREQ pin ***/
#define VS_DREQ_Set()               (LATCSET = (1<<1))
#define VS_DREQ_Clear()             (LATCCLR = (1<<1))
#define VS_DREQ_Toggle()            (LATCINV= (1<<1))
#define VS_DREQ_OutputEnable()      (TRISCCLR = (1<<1))
#define VS_DREQ_InputEnable()       (TRISCSET = (1<<1))
#define VS_DREQ_Get()               ((PORTC >> 1) & 0x1)
#define VS_DREQ_PIN                  GPIO_PIN_RC1

/*** Macros for VS_XRST pin ***/
#define VS_XRST_Set()               (LATCSET = (1<<2))
#define VS_XRST_Clear()             (LATCCLR = (1<<2))
#define VS_XRST_Toggle()            (LATCINV= (1<<2))
#define VS_XRST_OutputEnable()      (TRISCCLR = (1<<2))
#define VS_XRST_InputEnable()       (TRISCSET = (1<<2))
#define VS_XRST_Get()               ((PORTC >> 2) & 0x1)
#define VS_XRST_PIN                  GPIO_PIN_RC2

/*** Macros for GPIO_RC3 pin ***/
#define GPIO_RC3_Set()               (LATCSET = (1<<3))
#define GPIO_RC3_Clear()             (LATCCLR = (1<<3))
#define GPIO_RC3_Toggle()            (LATCINV= (1<<3))
#define GPIO_RC3_OutputEnable()      (TRISCCLR = (1<<3))
#define GPIO_RC3_InputEnable()       (TRISCSET = (1<<3))
#define GPIO_RC3_Get()               ((PORTC >> 3) & 0x1)
#define GPIO_RC3_PIN                  GPIO_PIN_RC3

/*** Macros for GPIO_RG6 pin ***/
#define GPIO_RG6_Set()               (LATGSET = (1<<6))
#define GPIO_RG6_Clear()             (LATGCLR = (1<<6))
#define GPIO_RG6_Toggle()            (LATGINV= (1<<6))
#define GPIO_RG6_OutputEnable()      (TRISGCLR = (1<<6))
#define GPIO_RG6_InputEnable()       (TRISGSET = (1<<6))
#define GPIO_RG6_Get()               ((PORTG >> 6) & 0x1)
#define GPIO_RG6_PIN                  GPIO_PIN_RG6

/*** Macros for GPIO_RG7 pin ***/
#define GPIO_RG7_Set()               (LATGSET = (1<<7))
#define GPIO_RG7_Clear()             (LATGCLR = (1<<7))
#define GPIO_RG7_Toggle()            (LATGINV= (1<<7))
#define GPIO_RG7_OutputEnable()      (TRISGCLR = (1<<7))
#define GPIO_RG7_InputEnable()       (TRISGSET = (1<<7))
#define GPIO_RG7_Get()               ((PORTG >> 7) & 0x1)
#define GPIO_RG7_PIN                  GPIO_PIN_RG7

/*** Macros for GPIO_RA0 pin ***/
#define GPIO_RA0_Set()               (LATASET = (1<<0))
#define GPIO_RA0_Clear()             (LATACLR = (1<<0))
#define GPIO_RA0_Toggle()            (LATAINV= (1<<0))
#define GPIO_RA0_OutputEnable()      (TRISACLR = (1<<0))
#define GPIO_RA0_InputEnable()       (TRISASET = (1<<0))
#define GPIO_RA0_Get()               ((PORTA >> 0) & 0x1)
#define GPIO_RA0_PIN                  GPIO_PIN_RA0

/*** Macros for GPIO_RE8 pin ***/
#define GPIO_RE8_Set()               (LATESET = (1<<8))
#define GPIO_RE8_Clear()             (LATECLR = (1<<8))
#define GPIO_RE8_Toggle()            (LATEINV= (1<<8))
#define GPIO_RE8_OutputEnable()      (TRISECLR = (1<<8))
#define GPIO_RE8_InputEnable()       (TRISESET = (1<<8))
#define GPIO_RE8_Get()               ((PORTE >> 8) & 0x1)
#define GPIO_RE8_PIN                  GPIO_PIN_RE8

/*** Macros for GPIO_RE9 pin ***/
#define GPIO_RE9_Set()               (LATESET = (1<<9))
#define GPIO_RE9_Clear()             (LATECLR = (1<<9))
#define GPIO_RE9_Toggle()            (LATEINV= (1<<9))
#define GPIO_RE9_OutputEnable()      (TRISECLR = (1<<9))
#define GPIO_RE9_InputEnable()       (TRISESET = (1<<9))
#define GPIO_RE9_Get()               ((PORTE >> 9) & 0x1)
#define GPIO_RE9_PIN                  GPIO_PIN_RE9

/*** Macros for GPIO_RB5 pin ***/
#define GPIO_RB5_Set()               (LATBSET = (1<<5))
#define GPIO_RB5_Clear()             (LATBCLR = (1<<5))
#define GPIO_RB5_Toggle()            (LATBINV= (1<<5))
#define GPIO_RB5_OutputEnable()      (TRISBCLR = (1<<5))
#define GPIO_RB5_InputEnable()       (TRISBSET = (1<<5))
#define GPIO_RB5_Get()               ((PORTB >> 5) & 0x1)
#define GPIO_RB5_PIN                  GPIO_PIN_RB5

/*** Macros for GPIO_RB4 pin ***/
#define GPIO_RB4_Set()               (LATBSET = (1<<4))
#define GPIO_RB4_Clear()             (LATBCLR = (1<<4))
#define GPIO_RB4_Toggle()            (LATBINV= (1<<4))
#define GPIO_RB4_OutputEnable()      (TRISBCLR = (1<<4))
#define GPIO_RB4_InputEnable()       (TRISBSET = (1<<4))
#define GPIO_RB4_Get()               ((PORTB >> 4) & 0x1)
#define GPIO_RB4_PIN                  GPIO_PIN_RB4

/*** Macros for GPIO_RB3 pin ***/
#define GPIO_RB3_Set()               (LATBSET = (1<<3))
#define GPIO_RB3_Clear()             (LATBCLR = (1<<3))
#define GPIO_RB3_Toggle()            (LATBINV= (1<<3))
#define GPIO_RB3_OutputEnable()      (TRISBCLR = (1<<3))
#define GPIO_RB3_InputEnable()       (TRISBSET = (1<<3))
#define GPIO_RB3_Get()               ((PORTB >> 3) & 0x1)
#define GPIO_RB3_PIN                  GPIO_PIN_RB3

/*** Macros for GPIO_RB2 pin ***/
#define GPIO_RB2_Set()               (LATBSET = (1<<2))
#define GPIO_RB2_Clear()             (LATBCLR = (1<<2))
#define GPIO_RB2_Toggle()            (LATBINV= (1<<2))
#define GPIO_RB2_OutputEnable()      (TRISBCLR = (1<<2))
#define GPIO_RB2_InputEnable()       (TRISBSET = (1<<2))
#define GPIO_RB2_Get()               ((PORTB >> 2) & 0x1)
#define GPIO_RB2_PIN                  GPIO_PIN_RB2

/*** Macros for GPIO_RB1 pin ***/
#define GPIO_RB1_Set()               (LATBSET = (1<<1))
#define GPIO_RB1_Clear()             (LATBCLR = (1<<1))
#define GPIO_RB1_Toggle()            (LATBINV= (1<<1))
#define GPIO_RB1_OutputEnable()      (TRISBCLR = (1<<1))
#define GPIO_RB1_InputEnable()       (TRISBSET = (1<<1))
#define GPIO_RB1_Get()               ((PORTB >> 1) & 0x1)
#define GPIO_RB1_PIN                  GPIO_PIN_RB1

/*** Macros for GPIO_RB0 pin ***/
#define GPIO_RB0_Set()               (LATBSET = (1<<0))
#define GPIO_RB0_Clear()             (LATBCLR = (1<<0))
#define GPIO_RB0_Toggle()            (LATBINV= (1<<0))
#define GPIO_RB0_OutputEnable()      (TRISBCLR = (1<<0))
#define GPIO_RB0_InputEnable()       (TRISBSET = (1<<0))
#define GPIO_RB0_Get()               ((PORTB >> 0) & 0x1)
#define GPIO_RB0_PIN                  GPIO_PIN_RB0

/*** Macros for GPIO_RB6 pin ***/
#define GPIO_RB6_Set()               (LATBSET = (1<<6))
#define GPIO_RB6_Clear()             (LATBCLR = (1<<6))
#define GPIO_RB6_Toggle()            (LATBINV= (1<<6))
#define GPIO_RB6_OutputEnable()      (TRISBCLR = (1<<6))
#define GPIO_RB6_InputEnable()       (TRISBSET = (1<<6))
#define GPIO_RB6_Get()               ((PORTB >> 6) & 0x1)
#define GPIO_RB6_PIN                  GPIO_PIN_RB6

/*** Macros for GPIO_RB7 pin ***/
#define GPIO_RB7_Set()               (LATBSET = (1<<7))
#define GPIO_RB7_Clear()             (LATBCLR = (1<<7))
#define GPIO_RB7_Toggle()            (LATBINV= (1<<7))
#define GPIO_RB7_OutputEnable()      (TRISBCLR = (1<<7))
#define GPIO_RB7_InputEnable()       (TRISBSET = (1<<7))
#define GPIO_RB7_Get()               ((PORTB >> 7) & 0x1)
#define GPIO_RB7_PIN                  GPIO_PIN_RB7

/*** Macros for GPIO_RA9 pin ***/
#define GPIO_RA9_Set()               (LATASET = (1<<9))
#define GPIO_RA9_Clear()             (LATACLR = (1<<9))
#define GPIO_RA9_Toggle()            (LATAINV= (1<<9))
#define GPIO_RA9_OutputEnable()      (TRISACLR = (1<<9))
#define GPIO_RA9_InputEnable()       (TRISASET = (1<<9))
#define GPIO_RA9_Get()               ((PORTA >> 9) & 0x1)
#define GPIO_RA9_PIN                  GPIO_PIN_RA9

/*** Macros for GPIO_RA10 pin ***/
#define GPIO_RA10_Set()               (LATASET = (1<<10))
#define GPIO_RA10_Clear()             (LATACLR = (1<<10))
#define GPIO_RA10_Toggle()            (LATAINV= (1<<10))
#define GPIO_RA10_OutputEnable()      (TRISACLR = (1<<10))
#define GPIO_RA10_InputEnable()       (TRISASET = (1<<10))
#define GPIO_RA10_Get()               ((PORTA >> 10) & 0x1)
#define GPIO_RA10_PIN                  GPIO_PIN_RA10

/*** Macros for GPIO_RB8 pin ***/
#define GPIO_RB8_Set()               (LATBSET = (1<<8))
#define GPIO_RB8_Clear()             (LATBCLR = (1<<8))
#define GPIO_RB8_Toggle()            (LATBINV= (1<<8))
#define GPIO_RB8_OutputEnable()      (TRISBCLR = (1<<8))
#define GPIO_RB8_InputEnable()       (TRISBSET = (1<<8))
#define GPIO_RB8_Get()               ((PORTB >> 8) & 0x1)
#define GPIO_RB8_PIN                  GPIO_PIN_RB8

/*** Macros for GPIO_RB9 pin ***/
#define GPIO_RB9_Set()               (LATBSET = (1<<9))
#define GPIO_RB9_Clear()             (LATBCLR = (1<<9))
#define GPIO_RB9_Toggle()            (LATBINV= (1<<9))
#define GPIO_RB9_OutputEnable()      (TRISBCLR = (1<<9))
#define GPIO_RB9_InputEnable()       (TRISBSET = (1<<9))
#define GPIO_RB9_Get()               ((PORTB >> 9) & 0x1)
#define GPIO_RB9_PIN                  GPIO_PIN_RB9

/*** Macros for DP_RST pin ***/
#define DP_RST_Set()               (LATBSET = (1<<10))
#define DP_RST_Clear()             (LATBCLR = (1<<10))
#define DP_RST_Toggle()            (LATBINV= (1<<10))
#define DP_RST_OutputEnable()      (TRISBCLR = (1<<10))
#define DP_RST_InputEnable()       (TRISBSET = (1<<10))
#define DP_RST_Get()               ((PORTB >> 10) & 0x1)
#define DP_RST_PIN                  GPIO_PIN_RB10

/*** Macros for FLASH_CS pin ***/
#define FLASH_CS_Set()               (LATASET = (1<<1))
#define FLASH_CS_Clear()             (LATACLR = (1<<1))
#define FLASH_CS_Toggle()            (LATAINV= (1<<1))
#define FLASH_CS_OutputEnable()      (TRISACLR = (1<<1))
#define FLASH_CS_InputEnable()       (TRISASET = (1<<1))
#define FLASH_CS_Get()               ((PORTA >> 1) & 0x1)
#define FLASH_CS_PIN                  GPIO_PIN_RA1

/*** Macros for GPIO_RF12 pin ***/
#define GPIO_RF12_Set()               (LATFSET = (1<<12))
#define GPIO_RF12_Clear()             (LATFCLR = (1<<12))
#define GPIO_RF12_Toggle()            (LATFINV= (1<<12))
#define GPIO_RF12_OutputEnable()      (TRISFCLR = (1<<12))
#define GPIO_RF12_InputEnable()       (TRISFSET = (1<<12))
#define GPIO_RF12_Get()               ((PORTF >> 12) & 0x1)
#define GPIO_RF12_PIN                  GPIO_PIN_RF12

/*** Macros for GPIO_RB14 pin ***/
#define GPIO_RB14_Set()               (LATBSET = (1<<14))
#define GPIO_RB14_Clear()             (LATBCLR = (1<<14))
#define GPIO_RB14_Toggle()            (LATBINV= (1<<14))
#define GPIO_RB14_OutputEnable()      (TRISBCLR = (1<<14))
#define GPIO_RB14_InputEnable()       (TRISBSET = (1<<14))
#define GPIO_RB14_Get()               ((PORTB >> 14) & 0x1)
#define GPIO_RB14_PIN                  GPIO_PIN_RB14

/*** Macros for SD_CS pin ***/
#define SD_CS_Set()               (LATBSET = (1<<15))
#define SD_CS_Clear()             (LATBCLR = (1<<15))
#define SD_CS_Toggle()            (LATBINV= (1<<15))
#define SD_CS_OutputEnable()      (TRISBCLR = (1<<15))
#define SD_CS_InputEnable()       (TRISBSET = (1<<15))
#define SD_CS_Get()               ((PORTB >> 15) & 0x1)
#define SD_CS_PIN                  GPIO_PIN_RB15

/*** Macros for GPIO_RF3 pin ***/
#define GPIO_RF3_Set()               (LATFSET = (1<<3))
#define GPIO_RF3_Clear()             (LATFCLR = (1<<3))
#define GPIO_RF3_Toggle()            (LATFINV= (1<<3))
#define GPIO_RF3_OutputEnable()      (TRISFCLR = (1<<3))
#define GPIO_RF3_InputEnable()       (TRISFSET = (1<<3))
#define GPIO_RF3_Get()               ((PORTF >> 3) & 0x1)
#define GPIO_RF3_PIN                  GPIO_PIN_RF3

/*** Macros for GPIO_RF2 pin ***/
#define GPIO_RF2_Set()               (LATFSET = (1<<2))
#define GPIO_RF2_Clear()             (LATFCLR = (1<<2))
#define GPIO_RF2_Toggle()            (LATFINV= (1<<2))
#define GPIO_RF2_OutputEnable()      (TRISFCLR = (1<<2))
#define GPIO_RF2_InputEnable()       (TRISFSET = (1<<2))
#define GPIO_RF2_Get()               ((PORTF >> 2) & 0x1)
#define GPIO_RF2_PIN                  GPIO_PIN_RF2

/*** Macros for GPIO_RF8 pin ***/
#define GPIO_RF8_Set()               (LATFSET = (1<<8))
#define GPIO_RF8_Clear()             (LATFCLR = (1<<8))
#define GPIO_RF8_Toggle()            (LATFINV= (1<<8))
#define GPIO_RF8_OutputEnable()      (TRISFCLR = (1<<8))
#define GPIO_RF8_InputEnable()       (TRISFSET = (1<<8))
#define GPIO_RF8_Get()               ((PORTF >> 8) & 0x1)
#define GPIO_RF8_PIN                  GPIO_PIN_RF8

/*** Macros for GPIO_RA2 pin ***/
//#define GPIO_RA2_Set()               (LATASET = (1<<2))
//#define GPIO_RA2_Clear()             (LATACLR = (1<<2))
//#define GPIO_RA2_Toggle()            (LATAINV= (1<<2))
//#define GPIO_RA2_OutputEnable()      (TRISACLR = (1<<2))
//#define GPIO_RA2_InputEnable()       (TRISASET = (1<<2))
//#define GPIO_RA2_Get()               ((PORTA >> 2) & 0x1)
//#define GPIO_RA2_PIN                  GPIO_PIN_RA2

/*** Macros for GPIO_RA2 pin ***/
#define GPIO_RA2_Set()               (LATASET = (1<<3))
#define GPIO_RA2_Clear()             (LATACLR = (1<<3))
#define GPIO_RA2_Toggle()            (LATAINV= (1<<3))
#define GPIO_RA2_OutputEnable()      (TRISACLR = (1<<3))
#define GPIO_RA2_InputEnable()       (TRISASET = (1<<3))
#define GPIO_RA2_Get()               ((PORTA >> 3) & 0x1)
#define GPIO_RA2_PIN                  GPIO_PIN_RA3

/*** Macros for GPIO_RA3 pin ***/
#define GPIO_RA3_Set()               (LATASET = (1<<4))
#define GPIO_RA3_Clear()             (LATACLR = (1<<4))
#define GPIO_RA3_Toggle()            (LATAINV= (1<<4))
#define GPIO_RA3_OutputEnable()      (TRISACLR = (1<<4))
#define GPIO_RA3_InputEnable()       (TRISASET = (1<<4))
#define GPIO_RA3_Get()               ((PORTA >> 4) & 0x1)
#define GPIO_RA3_PIN                  GPIO_PIN_RA4

/*** Macros for GPIO_RA4 pin ***/
#define GPIO_RA4_Set()               (LATASET = (1<<5))
#define GPIO_RA4_Clear()             (LATACLR = (1<<5))
#define GPIO_RA4_Toggle()            (LATAINV= (1<<5))
#define GPIO_RA4_OutputEnable()      (TRISACLR = (1<<5))
#define GPIO_RA4_InputEnable()       (TRISASET = (1<<5))
#define GPIO_RA4_Get()               ((PORTA >> 5) & 0x1)
#define GPIO_RA4_PIN                  GPIO_PIN_RA5

/*** Macros for LCD_BACKLIGHT pin ***/
#define LCD_BACKLIGHT_Set()               (LATCSET = (1<<15))
#define LCD_BACKLIGHT_Clear()             (LATCCLR = (1<<15))
#define LCD_BACKLIGHT_Toggle()            (LATCINV= (1<<15))
#define LCD_BACKLIGHT_OutputEnable()      (TRISCCLR = (1<<15))
#define LCD_BACKLIGHT_InputEnable()       (TRISCSET = (1<<15))
#define LCD_BACKLIGHT_Get()               ((PORTC >> 15) & 0x1)
#define LCD_BACKLIGHT_PIN                  GPIO_PIN_RC15

/*** Macros for GPIO_RA14 pin ***/
#define GPIO_RA14_Set()               (LATASET = (1<<14))
#define GPIO_RA14_Clear()             (LATACLR = (1<<14))
#define GPIO_RA14_Toggle()            (LATAINV= (1<<14))
#define GPIO_RA14_OutputEnable()      (TRISACLR = (1<<14))
#define GPIO_RA14_InputEnable()       (TRISASET = (1<<14))
#define GPIO_RA14_Get()               ((PORTA >> 14) & 0x1)
#define GPIO_RA14_PIN                  GPIO_PIN_RA14

/*** Macros for GPIO_RA15 pin ***/
#define GPIO_RA15_Set()               (LATASET = (1<<15))
#define GPIO_RA15_Clear()             (LATACLR = (1<<15))
#define GPIO_RA15_Toggle()            (LATAINV= (1<<15))
#define GPIO_RA15_OutputEnable()      (TRISACLR = (1<<15))
#define GPIO_RA15_InputEnable()       (TRISASET = (1<<15))
#define GPIO_RA15_Get()               ((PORTA >> 15) & 0x1)
#define GPIO_RA15_PIN                  GPIO_PIN_RA15

/*** Macros for GPIO_RD9 pin ***/
#define GPIO_RD9_Set()               (LATDSET = (1<<9))
#define GPIO_RD9_Clear()             (LATDCLR = (1<<9))
#define GPIO_RD9_Toggle()            (LATDINV= (1<<9))
#define GPIO_RD9_OutputEnable()      (TRISDCLR = (1<<9))
#define GPIO_RD9_InputEnable()       (TRISDSET = (1<<9))
#define GPIO_RD9_Get()               ((PORTD >> 9) & 0x1)
#define GPIO_RD9_PIN                  GPIO_PIN_RD9

/*** Macros for GPIO_RD1 pin ***/
#define GPIO_RD1_Set()               (LATDSET = (1<<1))
#define GPIO_RD1_Clear()             (LATDCLR = (1<<1))
#define GPIO_RD1_Toggle()            (LATDINV= (1<<1))
#define GPIO_RD1_OutputEnable()      (TRISDCLR = (1<<1))
#define GPIO_RD1_InputEnable()       (TRISDSET = (1<<1))
#define GPIO_RD1_Get()               ((PORTD >> 1) & 0x1)
#define GPIO_RD1_PIN                  GPIO_PIN_RD1

/*** Macros for LCD_D4 pin ***/
#define LCD_D4_Set()               (LATDSET = (1<<2))
#define LCD_D4_Clear()             (LATDCLR = (1<<2))
#define LCD_D4_Toggle()            (LATDINV= (1<<2))
#define LCD_D4_OutputEnable()      (TRISDCLR = (1<<2))
#define LCD_D4_InputEnable()       (TRISDSET = (1<<2))
#define LCD_D4_Get()               ((PORTD >> 2) & 0x1)
#define LCD_D4_PIN                  GPIO_PIN_RD2

/*** Macros for LCD_D5 pin ***/
#define LCD_D5_Set()               (LATDSET = (1<<3))
#define LCD_D5_Clear()             (LATDCLR = (1<<3))
#define LCD_D5_Toggle()            (LATDINV= (1<<3))
#define LCD_D5_OutputEnable()      (TRISDCLR = (1<<3))
#define LCD_D5_InputEnable()       (TRISDSET = (1<<3))
#define LCD_D5_Get()               ((PORTD >> 3) & 0x1)
#define LCD_D5_PIN                  GPIO_PIN_RD3

/*** Macros for GPIO_RD12 pin ***/
#define GPIO_RD12_Set()               (LATDSET = (1<<12))
#define GPIO_RD12_Clear()             (LATDCLR = (1<<12))
#define GPIO_RD12_Toggle()            (LATDINV= (1<<12))
#define GPIO_RD12_OutputEnable()      (TRISDCLR = (1<<12))
#define GPIO_RD12_InputEnable()       (TRISDSET = (1<<12))
#define GPIO_RD12_Get()               ((PORTD >> 12) & 0x1)
#define GPIO_RD12_PIN                  GPIO_PIN_RD12

/*** Macros for GPIO_RD13 pin ***/
#define GPIO_RD13_Set()               (LATDSET = (1<<13))
#define GPIO_RD13_Clear()             (LATDCLR = (1<<13))
#define GPIO_RD13_Toggle()            (LATDINV= (1<<13))
#define GPIO_RD13_OutputEnable()      (TRISDCLR = (1<<13))
#define GPIO_RD13_InputEnable()       (TRISDSET = (1<<13))
#define GPIO_RD13_Get()               ((PORTD >> 13) & 0x1)
#define GPIO_RD13_PIN                  GPIO_PIN_RD13

/*** Macros for LCD_D6 pin ***/
#define LCD_D6_Set()               (LATDSET = (1<<4))
#define LCD_D6_Clear()             (LATDCLR = (1<<4))
#define LCD_D6_Toggle()            (LATDINV= (1<<4))
#define LCD_D6_OutputEnable()      (TRISDCLR = (1<<4))
#define LCD_D6_InputEnable()       (TRISDSET = (1<<4))
#define LCD_D6_Get()               ((PORTD >> 4) & 0x1)
#define LCD_D6_PIN                  GPIO_PIN_RD4

/*** Macros for LCD_D7 pin ***/
#define LCD_D7_Set()               (LATDSET = (1<<5))
#define LCD_D7_Clear()             (LATDCLR = (1<<5))
#define LCD_D7_Toggle()            (LATDINV= (1<<5))
#define LCD_D7_OutputEnable()      (TRISDCLR = (1<<5))
#define LCD_D7_InputEnable()       (TRISDSET = (1<<5))
#define LCD_D7_Get()               ((PORTD >> 5) & 0x1)
#define LCD_D7_PIN                  GPIO_PIN_RD5

/*** Macros for GPIO_RD7 pin ***/
#define GPIO_RD7_Set()               (LATDSET = (1<<7))
#define GPIO_RD7_Clear()             (LATDCLR = (1<<7))
#define GPIO_RD7_Toggle()            (LATDINV= (1<<7))
#define GPIO_RD7_OutputEnable()      (TRISDCLR = (1<<7))
#define GPIO_RD7_InputEnable()       (TRISDSET = (1<<7))
#define GPIO_RD7_Get()               ((PORTD >> 7) & 0x1)
#define GPIO_RD7_PIN                  GPIO_PIN_RD7

/*** Macros for GPIO_RG1 pin ***/
#define GPIO_RG1_Set()               (LATGSET = (1<<1))
#define GPIO_RG1_Clear()             (LATGCLR = (1<<1))
#define GPIO_RG1_Toggle()            (LATGINV= (1<<1))
#define GPIO_RG1_OutputEnable()      (TRISGCLR = (1<<1))
#define GPIO_RG1_InputEnable()       (TRISGSET = (1<<1))
#define GPIO_RG1_Get()               ((PORTG >> 1) & 0x1)
#define GPIO_RG1_PIN                  GPIO_PIN_RG1

/*** Macros for GPIO_RG0 pin ***/
#define GPIO_RG0_Set()               (LATGSET = (1<<0))
#define GPIO_RG0_Clear()             (LATGCLR = (1<<0))
#define GPIO_RG0_Toggle()            (LATGINV= (1<<0))
#define GPIO_RG0_OutputEnable()      (TRISGCLR = (1<<0))
#define GPIO_RG0_InputEnable()       (TRISGSET = (1<<0))
#define GPIO_RG0_Get()               ((PORTG >> 0) & 0x1)
#define GPIO_RG0_PIN                  GPIO_PIN_RG0

/*** Macros for GPIO_RA6 pin ***/
#define GPIO_RA6_Set()               (LATASET = (1<<6))
#define GPIO_RA6_Clear()             (LATACLR = (1<<6))
#define GPIO_RA6_Toggle()            (LATAINV= (1<<6))
#define GPIO_RA6_OutputEnable()      (TRISACLR = (1<<6))
#define GPIO_RA6_InputEnable()       (TRISASET = (1<<6))
#define GPIO_RA6_Get()               ((PORTA >> 6) & 0x1)
#define GPIO_RA6_PIN                  GPIO_PIN_RA6

/*** Macros for GPIO_RA7 pin ***/
#define GPIO_RA7_Set()               (LATASET = (1<<7))
#define GPIO_RA7_Clear()             (LATACLR = (1<<7))
#define GPIO_RA7_Toggle()            (LATAINV= (1<<7))
#define GPIO_RA7_OutputEnable()      (TRISACLR = (1<<7))
#define GPIO_RA7_InputEnable()       (TRISASET = (1<<7))
#define GPIO_RA7_Get()               ((PORTA >> 7) & 0x1)
#define GPIO_RA7_PIN                  GPIO_PIN_RA7

/*** Macros for GPIO_RE0 pin ***/
#define GPIO_RE0_Set()               (LATESET = (1<<0))
#define GPIO_RE0_Clear()             (LATECLR = (1<<0))
#define GPIO_RE0_Toggle()            (LATEINV= (1<<0))
#define GPIO_RE0_OutputEnable()      (TRISECLR = (1<<0))
#define GPIO_RE0_InputEnable()       (TRISESET = (1<<0))
#define GPIO_RE0_Get()               ((PORTE >> 0) & 0x1)
#define GPIO_RE0_PIN                  GPIO_PIN_RE0

/*** Macros for LCD_EN pin ***/
#define LCD_EN_Set()               (LATESET = (1<<1))
#define LCD_EN_Clear()             (LATECLR = (1<<1))
#define LCD_EN_Toggle()            (LATEINV= (1<<1))
#define LCD_EN_OutputEnable()      (TRISECLR = (1<<1))
#define LCD_EN_InputEnable()       (TRISESET = (1<<1))
#define LCD_EN_Get()               ((PORTE >> 1) & 0x1)
#define LCD_EN_PIN                  GPIO_PIN_RE1

/*** Macros for LCD_RW pin ***/
#define LCD_RW_Set()               (LATGSET = (1<<14))
#define LCD_RW_Clear()             (LATGCLR = (1<<14))
#define LCD_RW_Toggle()            (LATGINV= (1<<14))
#define LCD_RW_OutputEnable()      (TRISGCLR = (1<<14))
#define LCD_RW_InputEnable()       (TRISGSET = (1<<14))
#define LCD_RW_Get()               ((PORTG >> 14) & 0x1)
#define LCD_RW_PIN                  GPIO_PIN_RG14

/*** Macros for LCD_RS pin ***/
#define LCD_RS_Set()               (LATGSET = (1<<12))
#define LCD_RS_Clear()             (LATGCLR = (1<<12))
#define LCD_RS_Toggle()            (LATGINV= (1<<12))
#define LCD_RS_OutputEnable()      (TRISGCLR = (1<<12))
#define LCD_RS_InputEnable()       (TRISGSET = (1<<12))
#define LCD_RS_Get()               ((PORTG >> 12) & 0x1)
#define LCD_RS_PIN                  GPIO_PIN_RG12

/*** Macros for GPIO_RG13 pin ***/
#define GPIO_RG13_Set()               (LATGSET = (1<<13))
#define GPIO_RG13_Clear()             (LATGCLR = (1<<13))
#define GPIO_RG13_Toggle()            (LATGINV= (1<<13))
#define GPIO_RG13_OutputEnable()      (TRISGCLR = (1<<13))
#define GPIO_RG13_InputEnable()       (TRISGSET = (1<<13))
#define GPIO_RG13_Get()               ((PORTG >> 13) & 0x1)
#define GPIO_RG13_PIN                  GPIO_PIN_RG13

/*** Macros for GPIO_RE2 pin ***/
#define GPIO_RE2_Set()               (LATESET = (1<<2))
#define GPIO_RE2_Clear()             (LATECLR = (1<<2))
#define GPIO_RE2_Toggle()            (LATEINV= (1<<2))
#define GPIO_RE2_OutputEnable()      (TRISECLR = (1<<2))
#define GPIO_RE2_InputEnable()       (TRISESET = (1<<2))
#define GPIO_RE2_Get()               ((PORTE >> 2) & 0x1)
#define GPIO_RE2_PIN                  GPIO_PIN_RE2

/*** Macros for GPIO_RE3 pin ***/
#define GPIO_RE3_Set()               (LATESET = (1<<3))
#define GPIO_RE3_Clear()             (LATECLR = (1<<3))
#define GPIO_RE3_Toggle()            (LATEINV= (1<<3))
#define GPIO_RE3_OutputEnable()      (TRISECLR = (1<<3))
#define GPIO_RE3_InputEnable()       (TRISESET = (1<<3))
#define GPIO_RE3_Get()               ((PORTE >> 3) & 0x1)
#define GPIO_RE3_PIN                  GPIO_PIN_RE3

/*** Macros for GPIO_RE4 pin ***/
#define GPIO_RE4_Set()               (LATESET = (1<<4))
#define GPIO_RE4_Clear()             (LATECLR = (1<<4))
#define GPIO_RE4_Toggle()            (LATEINV= (1<<4))
#define GPIO_RE4_OutputEnable()      (TRISECLR = (1<<4))
#define GPIO_RE4_InputEnable()       (TRISESET = (1<<4))
#define GPIO_RE4_Get()               ((PORTE >> 4) & 0x1)
#define GPIO_RE4_PIN                  GPIO_PIN_RE4


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B = 1,
    GPIO_PORT_C = 2,
    GPIO_PORT_D = 3,
    GPIO_PORT_E = 4,
    GPIO_PORT_F = 5,
    GPIO_PORT_G = 6,
} GPIO_PORT;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    GPIO_PIN_RA0 = 0,
    GPIO_PIN_RA1 = 1,
    GPIO_PIN_RA2 = 2,
    GPIO_PIN_RA3 = 3,
    GPIO_PIN_RA4 = 4,
    GPIO_PIN_RA5 = 5,
    GPIO_PIN_RA6 = 6,
    GPIO_PIN_RA7 = 7,
    GPIO_PIN_RA9 = 9,
    GPIO_PIN_RA10 = 10,
    GPIO_PIN_RA14 = 14,
    GPIO_PIN_RA15 = 15,
    GPIO_PIN_RB0 = 16,
    GPIO_PIN_RB1 = 17,
    GPIO_PIN_RB2 = 18,
    GPIO_PIN_RB3 = 19,
    GPIO_PIN_RB4 = 20,
    GPIO_PIN_RB5 = 21,
    GPIO_PIN_RB6 = 22,
    GPIO_PIN_RB7 = 23,
    GPIO_PIN_RB8 = 24,
    GPIO_PIN_RB9 = 25,
    GPIO_PIN_RB10 = 26,
    GPIO_PIN_RB11 = 27,
    GPIO_PIN_RB12 = 28,
    GPIO_PIN_RB13 = 29,
    GPIO_PIN_RB14 = 30,
    GPIO_PIN_RB15 = 31,
    GPIO_PIN_RC1 = 33,
    GPIO_PIN_RC2 = 34,
    GPIO_PIN_RC3 = 35,
    GPIO_PIN_RC4 = 36,
    GPIO_PIN_RC12 = 44,
    GPIO_PIN_RC13 = 45,
    GPIO_PIN_RC14 = 46,
    GPIO_PIN_RC15 = 47,
    GPIO_PIN_RD0 = 48,
    GPIO_PIN_RD1 = 49,
    GPIO_PIN_RD2 = 50,
    GPIO_PIN_RD3 = 51,
    GPIO_PIN_RD4 = 52,
    GPIO_PIN_RD5 = 53,
    GPIO_PIN_RD6 = 54,
    GPIO_PIN_RD7 = 55,
    GPIO_PIN_RD8 = 56,
    GPIO_PIN_RD9 = 57,
    GPIO_PIN_RD10 = 58,
    GPIO_PIN_RD11 = 59,
    GPIO_PIN_RD12 = 60,
    GPIO_PIN_RD13 = 61,
    GPIO_PIN_RD14 = 62,
    GPIO_PIN_RD15 = 63,
    GPIO_PIN_RE0 = 64,
    GPIO_PIN_RE1 = 65,
    GPIO_PIN_RE2 = 66,
    GPIO_PIN_RE3 = 67,
    GPIO_PIN_RE4 = 68,
    GPIO_PIN_RE5 = 69,
    GPIO_PIN_RE6 = 70,
    GPIO_PIN_RE7 = 71,
    GPIO_PIN_RE8 = 72,
    GPIO_PIN_RE9 = 73,
    GPIO_PIN_RF0 = 80,
    GPIO_PIN_RF1 = 81,
    GPIO_PIN_RF2 = 82,
    GPIO_PIN_RF3 = 83,
    GPIO_PIN_RF4 = 84,
    GPIO_PIN_RF5 = 85,
    GPIO_PIN_RF8 = 88,
    GPIO_PIN_RF12 = 92,
    GPIO_PIN_RF13 = 93,
    GPIO_PIN_RG0 = 96,
    GPIO_PIN_RG1 = 97,
    GPIO_PIN_RG2 = 98,
    GPIO_PIN_RG3 = 99,
    GPIO_PIN_RG6 = 102,
    GPIO_PIN_RG7 = 103,
    GPIO_PIN_RG8 = 104,
    GPIO_PIN_RG9 = 105,
    GPIO_PIN_RG12 = 108,
    GPIO_PIN_RG13 = 109,
    GPIO_PIN_RG14 = 110,
    GPIO_PIN_RG15 = 111,

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
    GPIO_PIN_NONE = -1

} GPIO_PIN;

typedef enum
{
  CN0_PIN = 1 << 0,
  CN1_PIN = 1 << 1,
  CN2_PIN = 1 << 2,
  CN3_PIN = 1 << 3,
  CN4_PIN = 1 << 4,
  CN5_PIN = 1 << 5,
  CN6_PIN = 1 << 6,
  CN7_PIN = 1 << 7,
  CN8_PIN = 1 << 8,
  CN9_PIN = 1 << 9,
  CN10_PIN = 1 << 10,
  CN11_PIN = 1 << 11,
  CN12_PIN = 1 << 12,
  CN13_PIN = 1 << 13,
  CN14_PIN = 1 << 14,
  CN15_PIN = 1 << 15,
  CN16_PIN = 1 << 16,
  CN17_PIN = 1 << 17,
  CN18_PIN = 1 << 18,
  CN19_PIN = 1 << 19,
  CN20_PIN = 1 << 20,
  CN21_PIN = 1 << 21,
}CN_PIN;


void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(pin>>4), (uint32_t)(0x1) << (pin & 0xF), (uint32_t)(value) << (pin & 0xF));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(pin>>4))) >> (pin & 0xF)) & 0x1);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(pin>>4)) >> (pin & 0xF)) & 0x1);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
