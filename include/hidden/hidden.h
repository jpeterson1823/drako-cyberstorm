#ifndef __drako_hidden_challenge
#define __drako_hidden_challenge

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <drako/drako.h>

extern bool hc_c2_complete;
static const size_t hc_c2_length = 640;
static const uint16_t hc_c2_start = 0x094f + 1000;
static const uint16_t hc_c2_end   = hc_c2_start + hc_c2_length;

static inline void hc_c2_update_clearance() {
    at28c64b_select(&drako.prom);
    uint8_t byte;
    at28c64b_read8(&drako.prom, 0x1fff, &byte);
    if (byte == 0xFF) 
        hc_c2_complete = true;
    else
        hc_c2_complete = false;
}

#endif
