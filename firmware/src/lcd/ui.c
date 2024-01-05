#include <stdio.h>
#include "hd44780.h"
#include "ui.h"
#include "../vs1003/vs1003.h"
#include "../vs1003/mediainfo.h"
#include "../io/buttons.h"
#include "../io/rotary.h"
#include "../common.h"
#include "internal.h"

typedef enum {SCROLL, SCROLL_WAIT} scroll_state_t;

static button_t next_btn;
static button_t prev_btn;
static button_t state_button;

static ui_state_t ui_state = UI_HANDLE_MAIN_SCREEN;
static scroll_state_t scroll_state = SCROLL_WAIT;
static uint32_t scroll_timer;
static bool scroll_info = false;
static bool scroll_right = true;
static const char* scroll_begin;
static const char* scroll_ptr;

static int32_t selected_stream_id = 1;

static uint8_t calculate_selected_line(void);
static void ui_draw_main_screen(void);
static void ui_draw_scrollable_list(void);
static void ui_handle_main_screen(void);
static void ui_handle_scrollable_list(void);
static void ui_draw_pointer_at_line(uint8_t line);
static void ui_handle_updating_time(void);
static void ui_handle_scroll(void);

//Button functions
static void ui_rotary_change_volume(int8_t new_vol);
static void ui_rotary_move_cursor(int8_t val);
static void ui_button_switch_state(void);

void ui_init(void) {
    rotary_init();
    button_init(&prev_btn, &PORTG, _PORTG_RG13_MASK, &VS1003_play_prev, NULL);
    button_init(&next_btn, &PORTE, _PORTE_RE2_MASK, &VS1003_play_next, NULL);
    button_init(&state_button, &PORTE, _PORTE_RE5_MASK, &ui_button_switch_state, NULL);
    rotary_register_callback(ui_rotary_change_volume);
    
	ui_state = UI_HANDLE_MAIN_SCREEN;
	ui_draw_main_screen();
}

void ui_switch_state(ui_state_t new_state) {
	switch(new_state) {
		case UI_HANDLE_MAIN_SCREEN:
        ui_state = new_state;
        rotary_register_callback(ui_rotary_change_volume);
		ui_draw_main_screen();
		break;
		
		case UI_HANDLE_SCROLLABLE_LIST:
        ui_state = new_state;
        rotary_register_callback(ui_rotary_move_cursor);
		ui_draw_scrollable_list();
		break;
	}
}

uint8_t calculate_selected_line(void) {
	uint8_t selected_line = (selected_stream_id%(LCD_ROWS));
	selected_line = selected_line ? selected_line-1 : LCD_ROWS-1;
	return selected_line;
}

static void ui_draw_main_screen(void) {
	if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    lcd_cls();
	ui_update_content_info(mediainfo_title_get());
    lcd_flush_buffer()
    lcd_locate(3, 0);
    lcd_str("Volume: ");
    ui_update_volume();
    lcd_flush_buffer();
}

static void ui_draw_scrollable_list(void) {
	if (ui_state != UI_HANDLE_SCROLLABLE_LIST) { return; }
	uint8_t selected_line = calculate_selected_line();
	uint8_t stream_at_first_line = selected_stream_id-selected_line;
	char name[19];
    char buf[21];
	char* url = NULL;
    lcd_cls();
	for (int line=0; line<LCD_ROWS; line++) {
		url = get_station_url_from_file(stream_at_first_line+line, name, sizeof(name));
		if (url != NULL) {
            if (line == selected_line) {
                snprintf(buf, sizeof(buf), "%s%s", "->", name);
            }
            else {
                snprintf(buf, sizeof(buf), "%s%s", "  ", name);
            }
			lcd_locate(line, 0);
			lcd_str_padd_rest(buf, LCD_COLS, ' ');
            lcd_flush_buffer();
		}
	}
}

static void ui_draw_pointer_at_line(uint8_t line) {
	if (line > 3) { return; }
	lcd_locate(line, 0);
	lcd_str("->");
	lcd_flush_buffer();
}

void ui_update_volume(void) {
    char supbuf[16];
    
    if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    uint8_t volume = VS1003_getVolume();
    snprintf(supbuf, sizeof(supbuf)-1, "%d%s", volume, (volume < 100) ? " " : "");
    lcd_locate(3, 8);
    lcd_str(supbuf);
}

void ui_update_content_info(const char* str) {
	if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    if (strlen(str) <= LCD_COLS) {
        lcd_locate(1, 0);
        uint8_t rest = lcd_utf8str_part(str, LCD_COLS);
        while (rest) {
            lcd_char(' ');
            rest--;
        }
    }    
    else {
        scroll_info = true;
        scroll_right = true;
        scroll_begin = scroll_ptr = str;
        scroll_timer = millis();
        scroll_state = SCROLL_WAIT;
        lcd_locate(1, 0);
        lcd_utf8str_part(scroll_begin, LCD_COLS);
    }
    lcd_flush_buffer();
}

void ui_clear_content_info(void) {
	if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    scroll_info = false;
    lcd_locate(1, 0);
    for (int i=0; i<LCD_COLS; i++) {
        lcd_char(' ');
    }
    lcd_flush_buffer();
}

void ui_update_state_info(const char* str) {
	if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    ui_clear_state_info();
    lcd_flush_buffer();
    if (str) {
        lcd_locate(2,0);
        lcd_str(str);
    }
    lcd_flush_buffer();
}

void ui_clear_state_info(void) {
	if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    lcd_locate(2, 0);
    for (int i=0; i<LCD_COLS; i++) {
        lcd_char(' ');
    }
    lcd_flush_buffer();
}

void ui_handle(void) {
    switch(ui_state) {
		case UI_HANDLE_MAIN_SCREEN:
		ui_handle_main_screen();
		break;
		
		case UI_HANDLE_SCROLLABLE_LIST:
		ui_handle_scrollable_list();
		break;
	}
    rotary_handle();
    button_handle(&next_btn);
    button_handle(&prev_btn);
    button_handle(&state_button);
}

static void ui_handle_main_screen(void) {
    ui_handle_scroll();
    ui_handle_updating_time();	
}

static void ui_handle_scrollable_list(void) {

}

static void ui_handle_scroll(void) {
    if (!scroll_info) {
        return;
    }
    switch(scroll_state) {
        case SCROLL:
        if (((uint32_t)(millis()-scroll_timer) > 800)) {
            if (scroll_right) {
                scroll_ptr++;
                lcd_locate(1, 0);
                lcd_utf8str_padd_rest(scroll_ptr, LCD_COLS, ' ');
                if (*scroll_ptr & (1<<7)) { scroll_ptr++; }
                if (scroll_ptr >= (scroll_begin+strlen(scroll_begin))-LCD_COLS) {
                    scroll_right = false;
                    scroll_state = SCROLL_WAIT;
                }
            }
            else {
                scroll_ptr--;
                lcd_locate(1, 0);
                if (scroll_ptr <= (scroll_begin)) {
                    scroll_right = true;
                    scroll_state = SCROLL_WAIT;
                }
                else {
					if ( *(scroll_ptr-1) & (1<<7)) { scroll_ptr--; }	
				}
                lcd_utf8str_padd_rest(scroll_ptr, LCD_COLS, ' ');
            }
    //        SYS_CONSOLE_PRINT("Whole: %s\r\n", scroll_buffer);
    //        SYS_CONSOLE_PRINT("Window: %s\r\n", supbuf);            
            scroll_timer = millis();
        }             
        break;
        
        case SCROLL_WAIT:
        if (((uint32_t)(millis()-scroll_timer) > 3000)) {
            scroll_timer = millis();
            scroll_state = SCROLL;
        }
        break;
        
        default:
        break;
    }
}

void ui_handle_updating_time(void) {
    static uint8_t last_second = 0;
    
    time_t rawtime = time(NULL);
    struct tm* current_time = localtime(&rawtime);
    
    if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    if (current_time->tm_sec != last_second) {
        last_second = current_time->tm_sec;
        char supbuf[32];
        snprintf(supbuf, sizeof(supbuf), "%02d:%02d:%02d  %02d-%02d-%04d", (uint8_t)current_time->tm_hour, (uint8_t)current_time->tm_min, (uint8_t)current_time->tm_sec, (uint8_t)current_time->tm_mday, (uint8_t)current_time->tm_mon+1, (uint16_t)current_time->tm_year+1900);
        lcd_locate(0, 0);
        lcd_str(supbuf);
    }  
}

// Button functions
static void ui_rotary_change_volume(int8_t new_vol) {
    int8_t volume = VS1003_getVolume();
    volume += new_vol;
    if (volume > 100) volume = 100;
    if (volume < 0) volume = 0;
    VS1003_setVolume(volume);
}

static void ui_rotary_move_cursor(int8_t val) {
	uint8_t prev_selected_line = calculate_selected_line();
	selected_stream_id += val;
	if (selected_stream_id < 1) {
		selected_stream_id = 1;
	}
	else if (selected_stream_id > get_max_stream_id()) {
		selected_stream_id = get_max_stream_id();
	}
	if ( ((prev_selected_line == 0) && (val<0)) || ((prev_selected_line == LCD_ROWS-1) && (val > 0)) ) {
		ui_draw_scrollable_list();
	}
	else  {
		lcd_locate(prev_selected_line, 0);
		lcd_str("  ");
		lcd_flush_buffer();
		ui_draw_pointer_at_line(calculate_selected_line());
	}
}

static void ui_button_switch_state(void) {
    if (ui_state == UI_HANDLE_MAIN_SCREEN) {
        ui_switch_state(UI_HANDLE_SCROLLABLE_LIST);
    }
    else {
        ui_switch_state(UI_HANDLE_MAIN_SCREEN);
    }
}