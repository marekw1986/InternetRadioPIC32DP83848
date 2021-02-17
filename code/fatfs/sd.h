#ifndef SD_H
#define	SD_H

#define FAIL    0
// Init ERROR code definitions
#define E_COMMAND_ACK     0x80
#define E_INIT_TIMEOUT    0x81


extern unsigned long sdsread, sd_bytesread;
extern char sd_led;

typedef unsigned long LBA;     // logic block address, 32 bit wide


int sd_init( void);     // initializes the SD/MMC memory device
int sd_getCD();              // chech card presence
int getWP();              // check write protection tab
int sd_readSECTOR ( LBA, char *); // reads a block of data
int sd_writeSECTOR( LBA, const char *); // writes a block of data

#define SD_CS_TRIS TRISBbits.TRISB15
#define SD_CS LATBbits.LATB15
#define SD_CD_TRIS TRISFbits.TRISF3 // card detect
#define SD_CD PORTFbits.RF3
#define SD_SPICON SPI4CON
#define SD_SPIBRG SPI4BRG
#define SD_SPIBRGINITVAL 142   // clock = Fpb/144 = 250kHz max
#define SD_SPIBRGRUNVAL 1    ///Fpb/(2*(SD_SPIBRG+1))=  Hz
#define SD_SPIBUF SPI4BUF
#define SD_BUFNSENT !SPI4STATbits.SPIRBF


#endif /* SD_H */