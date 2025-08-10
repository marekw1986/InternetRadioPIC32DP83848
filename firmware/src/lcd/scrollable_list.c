#include <stdio.h>
#include "scrollable_list.h"
#ifdef PCDEBUG
#include "low_level.h"
#else
#include "hd44780.h"
#include "../common.h"
#endif

static int32_t selected_item_id = 1;
static scrollable_list_config_t config;

static uint8_t calculate_selected_line(void);
static void draw_pointer_at_line(uint8_t line);


void handle_scrollable_list(void) {

}

void scrollable_list_set_config(const scrollable_list_draw_menu_page_t draw_menu_page_cbk, const scrollable_list_get_max_item_id_t max_id_cbk, const uint8_t show_number) {
	config.draw_menu_page = draw_menu_page_cbk;
	config.get_max_item_id = max_id_cbk;
	config.show_number = show_number;
}

void draw_scrollable_list(void) {
    lcd_cls();
    if (config.draw_menu_page) {
        config.draw_menu_page(selected_item_id);
    }
    draw_pointer_at_line(calculate_selected_line());
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
	if (config.get_max_item_id == NULL) { return; }
	if (selected_item_id < 1) {
		selected_item_id = config.get_max_item_id();
	}
	else if (selected_item_id > config.get_max_item_id()) {
		selected_item_id = 1;
	}
	if ( ((prev_selected_line == 0) && (val<0)) || ( ((prev_selected_line == LCD_ROWS-1) || (prev_selected_item_id == config.get_max_item_id())) && (val > 0)) ) {
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
	if (config.get_max_item_id == NULL) { return; }
	if (id > config.get_max_item_id()) return;
	selected_item_id = id;
}

uint16_t scrollable_list_get_selected_item_id(void) {
	return selected_item_id;
}

void scrollable_list_next_page(void) {
    if (config.get_max_item_id == NULL) { return; }

    int32_t max_id = config.get_max_item_id();
    int32_t new_id = selected_item_id + LCD_ROWS;

    if (new_id > max_id) {
        new_id = 1;
    }

    new_id = ((new_id - 1) / LCD_ROWS) * LCD_ROWS + 1;

    scrollable_list_set_selected_item_id(new_id);
    draw_scrollable_list();
}

void scrollable_list_prev_page(void) {
    if (config.get_max_item_id == NULL) { return; }

    int32_t max_id = config.get_max_item_id();
    int32_t new_id = selected_item_id - LCD_ROWS;

    if (new_id < 1) {
        new_id = ((max_id - 1) / LCD_ROWS) * LCD_ROWS + 1;
    } else {
        new_id = ((new_id - 1) / LCD_ROWS) * LCD_ROWS + 1;
    }

    scrollable_list_set_selected_item_id(new_id);
    draw_scrollable_list();
}