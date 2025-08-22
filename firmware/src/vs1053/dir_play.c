#include "dir_play.h"
#include "definitions.h"
#include "../common.h"
#include "../http_header_parser.h"
#include "vs1053.h"

extern uri_t uri;
extern bool dir_flag;
extern bool loop_flag;

static uint16_t dir_index = 0;
static uint16_t dir_count = 0;

static char* get_path_of_next_audio_file_in_currently_played_dir(void);

void VS1053_play_dir_alt (const char* path) {
    dir_count = count_audio_files_in_dir(path);
    if (dir_count == 0) { return; }
    dir_index = 1;
    dir_flag = true;    // TODO: This should be replaced with dir_index alone
    strncpy(uri.server, path, sizeof(uri.server)-1);		//we use uri.server to store current directory path
    int ret = snprintf(uri.server, sizeof(uri.server), "%s", path);   //we use uri.server to store current directory path
    if (ret <0 || ret >= sizeof(uri.server)) {
//        SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Play dir: uri.server does not fit directory path\r\n");
        return;
    }
    char* file_path = get_path_of_next_audio_file_in_currently_played_dir();
    if (file_path == NULL) { return; }
    VS1053_play_file(file_path);
}

uint16_t count_audio_files_in_dir(const char* path) {
    SYS_FS_HANDLE dirHandle;
    SYS_FS_FSTAT dirEntry;
    uint16_t count = 0;
    
    dirEntry.lfname = NULL;
    dirEntry.lfsize = 0;

    dirHandle = SYS_FS_DirOpen(path);
    if (dirHandle == SYS_FS_HANDLE_INVALID) {
        SYS_CONSOLE_PRINT("Error opening directory: %s\n", path);
        return 0;
    }

    while (SYS_FS_DirRead(dirHandle, &dirEntry) == SYS_FS_RES_SUCCESS) {
        if (!dirEntry.fname[0]) { // empty string, end of directory
            break;
        }
        // Check for directory
        if (dirEntry.fattrib & SYS_FS_ATTR_DIR) {
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

static char* get_path_of_next_audio_file_in_currently_played_dir(void) {
    SYS_FS_HANDLE dirHandle;
    SYS_FS_FSTAT dirEntry;
    uint16_t count = 0;
    
    dirEntry.lfname = NULL;
    dirEntry.lfsize = 0;

    dirHandle = SYS_FS_DirOpen(uri.server);     // We use uri.server to store path for currently played dir
    if (dirHandle == SYS_FS_HANDLE_INVALID) {
        SYS_CONSOLE_PRINT("Error opening directory: %s\n", uri.server);
        return NULL;
    }

    while (SYS_FS_DirRead(dirHandle, &dirEntry) == SYS_FS_RES_SUCCESS) {
        if (!dirEntry.fname[0]) { // empty string, end of directory
            break;
        }
        // Check for directory
        if (dirEntry.fattrib & SYS_FS_ATTR_DIR) {
            continue;
        }
        // Check if it's an audio file
        if (is_audio_file(dirEntry.fname)) {
            count++;
        }
        if (count == dir_index) {
            char* file_name = dirEntry.altname[0] ? dirEntry.altname : dirEntry.fname;
            if (strlen(uri.server) + 1 + strlen(file_name) >= sizeof(uri.file)) {
                return NULL;
            }
            strlcpy(uri.file, uri.server, sizeof(uri.file));
            strlcat(uri.file, "/", sizeof(uri.file));
            strlcat(uri.file, file_name, sizeof(uri.file));
            return uri.file;
        }
    }

    SYS_FS_DirClose(dirHandle);
    return NULL;     
}

void play_next_file_in_dir(void) {
    if (!dir_flag) { return; }
    if (dir_index == 0) { return; }
    dir_index++;
    if (dir_index > dir_count) {
        dir_index = 1;
    }
    VS1053_stop();
    VS1053_play_file(get_path_of_next_audio_file_in_currently_played_dir());
}

void play_prev_file_in_dir(void) {
    if (!dir_flag) { return; }
    if (dir_index == 0) { return; }
    dir_index--;
    if (dir_index == 0) {
        dir_index = dir_count;
    }
    VS1053_stop();
    VS1053_play_file(get_path_of_next_audio_file_in_currently_played_dir());
}