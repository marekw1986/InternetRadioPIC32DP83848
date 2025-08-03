#include <stddef.h>
#include "string.h"
#include "main_list.h"
#include "ui.h"
#include "scrollable_list.h"

char* main_list_get_content(uint16_t number, char* working_buffer, size_t working_buffer_len, char* name, size_t name_len) {
	switch(number) {
		case 1:
		strncpy(name, "USB", name_len);
		break;
		
		case 2:
		strncpy(name, "SD", name_len);
		break;
		
		case 3:
		strncpy(name, "Streams", name_len);
		break;
		
		case 4:
		strncpy(name, "Radio", name_len);
		break;
		
		default:
		strncpy(name, "", name_len);
		break;
	}
	return name;
}

uint16_t main_list_get_max_id(void) {
	return 4;
}

void main_list_perform_action(void) {
	switch (scrollable_list_get_selected_item_id()) {
		case 1:
		break;
		
		case 2:
		break;
		
		case 3:
		ui_switch_state(UI_HANDLE_STREAM_LIST);
		break;
		
		case 4:
		break;
		
		default:
		break;
	}
}
