#ifndef DIR_LIST_H
#define	DIR_LIST_H

#include <stdint.h>
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

void dir_list_perform_action(void);
void dir_list_perform_alternate_action(void);
void dir_list_perform_back(void);
char* dir_list_get_path_from_id(uint16_t number, char* working_buffer, size_t working_buffer_len, char* name, size_t name_len);
char* dir_list_get_path_from_id_is_dir(uint16_t number, char* working_buffer, size_t working_buffer_len, char* name, size_t name_len, uint8_t* is_dir);
uint16_t dir_list_count_displayable_items(void);
void dir_list_set_path(const char* dir_path);
char* dir_list_get_path(void);
uint16_t dir_list_get_number_of_items(void);

#ifdef	__cplusplus
}
#endif

#endif	/* DIR_MENU_H */
