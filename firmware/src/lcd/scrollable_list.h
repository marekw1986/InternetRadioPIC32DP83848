#ifndef SCROLLABLE_LIST_H
#define	SCROLLABLE_LIST_H

#include <stdint.h>

typedef char* (*scrollable_list_get_content_t)(uint16_t, char*, size_t, char*, size_t);
typedef uint16_t (*scrollable_list_get_max_item_id_t)(void);
typedef void (*scrollable_list_perform_action_t)(void);

typedef struct {
	scrollable_list_get_content_t get_content;
	scrollable_list_get_max_item_id_t get_max_item_id;
	scrollable_list_perform_action_t perform_action;
} scrollable_list_callbacks_t;

#ifdef	__cplusplus
extern "C" {
#endif

void handle_scrollable_list(void);
void scrollable_list_set_callbacks(const scrollable_list_get_content_t content_cbk, const scrollable_list_get_max_item_id_t max_id_cbk, const scrollable_list_perform_action_t action_cbk);
void draw_scrollable_list(void);
void scrollable_list_move_cursor(int8_t val);
int32_t scrollable_list_get_selected_item_id(void);
void scrollable_list_set_selected_item_id(uint16_t id);
void scrollable_list_next_page(void);
void scrollable_list_prev_page(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SCROLLABLE_LIST_H */
