#include "rotary.h"

volatile uint8_t rotary_flag = 0;
volatile int16_t rotary_counter = 0; 


void rotary_init (void) {
    ROTARY_CLK_TRIS = 1;
    ROTARY_DT_TRIS = 1;
}


int8_t rotary_handle (void) {
    static uint8_t last_clk = 0;
    uint8_t tmp;
    
    tmp = ROTARY_CLK_PIN;
    if (tmp != last_clk) {
        last_clk = tmp;
        if (ROTARY_DT_PIN) {
            return -1;
        }
        else {
            return 1;
        }
    }
    return 0;
}