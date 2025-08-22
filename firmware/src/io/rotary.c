#include "rotary.h"
#include "buttons.h"
#include "../common.h"

void (*rotary_cbk)(int8_t);

void rotary_init (void) {
    ROTARY_CLK_TRIS = 1;
    ROTARY_DT_TRIS = 1;
    rotary_cbk = NULL;
}

void rotary_register_callback(void (*cbk)(int8_t)) {
    if (cbk) {
        rotary_cbk = cbk;
    }
}

void rotary_handle (void) {
    static uint8_t last_clk = 0;
    static uint32_t last_turn_time = 0;
    uint8_t tmp_clk;
    uint32_t now = millis();
    
    tmp_clk = ROTARY_CLK_PIN;
    
    if (tmp_clk != last_clk) {
        last_clk = tmp_clk;
        
        if (tmp_clk) {
            return;
        }
        
        uint32_t delta = now - last_turn_time;
        last_turn_time = now;
        
        int step_size = 1;
        if (delta < 50) { step_size = 5; }       // very fast
        else if (delta < 100) { step_size = 3; } // fast
        else if (delta < 200) { step_size = 2; } // medium
        else { step_size = 1; }
        
        if (ROTARY_DT_PIN) {
            button_call_global_callback();
            rotary_cbk(-step_size);
        }
        else {
            button_call_global_callback();
            rotary_cbk(step_size);
        }
    }
    return;
}