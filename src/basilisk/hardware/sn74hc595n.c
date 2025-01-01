#include "basilisk/hardware/sn74hc595n.h"
#include <pico/stdlib.h>

void shiftreg_init(shiftreg* sreg, uint8_t ser, uint8_t rclk, uint8_t srclk, uint8_t oe)
{
    // initialize struct
    sreg->ser = ser;
    sreg->rclk = rclk;
    sreg->srclk = srclk;
    sreg->oe = oe;

    // initialize gpio pins
    gpio_init(ser);
    gpio_init(rclk);
    gpio_init(srclk);
    gpio_init(oe);

    // set pins to output
    gpio_set_dir(ser,   GPIO_OUT);
    gpio_set_dir(rclk,  GPIO_OUT);
    gpio_set_dir(srclk, GPIO_OUT);
    gpio_set_dir(oe,    GPIO_OUT);

    // pull down active low
    gpio_pull_down(oe);

    // set default pin states
    gpio_put(ser,   0);
    gpio_put(rclk,  0);
    gpio_put(srclk, 0);
    gpio_put(oe, 1);
}

// Takes GPIO control for current shift register
void shiftreg_select(shiftreg* sreg) {
    // reset pin dirs
    gpio_set_dir(sreg->ser,   GPIO_OUT);
    gpio_set_dir(sreg->rclk,  GPIO_OUT);
    gpio_set_dir(sreg->srclk, GPIO_OUT);
    gpio_set_dir(sreg->oe,    GPIO_OUT);
    // set default pin states
    gpio_put(sreg->ser,   0);
    gpio_put(sreg->rclk,  0);
    gpio_put(sreg->srclk, 0);
    gpio_put(sreg->oe, 1);
}

// Pulses clock by SR_DELAY_US microseconds
void shiftreg_pulse_clock(shiftreg *sreg) {
    gpio_put(sreg->srclk, 1);
    sleep_us(SR_DELAY_US);
    gpio_put(sreg->srclk, 0);
    sleep_us(SR_DELAY_US);
}

// Latches current shift register's state to the output register
void shiftreg_latch(shiftreg* sreg) {
    gpio_put(sreg->rclk, 1);
    sleep_us(SR_DELAY_US);
    gpio_put(sreg->rclk, 0);
    sleep_us(SR_DELAY_US);
}

// Sets OE pin HIGH
void shiftreg_oe_hi(shiftreg* sreg) {
    gpio_put(sreg->oe, 1);
    sleep_us(SR_DELAY_US);
}

// Sets OE pin LOW
void shiftreg_oe_lo(shiftreg* sreg) {
    gpio_put(sreg->oe, 0);
    sleep_us(SR_DELAY_US);
}

// Loads a single bit into the shift register.
void shiftreg_shift1(shiftreg* sreg, bool bit) {
    gpio_put(sreg->ser, bit);
    shiftreg_pulse_clock(sreg);
}

// Loads byte into shift register without latching to output. LSB first
void shiftreg_shift8(shiftreg *sreg, uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        gpio_put(sreg->ser, byte&(0x80>>i));
        sleep_us(SR_DELAY_US);
        shiftreg_pulse_clock(sreg);
        sleep_us(SR_DELAY_US);
    }
}

// Loads 16-biy data into shift register without latching to output. LSB first
void shiftreg_shift16(shiftreg* sreg, uint16_t data) {
    shiftreg_shift8(sreg, data);
    shiftreg_shift8(sreg, data>>8);
}

// Loads AND latches a single bit into shift register.
void shiftreg_put1(shiftreg *sreg, bool bit) {
    shiftreg_shift1(sreg, bit);
    shiftreg_latch(sreg);
}

// Loads AND latches byte into shift register. LSB fisrt.
void shiftreg_put8(shiftreg *sreg, uint8_t byte) {
    shiftreg_shift8(sreg, byte);
    shiftreg_latch(sreg);
}

// Loads AND latches 16-bit data into shift register. LSB first.
void shiftreg_put16(shiftreg* sreg, uint16_t data) {
    shiftreg_shift16(sreg, data);
    shiftreg_latch(sreg);
}
