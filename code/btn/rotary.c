#include "rotary.h"

volatile uint8_t rotary_flag = 0;
volatile int16_t rotary_counter = 0; 


void rotary_init (void) {
    ROTARY_CLK_TRIS = 1;
    ROTARY_DT_TRIS = 1;
    INTSetVectorPriority(INT_EXTERNAL_4_VECTOR, INT_PRIORITY_LEVEL_6);
    INTSetVectorSubPriority(INT_EXTERNAL_4_VECTOR, INT_SUB_PRIORITY_LEVEL_1);
    INTCONbits.INT4EP = 0;      //Falling edge
    INTClearFlag(INT_SOURCE_EX_INT(4));
    INTEnable(INT_SOURCE_EX_INT(4), INT_ENABLED);
}


int16_t rotary_handle (void) {
    int16_t counter;
    
    if (!rotary_flag) return 0;
    counter = rotary_counter;
    rotary_counter = 0;
    rotary_flag = 0;
    return counter;
}


void __ISR(_EXTERNAL_4_VECTOR, IPL6AUTO) IntRotaryHandler (void) {
    static uint8_t position = 1;
    
     if (ROTARY_CLK_PIN != position) {
        if (ROTARY_DT_PIN != position) {
            rotary_counter++;
        }
        else {
            rotary_counter--;
        }
     }
    position = ROTARY_CLK_PIN;
    rotary_flag = 1;    
    INTCONbits.INT4EP = !INTCONbits.INT4EP;
    INTClearFlag(INT_SOURCE_EX_INT(4));
}