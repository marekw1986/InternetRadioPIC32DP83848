/* 
 * File:   buttons.h
 * Author: marek
 *
 * Created on 31 stycznia 2021, 09:00
 */

#ifndef BUTTONS_H
#define	BUTTONS_H

#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include <p32xxxx.h>
#include <plib.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef struct button {
    volatile unsigned int *port;
    uint16_t pin_bm;
    uint32_t timer;
    uint8_t state;
    void (*push_proc)(void);
    void (*long_proc)(void);
} button_t;

void button_init(button_t *btn, volatile unsigned int *port, uint16_t pin_bm, void (*push_proc)(void), void (*long_proc)(void));
void button_handle(button_t *btn);



#ifdef	__cplusplus
}
#endif

#endif	/* BUTTONS_H */

