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
//#include "wolfcrypt/hash.h"

#define NTP_TIMESTAMP_DIFF     (2208988800)    // 1900 to 1970 in seconds

int time_zone = -120;

static volatile uint32_t milliseconds = 0;
static volatile uint32_t upt = 0;

static char media_dir_path[512];
static uint16_t number_of_items_in_media_dir = 0;

SYS_FS_RESULT FormatSpiFlashDisk (void) {
    SYS_FS_RESULT res;
    SYS_FS_FORMAT_PARAM opt;
    res = SYS_FS_DriveFormat("/dev/mtda1", &opt, NULL, 0);  //TO DO POPRAWY
    if (res == SYS_FS_RES_FAILURE) {SYS_CONSOLE_PRINT("SYS_FS_DriveFormat error code: %i\r\n", res);}
    else {SYS_CONSOLE_PRINT("SYS_FS_DriveFormat OK\r\n");}
    return res;
}

void AppDP83848ResetFunction(const struct DRV_ETHPHY_OBJECT_BASE_TYPE* pBaseObj, DRV_HANDLE handle) {
    SYS_CONSOLE_PRINT("Reseting PHY\r\n");
    DP_RST_Clear();
    CORETIMER_DelayUs(10);
    DP_RST_Set();
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

//char* get_file_path_from_media_dir_id(uint16_t number, char* working_buffer, size_t working_buffer_len, char* stream_name, size_t stream_name_len) {
//	char* res = NULL;
//	switch(number) {
//		case 1:
//		strncpy(stream_name, "Queen - Kind of Magic.mp3", stream_name_len);
//		res = "jeden";
//		break;
//		
//		case 2:
//		strncpy(stream_name, "Creed - My Sacrifice.mp3", stream_name_len);
//		res = "dwa";
//		break;
//		
//		case 3:
//		strncpy(stream_name, "The Cranberries - Zombie.mp3", stream_name_len);
//		res = "trzy";
//		break;
//		
//		case 4:
//		strncpy(stream_name, "Pixies - Where is my mind.aac", stream_name_len);
//		res = "cztery";
//		break;
//		
//		case 5:
//		strncpy(stream_name, "Frank Turner - Ballad of me and my friends.mp3", stream_name_len);
//		res = "piec";
//		break;
//		
//		case 6:
//		strncpy(stream_name, "Czeslaw Niemen - Dziwny jest ten swiat.mp3", stream_name_len);
//		res = "szesc";
//		break;
//		
//		case 7:
//		strncpy(stream_name, "Against Me! - I was a teenage anarchist.mp3", stream_name_len);
//		res = "siedem";
//		break;
//		
//		case 8:
//		strncpy(stream_name, "Phantom Planet - California.mp3", stream_name_len);
//		res = "osiem";
//		break;
//		
//		case 9:
//		strncpy(stream_name, "SAMURAI - Never fade away.mp3", stream_name_len);
//		res = "dziewiec";
//		break;
//		
//		case 10:
//		strncpy(stream_name, "Lifehouse - Whatever it takes.mp3", stream_name_len);
//		res = "dziesiec";
//		break;
//		
//		case 11:
//		strncpy(stream_name, "Pearl Jam - Once.mp3", stream_name_len);
//		res = "jedenascie";
//		break;
//		
//		case 12:
//		strncpy(stream_name, "Bad Religion - Sorrow.mp3", stream_name_len);
//		res = "dwanascie";
//		break;										
//		
//		default:
//		strncpy(stream_name, "", stream_name_len);
//		res = NULL;
//		break;
//	}
//	return res;
//}

char* get_file_path_from_media_dir_id(uint16_t number, char* working_buffer, size_t working_buffer_len, char* name, size_t name_len) {
    return get_file_path_from_media_dir_id_is_dir(number, working_buffer, working_buffer_len, name, name_len, NULL);
}

char* get_file_path_from_media_dir_id_is_dir(
    uint16_t number,
    char* working_buffer,
    size_t working_buffer_len,
    char* name,
    size_t name_len,
    uint8_t* is_dir
) {
    SYS_FS_HANDLE dirHandle;
    SYS_FS_FSTAT dirEntry;
    uint16_t count = 0;

    dirHandle = SYS_FS_DirOpen(media_dir_path);
    if (dirHandle == SYS_FS_HANDLE_INVALID) {
        SYS_CONSOLE_PRINT("Error opening directory: %s\n", media_dir_path);
        return NULL;
    }

    while (SYS_FS_DirRead(dirHandle, &dirEntry) == SYS_FS_RES_SUCCESS) {
        // Skip invalid or empty entries
        if (dirEntry.fname[0] == '\0') {
            continue;
        }

        if ((dirEntry.fattrib & SYS_FS_ATTR_DIR) || is_audio_file(dirEntry.fname)) {
            count++;
        }

        if (count == number) {
            SYS_FS_DirClose(dirHandle);

            // Copy the display name
            strncpy(name, dirEntry.fname, name_len);
            name[name_len - 1] = '\0';  // Ensure null-termination
            snprintf(working_buffer, working_buffer_len, "%s/%s", media_dir_path, dirEntry.altname);

            if (is_dir != NULL) {
                *is_dir = (dirEntry.fattrib & SYS_FS_ATTR_DIR);
            }

            return working_buffer;
        }
    }

    SYS_FS_DirClose(dirHandle);
    return NULL;
}

// Convert string to lowercase for case-insensitive comparison
void to_lower(char *str) {
    while (*str) {
        *str = (char)tolower((unsigned char)*str);
        str++;
    }
}

uint8_t is_audio_file (char* name) {
    const char* ext = strrchr(name, '.');
    if (!ext) { return 0; }
    char ext_lower[5];
    strncpy(ext_lower, ext, sizeof(ext_lower));
    ext_lower[4] = '\0';
    to_lower(ext_lower);
    return (strstr(name, ".mp3") || strstr(name, ".aac") || strstr(name, ".flac") ||strstr(name, ".wma") || strstr(name, ".mid"));
 }

void set_media_dir_path(const char* dir_path) {
    strncpy(media_dir_path, dir_path, sizeof(media_dir_path));
    number_of_items_in_media_dir = count_dirs_and_audio_files_in_media_dir();
}

char* get_media_dir_path(void) {
    return media_dir_path;
}

uint16_t get_number_of_items_in_media_dir() {
    return number_of_items_in_media_dir;
}

uint16_t count_dirs_and_audio_files_in_media_dir(void) {
    SYS_FS_HANDLE dirHandle;
    SYS_FS_FSTAT dirEntry;
    uint16_t count = 0;

    dirHandle = SYS_FS_DirOpen(media_dir_path);
    if (dirHandle == SYS_FS_HANDLE_INVALID) {
        SYS_CONSOLE_PRINT("Error opening directory: %s\n", media_dir_path);
        return 0;
    }

    while (SYS_FS_DirRead(dirHandle, &dirEntry) == SYS_FS_RES_SUCCESS) {
        // Check for directory
        if (dirEntry.fattrib & SYS_FS_ATTR_DIR) {
            count++;
            continue;
        }

        // Check if it's an audio file
        if (is_audio_file(dirEntry.fname)) {
            count++;
        }
    }

    SYS_FS_DirClose(dirHandle);
    return count;
}