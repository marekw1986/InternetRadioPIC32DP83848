#include "dir_list.h"
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
        if (is_dir) {
            set_media_dir_path(path);
            ui_switch_state(UI_HANDLE_DIR_LIST);
        }
        else {
            VS1053_play_file(path);
            ui_switch_state(UI_HANDLE_PLAY_SCREEN);
        }
    }
}
