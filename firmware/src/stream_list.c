#include <stdlib.h>
#include "lcd/hd44780.h"
#include "stream_list.h"
#include "definitions.h"

static uint16_t max_stream_id = 0;

static char* find_station_in_file(SYS_FS_HANDLE file, uint16_t number, char* workbuf, size_t workbuf_len, char* stream_name, size_t stream_name_len);

void initialize_stream_list(void) {
    SYS_FS_HANDLE file;
    
    file = SYS_FS_FileOpen(STREAM_LIST_PATH, SYS_FS_FILE_OPEN_READ);
    if (file == SYS_FS_HANDLE_INVALID) {
        SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Get station url: Can't open file\r\n");
        return;
    }
    char working_buffer[512];
    uint16_t number = 0;
    while (SYS_FS_FileStringGet(file, working_buffer, sizeof(working_buffer)) == SYS_FS_RES_SUCCESS) {
        number++;
    }
    if (number > 0) {
        max_stream_id = number;
    }
    SYS_CONSOLE_PRINT("%d streams found\r\n", max_stream_id);
    SYS_FS_FileClose(file);
}

uint16_t get_max_stream_id(void) {
    return max_stream_id;
}

char* get_station_url_from_file(uint16_t number, char* workbuf, size_t workbuf_len, char* stream_name, size_t stream_name_len) {
    SYS_FS_HANDLE file;
    char* result = NULL;
    
    file = SYS_FS_FileOpen(STREAM_LIST_PATH, SYS_FS_FILE_OPEN_READ);
    if (file == SYS_FS_HANDLE_INVALID) {
        SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Get station url: Can't open file\r\n");
        return NULL;
    }
    result = find_station_in_file(file, number, workbuf, workbuf_len, stream_name, stream_name_len);
    
    SYS_FS_FileClose(file);
    return result;
}

char* find_station_in_file(SYS_FS_HANDLE file, uint16_t number,  char* workbuf, size_t workbuf_len, char* stream_name, size_t stream_name_len) {
	char* result = NULL;
    uint16_t current_id = 0;
    while (SYS_FS_FileStringGet(file, workbuf, workbuf_len) == SYS_FS_RES_SUCCESS) {
        current_id++;
        if (current_id == number) {
            if (workbuf[strlen(workbuf)-1] == '\n') {
                workbuf[strlen(workbuf)-1] = '\0';
            }
            if (parse_stream_data_line(workbuf, strlen(workbuf), stream_name, stream_name_len, workbuf, workbuf_len)) {
                result = workbuf;
                break;
            }
            else { break; }
        }
    }
    return result;
}

bool parse_stream_data_line(char* line, size_t line_len, char* stream_name, size_t stream_name_len, char* stream_url, size_t stream_url_len) {
    char* url = strstr(line, " : ");
    if (url) {
        *url = '\0';
        url += 3;
        if (url >= line+line_len) { return 0; }
        if (stream_name) {
            strncpy(stream_name, line, stream_name_len);
        }
        if (stream_url) {
            strncpy(stream_url, url, stream_url_len);
        }
        return true;
    }
    return false;    
}

void stream_list_draw_menu_page(uint16_t id) {
    SYS_FS_HANDLE file;
    
    file = SYS_FS_FileOpen(STREAM_LIST_PATH, SYS_FS_FILE_OPEN_READ);
    if (file == SYS_FS_HANDLE_INVALID) {
        SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Get station url: Can't open file\r\n");
        return;
    }
    char workbuf[256];
    char name[64];
    char* result;
    for (uint8_t i=8; i < LCD_ROWS; i++) {
        result = find_station_in_file(file, id+i, workbuf, sizeof(workbuf)-1, name, sizeof(name)-1);
        lcd_locate(i, 0);
        lcd_char(' ');
        if (result) {
            lcd_utf8str_padd_rest(name, LCD_COLS, ' ');
        }
        else {
            lcd_utf8str_padd_rest(" ", LCD_COLS, ' ');
        }
    }
    SYS_FS_FileClose(file); 
}