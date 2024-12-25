#include "basilisk/display.h"

void display_init(display* disp) {
    // init shift register, clear display, and show display
    shiftreg_init(&disp->sreg, 0, 1, 2, 3);
    display_clear(disp);
    display_show(disp);
}

void display_show(display* disp) { 
    shiftreg_oe_lo(&disp->sreg);
}

void display_hide(display* disp) {
    shiftreg_oe_hi(&disp->sreg);
}

void display_clear(display* disp) {
    disp->data = 0x0000;
    shiftreg_put16(&disp->sreg, 0x0000);
}

void display_write(display* disp, uint16_t data) {
    disp->data = data;
    shiftreg_put16(&disp->sreg, data);
}
