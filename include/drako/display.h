#ifndef __drako_display
#define __drako_display

#include <stdbool.h>
#include "drako/hardware/sn74hc595n.h"

// Least-Significant Digit
#define DRKO_DISPL_0 0x003F
#define DRKO_DISPL_1 0x0006
#define DRKO_DISPL_2 0x005B
#define DRKO_DISPL_3 0x004F
#define DRKO_DISPL_4 0x0066
#define DRKO_DISPL_5 0x006D
#define DRKO_DISPL_6 0x007D
#define DRKO_DISPL_7 0x0007
#define DRKO_DISPL_8 0x007F
#define DRKO_DISPL_9 0x006F
#define DRKO_DISPL_A 0x0077
#define DRKO_DISPL_B 0x007C
#define DRKO_DISPL_C 0x0039
#define DRKO_DISPL_D 0x005E
#define DRKO_DISPL_E 0x0079
#define DRKO_DISPL_F 0x0071
#define DRKO_DISPL_P 0x0080

// Most-Significant Digit
#define DRKO_DISPM_0 0x3F00
#define DRKO_DISPM_1 0x0600
#define DRKO_DISPM_2 0x5B00
#define DRKO_DISPM_3 0x4F00
#define DRKO_DISPM_4 0x6600
#define DRKO_DISPM_5 0x6D00
#define DRKO_DISPM_6 0x7D00
#define DRKO_DISPM_7 0x0700
#define DRKO_DISPM_8 0x7F00
#define DRKO_DISPM_9 0x6F00
#define DRKO_DISPM_A 0x7700
#define DRKO_DISPM_B 0x7C00
#define DRKO_DISPM_C 0x3900
#define DRKO_DISPM_D 0x5E00
#define DRKO_DISPM_E 0x7900
#define DRKO_DISPM_F 0x7100
#define DRKO_DISPM_P 0x8000

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
