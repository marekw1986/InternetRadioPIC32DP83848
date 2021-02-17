#ifndef RC5_H
#define	RC5_H

#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include <p32xxxx.h>
#include <plib.h>

#define RC5_DT_TRIS                 TRISAbits.TRISA14
#define RC5_DT_PIN                  PORTAbits.RA14


#ifdef	__cplusplus
extern "C" {
#endif

void rc5_init (void);

#ifdef	__cplusplus
}
#endif

#endif	/* RC5_H */

