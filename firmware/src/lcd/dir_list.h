#ifndef DIR_LIST_H
#define	DIR_LIST_H

#include <stdint.h>
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

void dir_list_set_dir(char* path);
uint16_t dir_list_get_max_id(void);
void dir_list_perform_action(void);

#ifdef	__cplusplus
}
#endif

#endif	/* DIR_MENU_H */
