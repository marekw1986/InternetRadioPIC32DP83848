#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include <p32xxxx.h>
#include <plib.h>
#include "rc5.h"


void rc5_init (void) {
    RC5_DT_TRIS = 1;
    INTSetVectorPriority(INT_EXTERNAL_3_VECTOR, INT_PRIORITY_LEVEL_5);
    INTSetVectorSubPriority(INT_EXTERNAL_3_VECTOR, INT_SUB_PRIORITY_LEVEL_1);
    INTCONbits.INT3EP = 0;      //Falling edge
    INTClearFlag(INT_SOURCE_EX_INT(3));
    INTEnable(INT_SOURCE_EX_INT(3), INT_ENABLED);
}


void __ISR(_EXTERNAL_3_VECTOR, IPL5AUTO) IntRC5Handler (void) {
    INTCONbits.INT3EP = !INTCONbits.INT3EP;
    INTClearFlag(INT_SOURCE_EX_INT(3));
}