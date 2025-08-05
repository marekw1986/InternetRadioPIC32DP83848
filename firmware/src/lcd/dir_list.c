#include "dir_list.h"
#include "definitions.h"
#include "scrollable_list.h"
#include "ui.h"
#include "../common.h"
#include "../vs1053/vs1053.h"

void dir_list_perform_action(void) {
	char working_buffer[512];
    char name[256];
    uint8_t is_dir = 0;
    char* path = get_file_path_from_media_dir_id_is_dir(scrollable_list_get_selected_item_id(), working_buffer, sizeof(working_buffer), name, sizeof(name), &is_dir);
    if (path) {
//        SYS_CONSOLE_PRINT("Path: %s\r\n", path);
        if (is_dir) {
            set_media_dir_path(path);
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
	char working_buffer[512];
    char name[256];
    uint8_t is_dir = 0;
    char* path = get_file_path_from_media_dir_id_is_dir(scrollable_list_get_selected_item_id(), working_buffer, sizeof(working_buffer), name, sizeof(name), &is_dir);
    if (is_dir) {
        VS1053_stop();
        VS1053_play_dir(path);
    }
}
