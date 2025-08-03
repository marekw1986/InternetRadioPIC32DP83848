#include <stdio.h>
#include "scrollable_list.h"
#ifdef PCDEBUG
#include "low_level.h"
#else
#include "hd44780.h"
#include "../common.h"
#endif

static bool drawing_scrollable_list_active = false;
static uint8_t currently_drawn_line=0;
static int32_t selected_item_id = 1;
static int32_t cur_pos = 0;
static scrollable_list_callbacks_t callbacks;

static uint8_t calculate_selected_line(void);
static void draw_pointer_at_line(uint8_t line);
inline static void scrollable_list_increment_selected_item_id_by_lcd_rows(void);
inline static void scrollable_list_decrement_selected_item_id_by_lcd_rows(void);


void handle_scrollable_list(void) {
    if (drawing_scrollable_list_active) {
        uint8_t selected_line = calculate_selected_line();
        uint8_t item_at_first_line = selected_item_id-selected_line;
        char name[22];
        char buf[24];
        char* url = NULL;
        char working_buffer[512];
        if (callbacks.get_content == NULL) { return; }
        url = callbacks.get_content(item_at_first_line+currently_drawn_line, working_buffer, sizeof(working_buffer), name, sizeof(name));
        if (url != NULL) {
            int bytes_in_buffer;
            if (currently_drawn_line == selected_line) {
                bytes_in_buffer = snprintf(buf, sizeof(buf), "%s%d %s", ">", item_at_first_line+currently_drawn_line, name);
            }
            else {
                bytes_in_buffer = snprintf(buf, sizeof(buf), "%s%d %s", " ", item_at_first_line+currently_drawn_line, name);
            }
            if (bytes_in_buffer > 0) {
                lcd_locate(currently_drawn_line, 0);
                lcd_utf8str_padd_rest(buf, LCD_COLS, ' ');
            }
            currently_drawn_line++;
        }
        else {
            drawing_scrollable_list_active = false;
        }
        if (currently_drawn_line >= LCD_ROWS) {
            cur_pos = 0;
            drawing_scrollable_list_active = false;
        }
    }
}

void scrollable_list_set_callbacks(const scrollable_list_get_content_t content_cbk, const scrollable_list_get_max_item_id_t max_id_cbk, const scrollable_list_perform_action_t action_cbk) {
	callbacks.get_content = content_cbk;
	callbacks.get_max_item_id = max_id_cbk;
	callbacks.perform_action = action_cbk;
}

void draw_scrollable_list(void) {
    lcd_cls();
    cur_pos = 0;
    currently_drawn_line=0;
    drawing_scrollable_list_active = true;
}

static uint8_t calculate_selected_line(void) {
	uint8_t selected_line = (selected_item_id%(LCD_ROWS));
	selected_line = selected_line ? selected_line-1 : LCD_ROWS-1;
	return selected_line;
}

void scrollable_list_move_cursor(int8_t val) {
	uint8_t prev_selected_line = calculate_selected_line();
    uint16_t prev_selected_item_id = selected_item_id;
	selected_item_id += val;
	if (callbacks.get_max_item_id == NULL) { return; }
	if (selected_item_id < 1) {
		selected_item_id = callbacks.get_max_item_id();
	}
	else if (selected_item_id > callbacks.get_max_item_id()) {
		selected_item_id = 1;
	}
	if ( ((prev_selected_line == 0) && (val<0)) || ( ((prev_selected_line == LCD_ROWS-1) || (prev_selected_item_id == callbacks.get_max_item_id())) && (val > 0)) ) {
        draw_scrollable_list();
	}
	else  {
		lcd_locate(prev_selected_line, 0);
		lcd_char(' ');
		draw_pointer_at_line(calculate_selected_line());
	}
}

static void draw_pointer_at_line(uint8_t line) {
	if (line > 3) { return; }
	lcd_locate(line, 0);
	lcd_char('>');
}

void scrollable_list_set_selected_item_id(uint16_t id) {
	if (callbacks.get_max_item_id == NULL) { return; }
	if (id > callbacks.get_max_item_id()) return;
	selected_item_id = id;
}

int32_t scrollable_list_get_selected_item_id(void) {
	return selected_item_id;
}

inline static void scrollable_list_increment_selected_item_id_by_lcd_rows(void) {
	selected_item_id += LCD_ROWS;
}

inline static void scrollable_list_decrement_selected_item_id_by_lcd_rows(void) {
	selected_item_id -= LCD_ROWS;
}

void scrollable_list_next_page(void) {
	if (callbacks.get_max_item_id == NULL) { return; }
    scrollable_list_increment_selected_item_id_by_lcd_rows();
    if (scrollable_list_get_selected_item_id() > callbacks.get_max_item_id()) {
        scrollable_list_set_selected_item_id(1);
    }
    draw_scrollable_list();
}

void scrollable_list_prev_page(void) {
	if (callbacks.get_max_item_id == NULL) { return; }
    scrollable_list_decrement_selected_item_id_by_lcd_rows();
    if (scrollable_list_get_selected_item_id() < 1) {
        scrollable_list_set_selected_item_id(callbacks.get_max_item_id());
    }
    draw_scrollable_list();
}
