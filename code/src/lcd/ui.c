#include <stdio.h>
#include "hd44780.h"
#include "ui.h"
#include "../vs1003/vs1003.h"
#include "../common.h"

static uint32_t scroll_timer;
static bool scroll_info = false;
static bool scroll_right = true;
static char scroll_buffer[128];
static char* scroll_ptr;

const char padding[] = "                    ";

void lcd_ui_handle_updating_time(void);

//static void clear_utf8(char* str);
static void copy_utf8_to_ascii(char* dst, const char* src, uint16_t len);

void lcd_ui_draw_interface(void) {
    lcd_locate(3, 0);
    lcd_str("Volume: ");
    lcd_ui_update_volume();
}

void lcd_ui_update_volume(void) {
    char supbuf[16];
    uint8_t volume = VS1003_getVolume();
    snprintf(supbuf, sizeof(supbuf)-1, "%d%s", volume, (volume < 100) ? " " : "");
    lcd_locate(3, 8);
    lcd_str(supbuf);
}

void lcd_ui_update_content_info(const char* str) {
    char supbuf[32];
    char asciibuf[128];
    
    memset(asciibuf, 0x00, sizeof(asciibuf));
    copy_utf8_to_ascii(asciibuf, str, sizeof(asciibuf)-1);
    
    uint8_t len = strlen(asciibuf);
    if (len < LCD_COLS) {
        char padbuf[32];
        memset(padbuf, 0x00, sizeof(padbuf));
        uint8_t padlen = (LCD_COLS-len);
        memset(padbuf, ' ', padlen);
        padbuf[padlen+1] = '\0';
        //char buf[32];
        //copy_utf8_to_ascii(buf, str, sizeof(buf)-1);
        snprintf(supbuf, sizeof(supbuf)-1, "%s%s", asciibuf, padbuf);
    }    
    else if (len == LCD_COLS) {
        strncpy(supbuf, asciibuf, LCD_COLS);
        //copy_utf8_to_ascii(supbuf, str, LCD_COLS);
    }
    else {
        scroll_info = true;
        scroll_right = true;
        //snprintf(scroll_buffer, sizeof(scroll_buffer)-1, "%s%s%s", padding, asciibuf, padding);
        strncpy(scroll_buffer, asciibuf, sizeof(scroll_buffer)-1);        //clear_utf8(scroll_buffer);
        scroll_ptr = scroll_buffer;
        strncpy(supbuf, scroll_buffer, LCD_COLS);
        //copy_utf8_to_ascii(supbuf, scroll_buffer, LCD_COLS);
        scroll_timer = millis();
    }
    
    lcd_locate(1, 0);
    lcd_str(supbuf);
}

void lcd_ui_clear_content_info(void) {
    scroll_info = false;
    lcd_locate(1, 0);
    for (int i=0; i<LCD_COLS; i++) {
        lcd_char(' ');
    }
}

void lcd_ui_update_state_info(const char* str) {
    char supbuf[32];
    
    lcd_locate(2, 0);
    copy_utf8_to_ascii(supbuf, str, LCD_COLS);
    //clear_utf8(supbuf);
    lcd_str(supbuf);
}

void lcd_ui_clear_state_info(void) {
    lcd_locate(2, 0);
    for (int i=0; i<LCD_COLS; i++) {
        lcd_char(' ');
    }
}

void lcd_ui_handle(void) {
    static uint32_t refresh_timer = 0;
    
    if ( (uint32_t)(millis()-refresh_timer) > 5000) {
        //Periodicaly refresh UI;
        lcd_ui_draw_interface();
        refresh_timer = millis();
    }
    
    if (scroll_info && ((uint32_t)(millis()-scroll_timer) > 800) ) {
        char supbuf[32];
        //memset(supbuf, 0x00, sizeof(supbuf));
        //copy_utf8_to_ascii(supbuf, scroll_ptr, LCD_COLS);
        copy_utf8_to_ascii(supbuf, scroll_ptr, LCD_COLS);
        lcd_locate(1, 0);
        lcd_str(supbuf);
//        if (strlen(supbuf) < LCD_COLS) {
//            int pad = LCD_COLS - strlen(supbuf);
//            while(pad) {
//                lcd_char(' ');
//                pad--;
//            }
//        }
//        SYS_CONSOLE_PRINT("Whole: %s\r\n", scroll_buffer);
//        SYS_CONSOLE_PRINT("Window: %s\r\n", supbuf);
        if (scroll_right) {
            scroll_ptr++;
            if (scroll_ptr >= (scroll_buffer+strlen(scroll_buffer))-LCD_COLS) {
                scroll_right = false;
            }
        }
        else {
            scroll_ptr--;
            if (scroll_ptr <= (scroll_buffer)) {
                scroll_right = true;
            }
        }
        scroll_timer = millis();
    }
    
    lcd_ui_handle_updating_time();
}

void lcd_ui_handle_updating_time(void) {
    static uint8_t last_second = 0;
    
    time_t rawtime = time(NULL);
    struct tm* current_time = localtime(&rawtime);
    
    if (current_time->tm_sec != last_second) {
        last_second = current_time->tm_sec;
        
        char supbuf[32];
        snprintf(supbuf, sizeof(supbuf)-1, "%02d:%02d:%02d", current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
        lcd_locate(0, 0);
        lcd_str(supbuf);
    }  
}

//static void clear_utf8(char* str) {     //TEMP WORKAROUND
//    while (*str) {
//        *str &= ~(1<<7);
//        str++;
//    }
//}

static void copy_utf8_to_ascii(char* dst, const char* src, uint16_t len) {
    uint16_t copied = 0;
    while (*src) {
        if ( !(*src & (1<<7)) ) {
            *dst = *src;
            dst++;
            copied++;
            if (copied >= len) {break;}
        }
        src++;
    }
    *dst = '\0';
}