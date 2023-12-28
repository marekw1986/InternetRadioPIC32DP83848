/* 
 * File:   rotary.h
 * Author: marek
 *
 * Created on 31 stycznia 2021, 14:28
 */

#include <xc.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef ROTARY_H
#define	ROTARY_H

#define ROTARY_CLK_TRIS TRISDbits.TRISD1
#define ROTARY_CLK_PIN PORTDbits.RD1
#define ROTARY_DT_TRIS TRISDbits.TRISD9
#define ROTARY_DT_PIN PORTDbits.RD9

#ifdef	__cplusplus
extern "C" {
#endif

void rotary_init (void);
int8_t rotary_handle (void);


#ifdef	__cplusplus
}
#endif

#endif	/* ROTARY_H */

