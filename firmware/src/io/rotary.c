#include "rotary.h"
#include "buttons.h"

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
    uint8_t tmp_clk;
    
    tmp_clk = ROTARY_CLK_PIN;
    
    if (tmp_clk != last_clk) {
        last_clk = tmp_clk;
        
        if (tmp_clk) {
            return;
        }
        
        if (ROTARY_DT_PIN) {
            button_call_global_callback();
            rotary_cbk(-1);
        }
        else {
            button_call_global_callback();
            rotary_cbk(1);
        }
    }
    return;
}