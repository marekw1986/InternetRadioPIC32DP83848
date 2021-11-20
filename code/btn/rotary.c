#include "rotary.h"

volatile uint8_t rotary_flag = 0;
volatile int16_t rotary_counter = 0; 


void rotary_init (void) {
    ROTARY_CLK_TRIS = 1;
    ROTARY_DT_TRIS = 1;
}


int8_t rotary_handle (void) {
    static uint8_t last_clk = 0;
    static uint8_t last_dt = 0;
    uint8_t tmp_clk, tmp_dt;
    
    tmp_clk = ROTARY_CLK_PIN;
    tmp_dt = ROTARY_DT_PIN;
    
    if (tmp_clk != last_clk) {
        last_clk = tmp_clk;
        
        if (tmp_dt != last_dt) {
            last_dt = tmp_dt;
            return 0;
        }
        
        if (tmp_dt) {
            return -1;
        }
        else {
            return 1;
        }
    }
    return 0;
}