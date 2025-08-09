#include "dir_list.h"
#include "definitions.h"
#include "scrollable_list.h"
#include "ui.h"
#include "../common.h"
#include "../vs1053/vs1053.h"

static char media_dir_path[128];
static uint16_t number_of_items_in_media_dir = 0;

void dir_list_perform_action(void) {
	char working_buffer[512];
    char name[256];
    uint8_t is_dir = 0;
    char* path = dir_list_get_path_from_id_is_dir(scrollable_list_get_selected_item_id(), working_buffer, sizeof(working_buffer), name, sizeof(name), &is_dir);
    if (path) {
//        SYS_CONSOLE_PRINT("Path: %s\r\n", path);
        if (is_dir) {
            dir_list_set_path(path);
            ui_switch_state(UI_HANDLE_DIR_LIST);
        }
        else {
            VS1053_stop();
            VS1053_play_file(path);
            ui_switch_state(UI_HANDLE_PLAY_SCREEN);
        }
    }
}

void dir_list_perform_alternate_action(void) {
	char working_buffer[256];
    char name[64];
    uint8_t is_dir = 0;
    char* path = dir_list_get_path_from_id_is_dir(scrollable_list_get_selected_item_id(), working_buffer, sizeof(working_buffer), name, sizeof(name), &is_dir);
    if (is_dir) {
        VS1053_stop();
        VS1053_play_dir(path);
        ui_switch_state(UI_HANDLE_PLAY_SCREEN);
    }
}

void dir_list_perform_back(void) {
    char* last_slash;
    last_slash = strrchr(media_dir_path, '/');
    if (!last_slash) { return; }
    *last_slash = '\0';
    // TODO: Refactor this
    if ( (strncmp(media_dir_path, "/mnt/myDrive0", sizeof(media_dir_path)) == 0) || (strncmp(media_dir_path, "/mnt/myDrive1", sizeof(media_dir_path)) == 0) )  {
        ui_switch_state(UI_HANDLE_MAIN_LIST);
        return;
    }
    dir_list_set_path(media_dir_path);
    ui_switch_state(UI_HANDLE_DIR_LIST);
}

char* dir_list_get_path_from_id(uint16_t number, char* working_buffer, size_t working_buffer_len, char* name, size_t name_len) {
    return dir_list_get_path_from_id_is_dir(number, working_buffer, working_buffer_len, name, name_len, NULL);
}

char* dir_list_get_path_from_id_is_dir(
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
    
    dirEntry.lfname = NULL;
    dirEntry.lfsize = 0;

    dirHandle = SYS_FS_DirOpen(media_dir_path);
    if (dirHandle == SYS_FS_HANDLE_INVALID) {
        SYS_CONSOLE_PRINT("Error opening directory: %s\n", media_dir_path);
        return NULL;
    }

    while (SYS_FS_DirRead(dirHandle, &dirEntry) == SYS_FS_RES_SUCCESS) {
        if (!dirEntry.fname[0]) { // empty string, end of directory
            break;
        }

        if ((dirEntry.fattrib & SYS_FS_ATTR_DIR) || is_audio_file(dirEntry.fname)) {
            count++;
        }

        if (count == number) {
            SYS_FS_DirClose(dirHandle);
            if (is_dir != NULL) {
                *is_dir = (dirEntry.fattrib & SYS_FS_ATTR_DIR);
            }
            if (name) {
                // Copy the display name
                strncpy(name, dirEntry.fname, name_len);
                name[name_len - 1] = '\0';  // Ensure null-termination
            }
            if (working_buffer) {
                char* file_name = dirEntry.altname[0] ? dirEntry.altname : dirEntry.fname;
                snprintf(working_buffer, working_buffer_len, "%s/%s", media_dir_path, file_name);
                return working_buffer;
            }
        }
    }

    SYS_FS_DirClose(dirHandle);
    return NULL;
}

uint16_t dir_list_count_displayable_items(void) {
    SYS_FS_HANDLE dirHandle;
    SYS_FS_FSTAT dirEntry;
    uint16_t count = 0;
    
    dirEntry.lfname = NULL;
    dirEntry.lfsize = 0;

    dirHandle = SYS_FS_DirOpen(media_dir_path);
    if (dirHandle == SYS_FS_HANDLE_INVALID) {
        SYS_CONSOLE_PRINT("Error opening directory: %s\n", media_dir_path);
        return 0;
    }

    while (SYS_FS_DirRead(dirHandle, &dirEntry) == SYS_FS_RES_SUCCESS) {
        if (!dirEntry.fname[0]) { // empty string, end of directory
            break;
        }
        
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

void dir_list_set_path(const char* dir_path) {
    strncpy(media_dir_path, dir_path, sizeof(media_dir_path));
    number_of_items_in_media_dir = dir_list_count_displayable_items();
}

char* dir_list_get_path(void) {
    return media_dir_path;
}

uint16_t dir_list_get_number_of_items(void) {
    return number_of_items_in_media_dir;
}