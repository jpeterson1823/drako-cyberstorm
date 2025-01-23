#ifndef __drako_sn74hc595n
#define __drako_sn74hc595n


#include <stdint.h>
#include <stdbool.h>


#define SR_DELAY_US 10


typedef struct sn74hc595n {
    uint8_t ser;
    uint8_t rclk;
    uint8_t srclk;
    uint8_t oe;
} shiftreg;


void shiftreg_init(shiftreg* sreg, uint8_t ser, uint8_t rclk, uint8_t srclk, uint8_t oe);
void shiftreg_select(shiftreg* sreg);
void shiftreg_pulse_clock(shiftreg* sreg);
void shiftreg_latch(shiftreg* sreg);

void shiftreg_oe_hi(shiftreg* sreg);
void shiftreg_oe_lo(shiftreg* sreg);

void shiftreg_shift1(shiftreg* sreg, bool bit);
void shiftreg_shift8(shiftreg* sreg, uint8_t byte);
void shiftreg_shift16(shiftreg* sreg, uint16_t data);

void shiftreg_put1(shiftreg* sreg, bool bit);
void shiftreg_put8(shiftreg* sreg, uint8_t byte);
void shiftreg_put16(shiftreg* sreg, uint16_t data);

#endif
