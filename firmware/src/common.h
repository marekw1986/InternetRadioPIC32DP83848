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

#define URI_SERVER_SIZE 256
#define URI_FILE_SIZE 256

typedef struct {
	char server[URI_SERVER_SIZE+1];
	char file[URI_FILE_SIZE+1];
	uint8_t https;
	uint16_t port;
} uri_t;

typedef enum {HTTP_HEADER_ERROR = 0, HTTP_HEADER_IN_PROGRESS, HTTP_HEADER_OK, HTTP_HEADER_REDIRECTED} http_res_t;


#ifdef	__cplusplus
extern "C" {
#endif

SYS_FS_RESULT FormatSpiFlashDisk (void);
unsigned char BcdToByte(unsigned char bcd);
unsigned char ByteToBcd(unsigned char i);
//unsigned char StringToMACAddress(BYTE* str, MAC_ADDR* MACAddress);
long map(long x, long in_min, long in_max, long out_min, long out_max);
void prepare_http_parser(void);
http_res_t parse_http_headers(char* str, size_t len, uri_t* uri);
uint8_t parse_url (const char* str, size_t len, uri_t* uri);
void initialize_stream_list(void);
uint16_t get_max_stream_id(void);
char* get_station_url_from_file(uint16_t number, char* stream_name, size_t stream_name_len);
uint8_t parse_stream_data_line(char* line, size_t line_len, char* stream_name, size_t stream_name_len, char* stream_url, size_t stream_url_len);
uint32_t millis(void);
uint32_t uptime(void);
#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */
