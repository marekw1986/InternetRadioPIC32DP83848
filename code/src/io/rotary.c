#include "rotary.h"


void rotary_init (void) {
    ROTARY_CLK_TRIS = 1;
    ROTARY_DT_TRIS = 1;
}


int8_t rotary_handle (void) {
    static uint8_t last_clk = 0;
    uint8_t tmp_clk;
    
    tmp_clk = ROTARY_CLK_PIN;
    
    if (tmp_clk != last_clk) {
        last_clk = tmp_clk;
        
        if (tmp_clk) {
            return 0;
        }
        
        if (ROTARY_DT_PIN) {
            return -1;
        }
        else {
            return 1;
        }
    }
    return 0;
}