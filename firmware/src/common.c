#include <stdio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "definitions.h"
#include "tcpip/tcpip.h"
#include "system/fs/sys_fs.h"
#include "system/debug/sys_debug.h"
#include "wolfcrypt/hash.h"

#define NTP_TIMESTAMP_DIFF     (2208988800)    // 1900 to 1970 in seconds

int time_zone = -120;

static volatile uint32_t milliseconds = 0;
static volatile uint32_t upt = 0;

SYS_FS_RESULT FormatSpiFlashDisk (void) {
    SYS_FS_RESULT res;
    SYS_FS_FORMAT_PARAM opt;
    res = SYS_FS_DriveFormat("/dev/mtda1", &opt, NULL, 0);  //TO DO POPRAWY
    if (res == SYS_FS_RES_FAILURE) {SYS_CONSOLE_PRINT("SYS_FS_DriveFormat error code: %i\r\n", res);}
    else {SYS_CONSOLE_PRINT("SYS_FS_DriveFormat OK\r\n");}
    return res;
}


unsigned char BcdToByte(unsigned char bcd) {
    return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

unsigned char ByteToBcd(unsigned char i) {
    return ((i / 10) << 4) + (i % 10);
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

time_t time(time_t *timer) {
    time_t timestamp = TCPIP_SNTP_UTCSecondsGet();
    if (timer) {
        *timer = timestamp;
    }
    return timestamp;
}

void timer_callback(uint32_t status, uintptr_t context) {
    milliseconds++;
    if ( (milliseconds%1000) == 0 ) {
        upt++;
    }
}

uint32_t millis(void) {
    return milliseconds;
}

uint32_t uptime(void) {
    return upt;
}

uint32_t get_fattime (void) {
    time_t rawtime;
    struct tm* current_time;
    
    rawtime = time(NULL);
    current_time = localtime(&rawtime);
    return ((DWORD)(current_time->tm_year - 80) << 25)
         | ((DWORD)(current_time->tm_mon + 1) << 21)
         | ((DWORD)current_time->tm_mday << 16)
         | ((DWORD)current_time->tm_hour << 11)
         | ((DWORD)current_time->tm_min << 5)
         | ((DWORD)current_time->tm_sec >> 1);
//    return 0xFFFFFFFF;
}
