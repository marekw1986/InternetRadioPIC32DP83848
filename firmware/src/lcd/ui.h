/* 
 * File:   ui.h
 * Author: marek
 *
 * Created on 18 kwietnia 2023, 12:43
 */

#ifndef UI_H
#define	UI_H

typedef enum {UI_HANDLE_PLAY_SCREEN, UI_HANDLE_MAIN_LIST, UI_HANDLE_DIR_LIST, UI_HANDLE_STREAM_LIST} ui_state_t;

#ifdef	__cplusplus
extern "C" {
#endif

void ui_init(void);
void ui_switch_state(ui_state_t new_state);
void ui_update_volume(void);
void ui_update_content_info(const char* str);
void ui_clear_content_info(void);
void ui_update_state_info(const char* str);
void ui_clear_state_info(void);
void ui_update_loop_flag(uint8_t val);
void ui_update_dir_flag(uint8_t val);
void ui_handle(void);


#ifdef	__cplusplus
}
#endif

#endif	/* UI_H */

