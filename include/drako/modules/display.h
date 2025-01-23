#ifndef __drako_display
#define __drako_display


#include <stdbool.h>
#include <drako/hardware/sn74hc595n.h>


// Least-Significant Digit
enum DispLowByte {
    DISP_LB_0 = 0x003F,
    DISP_LB_1 = 0x0006,
    DISP_LB_2 = 0x005B,
    DISP_LB_3 = 0x004F,
    DISP_LB_4 = 0x0066,
    DISP_LB_5 = 0x006D,
    DISP_LB_6 = 0x007D,
    DISP_LB_7 = 0x0007,
    DISP_LB_8 = 0x007F,
    DISP_LB_9 = 0x006F,
    DISP_LB_A = 0x0077,
    DISP_LB_B = 0x007C,
    DISP_LB_C = 0x0039,
    DISP_LB_D = 0x005E,
    DISP_LB_E = 0x0079,
    DISP_LB_F = 0x0071,
    DISP_LB_P = 0x0080
};


// Most-Significant Digit
enum DispHighByte {
    DISP_HB_0 = 0x3F00,
    DISP_HB_1 = 0x0600,
    DISP_HB_2 = 0x5B00,
    DISP_HB_3 = 0x4F00,
    DISP_HB_4 = 0x6600,
    DISP_HB_5 = 0x6D00,
    DISP_HB_6 = 0x7D00,
    DISP_HB_7 = 0x0700,
    DISP_HB_8 = 0x7F00,
    DISP_HB_9 = 0x6F00,
    DISP_HB_A = 0x7700,
    DISP_HB_B = 0x7C00,
    DISP_HB_C = 0x3900,
    DISP_HB_D = 0x5E00,
    DISP_HB_E = 0x7900,
    DISP_HB_F = 0x7100,
    DISP_HB_P = 0x8000
};


typedef struct __display_struct {
    sn74hc595n sreg;
    uint16_t data;
    bool show_state;
} display;


void display_init(display* disp);
void display_show(display* disp);
void display_hide(display* disp);
void display_clear(display* disp);
void display_write(display* disp, uint16_t data);
void display_hex(display* disp, uint8_t byte);
uint16_t byte2disp(uint8_t data);

static inline void display_select(display* disp) {
    sn74hc595n_select(&disp->sreg);
    if (disp->show_state)
        display_show(disp);
    else
        display_hide(disp);
}

#endif
