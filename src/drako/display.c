#include <drako/display.h>

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
            disp |= DRKO_DISPL_0;
            break;
        case 0x01:
            disp |= DRKO_DISPL_1;
            break;
        case 0x02:
            disp |= DRKO_DISPL_2;
            break;
        case 0x03:
            disp |= DRKO_DISPL_3;
            break;
        case 0x04:
            disp |= DRKO_DISPL_4;
            break;
        case 0x05:
            disp |= DRKO_DISPL_5;
            break;
        case 0x06:
            disp |= DRKO_DISPL_6;
            break;
        case 0x07:
            disp |= DRKO_DISPL_7;
            break;
        case 0x08:
            disp |= DRKO_DISPL_8;
            break;
        case 0x09:
            disp |= DRKO_DISPL_9;
            break;
        case 0x0A:
            disp |= DRKO_DISPL_A;
            break;
        case 0x0B:
            disp |= DRKO_DISPL_B;
            break;
        case 0x0C:
            disp |= DRKO_DISPL_C;
            break;
        case 0x0D:
            disp |= DRKO_DISPL_D;
            break;
        case 0x0E:
            disp |= DRKO_DISPL_E;
            break;
        case 0x0F:
            disp |= DRKO_DISPL_F;
            break;
    }

    // high-byte encode
    switch (data&0xF0) {
        case 0x00:
            disp |= DRKO_DISPM_0;
            break;
        case 0x10:
            disp |= DRKO_DISPM_1;
            break;
        case 0x20:
            disp |= DRKO_DISPM_2;
            break;
        case 0x30:
            disp |= DRKO_DISPM_3;
            break;
        case 0x40:
            disp |= DRKO_DISPM_4;
            break;
        case 0x50:
            disp |= DRKO_DISPM_5;
            break;
        case 0x60:
            disp |= DRKO_DISPM_6;
            break;
        case 0x70:
            disp |= DRKO_DISPM_7;
            break;
        case 0x80:
            disp |= DRKO_DISPM_8;
            break;
        case 0x90:
            disp |= DRKO_DISPM_9;
            break;
        case 0xA0:
            disp |= DRKO_DISPM_A;
            break;
        case 0xB0:
            disp |= DRKO_DISPM_B;
            break;
        case 0xC0:
            disp |= DRKO_DISPM_C;
            break;
        case 0xD0:
            disp |= DRKO_DISPM_D;
            break;
        case 0xE0:
            disp |= DRKO_DISPM_E;
            break;
        case 0xF0:
            disp |= DRKO_DISPM_F;
            break;
    }

    return disp;
}
