#include "rotary.h"

void rotary_init (void) {
    ROTARY_CLK_TRIS = 1;
    ROTARY_DT_TRIS = 1;
}


int8_t rotary_handle (void) {
    static uint8_t position;
    uint8_t counter = 0;
    
    if (ROTARY_CLK_PIN != position) {
        if ( ROTARY_DT_PIN != position ) {
            counter--;
        }
        else {
            counter++;
        }
    }
    position = ROTARY_CLK_PIN;
    return counter;
}
