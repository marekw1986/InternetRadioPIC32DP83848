/* 
 * File:   HardwareProfile.h
 * Author: Marek
 *
 * Created on 7 stycznia 2016, 18:25
 */

#ifndef HARDWAREPROFILE_H
#define	HARDWAREPROFILE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define SystemClock             (80000000ul)

#define GetSystemClock()        SystemClock
#define GetInstructionClock()   SystemClock
#define GetPeripheralClock()    (SystemClock/(1<<OSCCONbits.PBDIV))

#define CP0_TICK_HZ             (SystemClock / 2)
    
//#define USE_SELF_POWER_SENSE_IO
//#define tris_self_power     TRISAbits.TRISA2    // Input
#define self_power          1

//#define USE_USB_BUS_SENSE_IO
//#define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
//#define USB_BUS_SENSE       1 
    
#define PHY_RMII
#define PHY_ADDRESS 0x01

#define SPIFLASH_CS_TRIS		(TRISAbits.TRISA1)
#define SPIFLASH_CS_IO			(LATAbits.LATA1)
#define SPIFLASH_SCK_TRIS		(TRISFbits.TRISF13)
#define SPIFLASH_SDI_TRIS		(TRISFbits.TRISF4)
#define SPIFLASH_SDI_IO			(PORTFbits.RF4)
#define SPIFLASH_SDO_TRIS		(TRISFbits.TRISF5)
#define SPIFLASH_SPI_IF			(IFS1bits.SPI4RXIF)
#define SPIFLASH_SSPBUF			(SPI4BUF)
#define SPIFLASH_SPICON1		(SPI4CON)
#define SPIFLASH_SPICON1bits	(SPI4CONbits)
#define SPIFLASH_SPISTATbits	(SPI4STATbits)
#define SPIFLASH_SPIBRG			(SPI4BRG)
    
// Sector size of SPI Flash required by MSD client library    
#define MEDIA_SECTOR_SIZE 4096
#define MSD_INTERNAL_FLASH_TOTAL_DISK_SIZE 488




#ifdef	__cplusplus
}
#endif

#endif	/* HARDWAREPROFILE_H */

