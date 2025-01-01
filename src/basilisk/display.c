#include "basilisk/display.h"

void display_init(display* disp) {
    // init shift register, clear display, and show display
    shiftreg_init(&disp->sreg, 0, 1, 2, 27);
    display_clear(disp);
    display_show(disp);
}

void display_show(display* disp) { 
    shiftreg_oe_lo(&disp->sreg);
    disp->show_state = true;
}

void display_hide(display* disp) {
    shiftreg_oe_hi(&disp->sreg);
    disp->show_state = false;
}

void display_clear(display* disp) {
    disp->data = 0x0000;
    shiftreg_put16(&disp->sreg, 0x0000);
}

void display_write(display* disp, uint16_t data) {
    disp->data = data;
    shiftreg_put16(&disp->sreg, data);
}

void display_hex(display* disp, uint8_t byte) {
    display_write(disp, byte2disp(byte));
}

uint16_t byte2disp(uint8_t data) {
    uint16_t disp = 0x0000;

    // low-byte encode
    switch (data&0x0F) {
        case 0x00:
            disp |= BSLK_DISPL_0;
            break;
        case 0x01:
            disp |= BSLK_DISPL_1;
            break;
        case 0x02:
            disp |= BSLK_DISPL_2;
            break;
        case 0x03:
            disp |= BSLK_DISPL_3;
            break;
        case 0x04:
            disp |= BSLK_DISPL_4;
            break;
        case 0x05:
            disp |= BSLK_DISPL_5;
            break;
        case 0x06:
            disp |= BSLK_DISPL_6;
            break;
        case 0x07:
            disp |= BSLK_DISPL_7;
            break;
        case 0x08:
            disp |= BSLK_DISPL_8;
            break;
        case 0x09:
            disp |= BSLK_DISPL_9;
            break;
        case 0x0A:
            disp |= BSLK_DISPL_A;
            break;
        case 0x0B:
            disp |= BSLK_DISPL_B;
            break;
        case 0x0C:
            disp |= BSLK_DISPL_C;
            break;
        case 0x0D:
            disp |= BSLK_DISPL_D;
            break;
        case 0x0E:
            disp |= BSLK_DISPL_E;
            break;
        case 0x0F:
            disp |= BSLK_DISPL_F;
            break;
    }

    // high-byte encode
    switch (data&0xF0) {
        case 0x00:
            disp |= BSLK_DISPM_0;
            break;
        case 0x10:
            disp |= BSLK_DISPM_1;
            break;
        case 0x20:
            disp |= BSLK_DISPM_2;
            break;
        case 0x30:
            disp |= BSLK_DISPM_3;
            break;
        case 0x40:
            disp |= BSLK_DISPM_4;
            break;
        case 0x50:
            disp |= BSLK_DISPM_5;
            break;
        case 0x60:
            disp |= BSLK_DISPM_6;
            break;
        case 0x70:
            disp |= BSLK_DISPM_7;
            break;
        case 0x80:
            disp |= BSLK_DISPM_8;
            break;
        case 0x90:
            disp |= BSLK_DISPM_9;
            break;
        case 0xA0:
            disp |= BSLK_DISPM_A;
            break;
        case 0xB0:
            disp |= BSLK_DISPM_B;
            break;
        case 0xC0:
            disp |= BSLK_DISPM_C;
            break;
        case 0xD0:
            disp |= BSLK_DISPM_D;
            break;
        case 0xE0:
            disp |= BSLK_DISPM_E;
            break;
        case 0xF0:
            disp |= BSLK_DISPM_F;
            break;
    }

    return disp;
}
