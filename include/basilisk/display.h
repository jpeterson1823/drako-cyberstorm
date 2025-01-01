#ifndef __basilisk_display
#define __basilisk_display

#include <stdbool.h>
#include "basilisk/hardware/sn74hc595n.h"

// Least-Significant Digit
#define BSLK_DISPL_0 0x003F
#define BSLK_DISPL_1 0x0006
#define BSLK_DISPL_2 0x005B
#define BSLK_DISPL_3 0x004F
#define BSLK_DISPL_4 0x0066
#define BSLK_DISPL_5 0x006D
#define BSLK_DISPL_6 0x007D
#define BSLK_DISPL_7 0x0007
#define BSLK_DISPL_8 0x007F
#define BSLK_DISPL_9 0x006F
#define BSLK_DISPL_A 0x0077
#define BSLK_DISPL_B 0x007C
#define BSLK_DISPL_C 0x0039
#define BSLK_DISPL_D 0x005E
#define BSLK_DISPL_E 0x0079
#define BSLK_DISPL_F 0x0071
#define BSLK_DISPL_P 0x0080

// Most-Significant Digit
#define BSLK_DISPM_0 0x3F00
#define BSLK_DISPM_1 0x0600
#define BSLK_DISPM_2 0x5B00
#define BSLK_DISPM_3 0x4F00
#define BSLK_DISPM_4 0x6600
#define BSLK_DISPM_5 0x6D00
#define BSLK_DISPM_6 0x7D00
#define BSLK_DISPM_7 0x0700
#define BSLK_DISPM_8 0x7F00
#define BSLK_DISPM_9 0x6F00
#define BSLK_DISPM_A 0x7700
#define BSLK_DISPM_B 0x7C00
#define BSLK_DISPM_C 0x3900
#define BSLK_DISPM_D 0x5E00
#define BSLK_DISPM_E 0x7900
#define BSLK_DISPM_F 0x7100
#define BSLK_DISPM_P 0x8000

typedef struct __display_struct {
    shiftreg sreg;
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
    shiftreg_select(&disp->sreg);
    if (disp->show_state)
        display_show(disp);
    else
        display_hide(disp);
}

#endif
