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

static uint16_t max_stream_id = 0;

static volatile uint32_t milliseconds = 0;
static volatile uint32_t upt = 0;
char working_buffer[512];

static char* find_station_in_file(SYS_FS_HANDLE file, uint16_t number, char* stream_name, size_t stream_name_len);

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

void initialize_stream_list(void) {
    SYS_FS_HANDLE file;
    uint16_t number = 0;
    
    file = SYS_FS_FileOpen(STREAM_LIST_PATH, SYS_FS_FILE_OPEN_READ);
    if (file == SYS_FS_HANDLE_INVALID) {
        SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Get station url: Can't open file\r\n");
        return;
    }
    while (SYS_FS_FileStringGet(file, working_buffer, sizeof(working_buffer)) == SYS_FS_RES_SUCCESS) {
        if (working_buffer[strlen(working_buffer)] == '\n') {
            working_buffer[strlen(working_buffer)] = '\0';
        }
        int ret = parse_stream_data_line(working_buffer, strlen(working_buffer), NULL, 0, NULL, 0);
        if (ret > number) {
            number = ret;
        }
    }
    if (number > 0) {
        max_stream_id = number;
    }
    SYS_FS_FileClose(file);
}

uint16_t get_max_stream_id(void) {
    return max_stream_id;
}

/*WARNING: To preserve precious memory on that hardware this function uses the same working buffer as
 parse_http_headers(). Be sure not to use both of them at the same time. Remember that
 parse_http_headers() is baing called multiple times by state machine.*/
char* get_station_url_from_file(uint16_t number, char* stream_name, size_t stream_name_len) {
    return get_station_url_from_file_use_seek(number, stream_name, stream_name_len, NULL);
}

char* get_station_url_from_file_use_seek(uint16_t number, char* stream_name, size_t stream_name_len, int32_t* cur_pos) {
    SYS_FS_HANDLE file;
    char* result = NULL;
    
    file = SYS_FS_FileOpen("/mnt/myDrive1/radio.txt", SYS_FS_FILE_OPEN_READ);
    if (file == SYS_FS_HANDLE_INVALID) {
        SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Get station url: Can't open file\r\n");
        return NULL;
    }
    
    if (cur_pos && (*cur_pos > 0)) {
        int32_t seek_result = SYS_FS_FileSeek(file, *cur_pos, SYS_FS_SEEK_SET);
        if (seek_result == -1) {
            SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Get station url: Can't seek to provided value\r\n");
            return NULL;
        }
    }
    
    result = find_station_in_file(file, number, stream_name, stream_name_len);
    if (cur_pos && (*cur_pos > 0) && (result == NULL)) {
        // It is possible stream is located somewhere earlier in file
        // Look again, just i case if we missed it
        int32_t seek_result = SYS_FS_FileSeek(file, 0, SYS_FS_SEEK_SET);
        if (seek_result == -1) {
            SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Get station url: Can't seek to provided value\r\n");
            return NULL;
        }
        result = find_station_in_file(file, number, stream_name, stream_name_len);
    }
    
    if (cur_pos) {
        int32_t tmp = SYS_FS_FileTell(file);
        if (tmp > 0) { *cur_pos = tmp; }
    }
    SYS_FS_FileClose(file);
    return result;
}

char* find_station_in_file(SYS_FS_HANDLE file, uint16_t number, char* stream_name, size_t stream_name_len) {
	char* result = NULL;
    while (SYS_FS_FileStringGet(file, working_buffer, sizeof(working_buffer)) == SYS_FS_RES_SUCCESS) {
        if (working_buffer[strlen(working_buffer)-1] == '\n') {
            working_buffer[strlen(working_buffer)-1] = '\0';
        }
        int ret = parse_stream_data_line(working_buffer, strlen(working_buffer), stream_name, stream_name_len, working_buffer, sizeof(working_buffer));
        if (ret && ret == number) {
            result = working_buffer;
            break;
        }
    }
    return result;
}

uint8_t parse_stream_data_line(char* line, size_t line_len, char* stream_name, size_t stream_name_len, char* stream_url, size_t stream_url_len) {
    int line_number = atoi(line);
    
    if (line_number > 0) {
        char* rest = strstr(line, " : ");
        if (rest) {
            *rest = '\0';
            rest += 3;
            if (rest >= line+line_len) { return 0; }
            char* url = strstr(rest, " : ");
            if (url) {
                *url = '\0';
                url += 3;
                if (url >= line+line_len) { return 0; }
                if (stream_name) {
                    strncpy(stream_name, rest, stream_name_len);
                }
                if (stream_url) {
                    strncpy(stream_url, url, stream_url_len);
                }
                return line_number;
            }
        }
    }
    return 0;    
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
