#include <drako/modules/display.h>


/**
 * @brief Initializes display object.
 * @param disp Pointer to display object.
 */
void display_init(display* disp) {
    // init shift register, clear display, and show display
    sn74hc595n_init(&disp->sreg, 2, 1, 3, 0);
    display_clear(disp);
    display_show(disp);
}


/**
 * @brief Enables output on shift registers that control the display.
 * @param disp Pointer to display object.
 */
void display_show(display* disp) { 
    sn74hc595n_oe_lo(&disp->sreg);
    disp->show_state = true;
}


/**
 * @brief Disables output on shift registers that control the display.
 * @param disp Pointer to display object.
 */
void display_hide(display* disp) {
    sn74hc595n_oe_hi(&disp->sreg);
    disp->show_state = false;
}


/**
 * @brief Clears shift registers' data, thereby clearing the display.
 * @param disp Pointer to display object.
 */
void display_clear(display* disp) {
    disp->data = 0x0000;
    sn74hc595n_put16(&disp->sreg, 0x0000);
}


/**
 * @brief Writes two bytes directly to the display shift registers.
 * @param disp Pointer to display object.
 * @param data Raw bytes to send to shift register (LSB First)
 */
void display_write(display* disp, uint16_t data) {
    disp->data = data;
    sn74hc595n_put16(&disp->sreg, data);
}


/**
 * @brief Displays the hex representation of a byte on the display.
 * @param disp Pointer to display object.
 * @param byte Byte to display as hex.
 */
void display_hex(display* disp, uint8_t byte) {
    display_write(disp, byte2disp(byte));
}


/**
 * @brief Converts raw byte to Drako display bytes.
 * @param data Byte to convert to Drako display bytes.
 */
uint16_t byte2disp(uint8_t data) {
    uint16_t disp = 0x0000;

    // low-byte encode
    switch (data&0x0F) {
        case 0x00:
            disp |= DISP_LB_0;
            break;
        case 0x01:
            disp |= DISP_LB_1;
            break;
        case 0x02:
            disp |= DISP_LB_2;
            break;
        case 0x03:
            disp |= DISP_LB_3;
            break;
        case 0x04:
            disp |= DISP_LB_4;
            break;
        case 0x05:
            disp |= DISP_LB_5;
            break;
        case 0x06:
            disp |= DISP_LB_6;
            break;
        case 0x07:
            disp |= DISP_LB_7;
            break;
        case 0x08:
            disp |= DISP_LB_8;
            break;
        case 0x09:
            disp |= DISP_LB_9;
            break;
        case 0x0A:
            disp |= DISP_LB_A;
            break;
        case 0x0B:
            disp |= DISP_LB_B;
            break;
        case 0x0C:
            disp |= DISP_LB_C;
            break;
        case 0x0D:
            disp |= DISP_LB_D;
            break;
        case 0x0E:
            disp |= DISP_LB_E;
            break;
        case 0x0F:
            disp |= DISP_LB_F;
            break;
    }

    // high-byte encode
    switch (data&0xF0) {
        case 0x00:
            disp |= DISP_HB_0;
            break;
        case 0x10:
            disp |= DISP_HB_1;
            break;
        case 0x20:
            disp |= DISP_HB_2;
            break;
        case 0x30:
            disp |= DISP_HB_3;
            break;
        case 0x40:
            disp |= DISP_HB_4;
            break;
        case 0x50:
            disp |= DISP_HB_5;
            break;
        case 0x60:
            disp |= DISP_HB_6;
            break;
        case 0x70:
            disp |= DISP_HB_7;
            break;
        case 0x80:
            disp |= DISP_HB_8;
            break;
        case 0x90:
            disp |= DISP_HB_9;
            break;
        case 0xA0:
            disp |= DISP_HB_A;
            break;
        case 0xB0:
            disp |= DISP_HB_B;
            break;
        case 0xC0:
            disp |= DISP_HB_C;
            break;
        case 0xD0:
            disp |= DISP_HB_D;
            break;
        case 0xE0:
            disp |= DISP_HB_E;
            break;
        case 0xF0:
            disp |= DISP_HB_F;
            break;
    }

    return disp;
}
