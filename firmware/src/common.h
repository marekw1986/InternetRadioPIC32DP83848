/* 
 * File:   common.h
 * Author: Marek
 *
 * Created on 20 grudnia 2015, 22:10
 */

#ifndef COMMON_H
#define	COMMON_H

#include <stdint.h>
#include "system/fs/sys_fs.h"
#include "FreeRTOS.h"
#include "task.h"

//#define millis() xTaskGetTickCount()

typedef struct {
	uint16_t year;	/* 2000..2099 */
	uint8_t month;	/* 1..12 */
	uint8_t	mday;	/* 1.. 31 */
	uint8_t	wday;	/* 1..7 */
	uint8_t	hour;	/* 0..23 */
	uint8_t	min;	/* 0..59 */
	uint8_t	sec;	/* 0..59 */
} RTC;

#ifdef	__cplusplus
extern "C" {
#endif

SYS_FS_RESULT FormatSpiFlashDisk (void);
unsigned char BcdToByte(unsigned char bcd);
unsigned char ByteToBcd(unsigned char i);
//unsigned char StringToMACAddress(BYTE* str, MAC_ADDR* MACAddress);
long map(long x, long in_min, long in_max, long out_min, long out_max);
uint32_t millis(void);
uint32_t uptime(void);
char* get_file_path_from_audio_file_id(uint16_t number, char* working_buffer, size_t working_buffer_len, char* stream_name, size_t stream_name_len);
void to_lower(char *str);
uint8_t is_audio_file (char* name);
uint16_t count_dirs_and_audio_files_in_dir(char* dir_path);
#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */
