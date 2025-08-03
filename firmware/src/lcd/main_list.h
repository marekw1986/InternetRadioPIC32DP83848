#ifndef MAIN_LIST_H
#define	MAIN_LIST_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

char* main_list_get_content(uint16_t number, char* working_buffer, size_t working_buffer_len, char* name, size_t name_len);
uint16_t main_list_get_max_id(void);
void main_list_perform_action(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_MENU_H */
