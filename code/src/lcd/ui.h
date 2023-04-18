/* 
 * File:   ui.h
 * Author: marek
 *
 * Created on 18 kwietnia 2023, 12:43
 */

#ifndef UI_H
#define	UI_H

#ifdef	__cplusplus
extern "C" {
#endif

void lcd_ui_draw_interface(void);    
void lcd_ui_update_volume(void);
void lcd_ui_update_content_info(const char* str);
void lcd_ui_clear_content_info(void);
void lcd_ui_handle(void);



#ifdef	__cplusplus
}
#endif

#endif	/* UI_H */

