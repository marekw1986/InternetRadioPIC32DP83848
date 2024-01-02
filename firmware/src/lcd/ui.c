#include <stdio.h>
#include "hd44780.h"
#include "ui.h"
#include "../vs1003/vs1003.h"
#include "../vs1003/mediainfo.h"
#include "../common.h"

typedef enum {HANDLE_MAIN_SCREEN, HANDLE_SCROLLABLE_LIST} ui_state_t;
typedef enum {SCROLL, SCROLL_WAIT} scroll_state_t;

static ui_state_t ui_state = HANDLE_MAIN_SCREEN;
static scroll_state_t scroll_state = SCROLL_WAIT;
static uint32_t scroll_timer;
static bool scroll_info = false;
static bool scroll_right = true;
static const char* scroll_begin;
static const char* scroll_ptr;

static void lcd_ui_draw_main_screen(void);
static void lcd_handle_main_screen(void);
static void lcd_handle_scrollable_list(void);
static void lcd_ui_handle_updating_time(void);
static void lcd_ui_handle_scroll(void);

void lcd_ui_init(void) {
	ui_state = HANDLE_MAIN_SCREEN;
	lcd_ui_draw_main_screen();
}

static void lcd_ui_draw_main_screen(void) {
	if (ui_state != HANDLE_MAIN_SCREEN) { return; }
	lcd_ui_update_content_info(mediainfo_title_get());
    lcd_locate(3, 0);
    lcd_str("Volume: ");
    lcd_ui_update_volume();
}

void lcd_ui_update_volume(void) {
    char supbuf[16];
    
    if (ui_state != HANDLE_MAIN_SCREEN) { return; }
    uint8_t volume = VS1003_getVolume();
    snprintf(supbuf, sizeof(supbuf)-1, "%d%s", volume, (volume < 100) ? " " : "");
    lcd_locate(3, 8);
    lcd_str(supbuf);
}

void lcd_ui_update_content_info(const char* str) {
	if (ui_state != HANDLE_MAIN_SCREEN) { return; }
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

void lcd_ui_clear_content_info(void) {
	if (ui_state != HANDLE_MAIN_SCREEN) { return; }
    scroll_info = false;
    lcd_locate(1, 0);
    for (int i=0; i<LCD_COLS; i++) {
        lcd_char(' ');
    }
    lcd_flush_buffer();
}

void lcd_ui_update_state_info(const char* str) {
	if (ui_state != HANDLE_MAIN_SCREEN) { return; }
    lcd_ui_clear_state_info();
    lcd_flush_buffer();
    if (str) {
        lcd_locate(2,0);
        lcd_str(str);
    }
    lcd_flush_buffer();
}

void lcd_ui_clear_state_info(void) {
	if (ui_state != HANDLE_MAIN_SCREEN) { return; }
    lcd_locate(2, 0);
    for (int i=0; i<LCD_COLS; i++) {
        lcd_char(' ');
    }
    lcd_flush_buffer();
}

void lcd_ui_handle(void) {
    switch(ui_state) {
		case HANDLE_MAIN_SCREEN:
		lcd_handle_main_screen();
		break;
		
		case HANDLE_SCROLLABLE_LIST:
		lcd_handle_scrollable_list();
		break;
	}
}

static void lcd_handle_main_screen(void) {
    lcd_ui_handle_scroll();
    lcd_ui_handle_updating_time();	
}

static void lcd_handle_scrollable_list(void) {
	
}

static void lcd_ui_handle_scroll(void) {
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

void lcd_ui_handle_updating_time(void) {
    static uint8_t last_second = 0;
    
    time_t rawtime = time(NULL);
    struct tm* current_time = localtime(&rawtime);
    
    if (ui_state != HANDLE_MAIN_SCREEN) { return; }
    if (current_time->tm_sec != last_second) {
        last_second = current_time->tm_sec;
        char supbuf[32];
        snprintf(supbuf, sizeof(supbuf), "%02d:%02d:%02d  %02d-%02d-%04d", (uint8_t)current_time->tm_hour, (uint8_t)current_time->tm_min, (uint8_t)current_time->tm_sec, (uint8_t)current_time->tm_mday, (uint8_t)current_time->tm_mon+1, (uint16_t)current_time->tm_year+1900);
        lcd_locate(0, 0);
        lcd_str(supbuf);
    }  
}
