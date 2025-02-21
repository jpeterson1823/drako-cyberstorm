#ifndef __drako_sn74hc595n
#define __drako_sn74hc595n


#include <hardware/gpio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#define SN74HC595N_DELAY_US 5


typedef struct sn74hc595n_struct {
    uint8_t ser;
    uint8_t rclk;
    uint8_t srclk;
    uint8_t oe;
} sn74hc595n;


void sn74hc595n_init(sn74hc595n* sreg, uint8_t ser, uint8_t rclk, uint8_t srclk, uint8_t oe);
void sn74hc595n_select(sn74hc595n* sreg);
void sn74hc595n_pulse_clock(sn74hc595n* sreg);
void sn74hc595n_latch(sn74hc595n* sreg);

void sn74hc595n_oe_hi(sn74hc595n* sreg);
void sn74hc595n_oe_lo(sn74hc595n* sreg);

void sn74hc595n_shift1(sn74hc595n* sreg, bool bit);
void sn74hc595n_shift8(sn74hc595n* sreg, uint8_t byte);
void sn74hc595n_shift16(sn74hc595n* sreg, uint16_t data);

void sn74hc595n_put1(sn74hc595n* sreg, bool bit);
void sn74hc595n_put8(sn74hc595n* sreg, uint8_t byte);
void sn74hc595n_put16(sn74hc595n* sreg, uint16_t data);

#endif
