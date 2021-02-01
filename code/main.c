/* 
 * File:   main.c
 * Author: Marek
 *
 * Created on 7 stycznia 2016, 12:22
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <p32xxxx.h>
#include <plib.h>
#include <time.h>
#include <string.h>

#include "usb/usb.h"
#include "usb/usb_host_msd.h"
#include "usb/usb_host_msd_scsi.h"
#include "fatfs/ff.h"
#include "fatfs/diskio.h"
#include "delay/delay.h"
#include "time/time.h"
#include "uart/uart.h"
#include "common.h"
#include "net/TCPIP.h"
#include "vs1003/vs1003.h"
#include "nvram/nvram.h"
#include "lcd/hd44780.h"
#include "lcd/i2c.h"
#include "btn/buttons.h"
#include "HardwareProfile.h"
#include "btn/rotary.h"


#pragma config FSOSCEN = ON        // ENABLE secondary oscillator

#pragma config POSCMOD=EC          // Primary Oscillator Configuration (External clock mode)
#pragma config OSCIOFNC = OFF      // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FNOSC=PRIPLL        // Use Primary Oscillator with PLL (XT, HS, or EC) 
#pragma config FPLLIDIV=DIV_2      // Divide 8MHz to between 4-5MHz before PLL (now 4MHz)
#pragma config FPLLMUL=MUL_20      // Multiply with PLL (now 80MHz)
#pragma config FPLLODIV=DIV_1      // Divide After PLL (now 80 MHz)

#pragma config FWDTEN = ON         // ENABLE watchdog timer
#pragma config WDTPS = PS4096
#pragma config ICESEL = ICS_PGx1   // select pins to transfer program data on (ICSP pins)

#pragma config FMIIEN = OFF        // Ethernet RMII/MII Enable (RMII Enabled)
#pragma config FETHIO = ON         // Ethernet I/O Pin Select (Default Ethernet I/O)
#pragma config FVBUSONIO = OFF
#pragma config FUSBIDIO = OFF
#pragma config UPLLEN   = ON
#pragma config UPLLIDIV = DIV_2

uint8_t buffer[512];
FATFS FatFS;
FRESULT res;
FIL fsrc;               // file objects
UINT br;         		// File R/W count
button_t prev_btn, next_btn, rot_btn;


extern void GenericTCPServer(void);
static inline void init_periph (void);
void usb_write (void);
void prev_func (void);
void next_func (void);
void rot_func (void);
void prev_hold_func (void);
void next_hold_func (void);
void rot_hold_func (void);

void Next_Track(FIL* File, DIR* Directory, FILINFO* File_Info);
void Send_Playback_Time();
void Send_Bitrate();
void Send_Samplerate();
void Send_Volume();
uint8_t IsPlayable(char* File_Name);

int main(int argc, char** argv) {
    
    int i;
    uint8_t counter = 0;
    int8_t tmp;
    
    uint32_t wdt_timer, usb_timer, vs_timer;
    
    DisableWDT();
    SYSTEMConfigPerformance(SYSCLK);
    init_periph();
    
    memset(buffer, 0x00, sizeof(buffer));
 
    time_init();
    MPFSInit();
    TickInit();
    uart_init();
    INTEnableSystemMultiVectoredInt();
    
    printf ("Start\r\n");
    
    USBInitialize(0);
//    while(!USBHostMSDSCSIMediaDetect()) {
//        USBTasks(); //wait   for usb attach
//        //tutuaj trzeba zaimplentowac timout oczekiwania na mediadetect()
//    }
    
    //FormatSpiFlashDisk(); 
    
    CheckAndLoadDefaults();
    
    delay_ms(500);  //TEST
    
    res = f_mount(&FatFS, "2:", 1);
    if (res != FR_OK) {printf("f_mount error code: %i\r\n", res);}
    else {printf("f_mount OK\r\n");}
    
    res = f_open(&fsrc, "2:/test.mp3", FA_READ);
    if (res != FR_OK) {printf("f_open error code: %i\r\n", res);}
    else {printf("f_open OK\r\n");}
    
    StackInit();
    
    rotary_init();
    button_init(&prev_btn, &PORTE, _PORTE_RE2_MASK, &prev_func, &prev_hold_func);
    button_init(&next_btn, &PORTG, _PORTG_RG13_MASK, &next_func, &next_hold_func);
    button_init(&rot_btn, &PORTA, _PORTA_RA15_MASK, &rot_func, &rot_hold_func);
    i2c_init();
    //lcd_init();
    //lcd_cls();
    //lcd_str("Test wyswietlacza");
    //VS1003_begin();
    //VS1003_setVolume(0x00);
    //VS1003_stopSong();
    //VS1003_startSong();
    
    ClearWDT();
    EnableWDT();
    
    while (1) {
        
        StackTask();
        StackApplications();
        UART_RX_STR_EVENT(buffer);
        GenericTCPServer();
        
        USBTasks();
        disk_timerproc();
        
        button_handle(&prev_btn);
        button_handle(&next_btn);
        button_handle(&rot_btn);
        
        if (tmp = rotary_handle()) {
            counter += tmp;
            printf("Rotary: %d\r\n", counter);
        }
        
        if ((uint32_t)(millis()-wdt_timer) >= 300) {
            wdt_timer = millis();
            ClearWDT();
        }
        
        if ((uint32_t)(millis()-usb_timer) >= 5000) {
            usb_timer = millis();
            //usb_write();
            i2c_rcv_byte(0x20);
            printf("Test\r\n");
        }
        
        /*
        if (VS_DREQ_PIN) {
            res = f_read(&fsrc, buffer, sizeof(buffer), &br);
            if(res == FR_OK) {
                VS1003_playChunk(buffer, br);
            }
            //End of file
            if(br == 0) {
                VS1003_stopSong();
                VS1003_startSong();
                res = f_lseek(&fsrc, 0);
                if (res != FR_OK) printf("f_lseek ERROR\r\n");
                else printf("f_lseek OK\r\n");
            }
        }
        */
        
    }

    return (EXIT_SUCCESS);
}


static inline void init_periph (void) {
    DDPCON = DDPCON & 0xFFFFFFF7;   // Disable JTAG
    AD1PCFG = 0xFFFF;               //All ANx inputs disabled, digitl mode only
    TRISCbits.TRISC1 = 1;           //VS_DREQ is input
    TRISCbits.TRISC2 = 0;           //VS_XRST is output
    LATCbits.LATC2 = 1;             //VS_XRST is high (it is active high)
    TRISCbits.TRISC4 = 1;           //VS_MISO is input
    TRISEbits.TRISE6 = 0;           //VS_XDCS is output
    LATEbits.LATE6 = 1;             //VS_XDCS is high
    TRISEbits.TRISE7 = 0;           //VS_XCS is output
    LATEbits.LATE7 = 1;             //VS_XCS is high
    TRISDbits.TRISD0 = 0;           //VS_MOSI is output
    TRISDbits.TRISD10 = 0;          //VS_SCK is output
    TRISDbits.TRISD14 = 1;          //RXD is input
    TRISDbits.TRISD15 = 0;          //TXD is output
    TRISFbits.TRISF3 = 1;           //SD_PRESENT is input
    TRISBbits.TRISB15 = 0;           //SD_CS is output
    LATBbits.LATB15 = 1;             //SD_CS is high
    TRISFbits.TRISF4 = 1;           //MISO is input
    TRISFbits.TRISF5 = 0;           //MOSI is output
    TRISAbits.TRISA1 = 0;           //FLASH_CS is output
    LATAbits.LATA1 = 1;             //FLASH_CS is high
    TRISFbits.TRISF13 = 0;          //SCK is output
    TRISFbits.TRISF8 = 0;           //SCL is output
    TRISFbits.TRISF2 = 0;           //SDA is output
    
    TRISDbits.TRISD9 = 1;           //Rotary CLK is input
    TRISDbits.TRISD1 = 1;           //Rotary DT is input
    TRISAbits.TRISA15 = 1;          //Rotary dial switch is input
    TRISGbits.TRISG13 = 1;          //NEXT button is input
    TRISEbits.TRISE2 = 1;           //PREV button is input
}


void usb_write (void) { 
    FRESULT res;
    FIL file;

    res = f_open(&file, "2:/test.txt", (FA_OPEN_ALWAYS | FA_WRITE));
    if (res != FR_OK) {
        printf("f_open error code: %i\r\n", res);
        return;
    }
    if (f_size(&file) == 0) {
        f_puts("To jest pierwsza linia\r\n", &file);
    }
    else {
        res = f_lseek(&file, f_size(&file));
        if (res != FR_OK) {
            printf("f_lseek error code: %i\r\n", res); 
            f_close(&file);
            return;
        }
    }
    f_puts("test\r\n", &file);
    f_close(&file); 
}

void prev_func (void) {
    printf("PREV button pressed!\r\n");
}


void next_func (void) {
    printf("NEXT button pressed!\r\n");
}


void rot_func (void) {
    printf("ROTARY DIAL pressed!\r\n");
}


void prev_hold_func (void) {
    printf("PREV button HELD!\r\n");
}


void next_hold_func (void) {
    printf("NEXT button HELD!\r\n");
}


void rot_hold_func (void) {
    printf("ROTARY DIAL HELD!\r\n");
}

static enum {
      EXCEP_IRQ = 0,            // interrupt
      EXCEP_AdEL = 4,            // address error exception (load or ifetch)
      EXCEP_AdES,                // address error exception (store)
      EXCEP_IBE,                // bus error (ifetch)
      EXCEP_DBE,                // bus error (load/store)
      EXCEP_Sys,                // syscall
      EXCEP_Bp,                // breakpoint
      EXCEP_RI,                // reserved instruction
      EXCEP_CpU,                // coprocessor unusable
      EXCEP_Overflow,            // arithmetic overflow
      EXCEP_Trap,                // trap (possible divide by zero)
      EXCEP_IS1 = 16,            // implementation specfic 1
      EXCEP_CEU,                // CorExtend Unuseable
      EXCEP_C2E                // coprocessor 2
  } _excep_code;


static unsigned int _epc_code;
static unsigned int _excep_addr;



void _general_exception_handler(void) {
    int i=0;
    asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
    asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));
  
    _excep_code = (_excep_code & 0x0000007C) >> 2;
 

	 printf("\r\n\r\n GE at %x ",_excep_addr);
	 switch(_excep_code){
        case EXCEP_IRQ: printf("interrupt");break;
        case EXCEP_AdEL: printf("address error exception (load or ifetch)");break;
        case EXCEP_AdES: printf("address error exception (store)");break;
        case EXCEP_IBE: printf("(ifetch)");break; //bus error
        case EXCEP_DBE: printf("(load/store)");break; //bus error
        case EXCEP_Sys: printf("syscall");break;
        case EXCEP_Bp: printf("breakpoint");break;
        case EXCEP_RI: printf("reserved instruction");break;
        case EXCEP_CpU: printf("coprocessor unusable");break;
        case EXCEP_Overflow: printf("arithmetic overflow");break;
        case EXCEP_Trap: printf("trap (possible divide by zero)");break;
        case EXCEP_IS1: printf("implementation specfic 1");break;
        case EXCEP_CEU: printf("CorExtend Unuseable");break;
        case EXCEP_C2E: printf("coprocessor 2");break;
      }
	  printf("\r\nExecution halted.\r\n");
      while (1) {
		printf("Waiting for WTD reset %lu\r\n",i++);
		delay_ms(1000);
      }
}

