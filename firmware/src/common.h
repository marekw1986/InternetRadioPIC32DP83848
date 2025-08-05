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
char* get_file_path_from_media_dir_id(uint16_t number, char* working_buffer, size_t working_buffer_len, char* name, size_t name_len);
char* get_file_path_from_media_dir_id_is_dir(uint16_t number, char* working_buffer, size_t working_buffer_len, char* name, size_t name_len, uint8_t* is_dir);
void to_lower(char *str);
uint8_t is_audio_file (char* name);
void set_media_dir_path(const char* dir_path);
char* get_media_dir_path(void);
uint16_t get_number_of_items_in_media_dir();
uint16_t count_dirs_and_audio_files_in_media_dir(void);
uint8_t dir_contains_audio_files(const char* path);
#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */
