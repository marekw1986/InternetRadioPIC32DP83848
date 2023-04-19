#include <stdio.h>
#include "hd44780.h"
#include "ui.h"
#include "../vs1003/vs1003.h"
#include "../common.h"

static uint32_t scroll_timer;
static bool scroll_info = false;
static char scroll_buffer[128];
static char* scroll_ptr;

const char padding[] = "                    ";

static void clear_utf8(char* str);

void lcd_ui_draw_interface(void) {
    lcd_locate(0, 0);
    lcd_str("Volume: ");
    lcd_ui_update_volume();
}

void lcd_ui_update_volume(void) {
    char supbuf[16];
    uint8_t volume = VS1003_getVolume();
    snprintf(supbuf, sizeof(supbuf)-1, "%d%s", volume, (volume < 100) ? " " : "");
    lcd_locate(0, 8);
    lcd_str(supbuf);
}

void lcd_ui_update_content_info(const char* str) {
    char supbuf[32];
    
    lcd_locate(1, 0);
    uint8_t len = strlen(str);
    if (len < 20) {
        char padbuf[32];
        uint8_t padlen = (20-len);
        memset(padbuf, ' ', padlen);
        padbuf[padlen+1] = '\0';
        snprintf(supbuf, sizeof(supbuf)-1, "%s%s", str, padbuf);
    }    
    else if (len == 20) {
        strncpy(supbuf, str, 20);
    }
    else {
        scroll_info = true;
        snprintf(scroll_buffer, sizeof(scroll_buffer)-1, "%s%s%s", padding, str, padding);
        clear_utf8(scroll_buffer);
        scroll_ptr = scroll_buffer;
        strncpy(supbuf, scroll_buffer, 20);
        scroll_timer = millis();
    }
    
    clear_utf8(supbuf);
    lcd_str(supbuf);
}

void lcd_ui_clear_content_info(void) {
    scroll_info = false;
    lcd_locate(1, 0);
    for (int i=0; i<20; i++) {
        lcd_char(' ');
    }
}

void lcd_ui_update_state_info(const char* str) {
    char supbuf[32];
    
    lcd_locate(2, 0);
    strncpy(supbuf, str, 20);
    clear_utf8(supbuf);
    lcd_str(supbuf);
}

void lcd_ui_clear_state_info(void) {
    lcd_locate(2, 0);
    for (int i=0; i<20; i++) {
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
        memset(supbuf, 0x00, sizeof(supbuf));
        strncpy(supbuf, scroll_ptr, 20);
        lcd_locate(1, 0);
        lcd_str(supbuf);
//        SYS_CONSOLE_PRINT("Whole: %s\r\n", scroll_buffer);
//        SYS_CONSOLE_PRINT("Window: %s\r\n", supbuf);
        scroll_ptr++;
        if (scroll_ptr >= (scroll_buffer+strlen(scroll_buffer))) {
            scroll_ptr = scroll_buffer;
        }
        scroll_timer = millis();
    }
}

static void clear_utf8(char* str) {     //TEMP WORKAROUND
    while (*str) {
        *str &= ~(1<<7);
        str++;
    }
}

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
}