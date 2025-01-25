#include <drako/hardware/sn74hc595n.h>
#include <pico/stdlib.h>


void sn74hc595n_init(sn74hc595n* sreg, uint8_t ser, uint8_t rclk, uint8_t srclk, uint8_t oe) {
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
    //gpio_pull_down(oe);

    // set default pin states
    gpio_put(ser,   0);
    gpio_put(rclk,  0);
    gpio_put(srclk, 0);
    gpio_put(oe, 1);
}


// Takes GPIO control for current shift register
void sn74hc595n_select(sn74hc595n* sreg) {
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


// Pulses clock by SN74HC595N_DELAY_US microseconds
void sn74hc595n_pulse_clock(sn74hc595n *sreg) {
    gpio_put(sreg->srclk, 1);
    sleep_us(SN74HC595N_DELAY_US);
    gpio_put(sreg->srclk, 0);
    sleep_us(SN74HC595N_DELAY_US);
}


// Latches current shift register's state to the output register
void sn74hc595n_latch(sn74hc595n* sreg) {
    gpio_put(sreg->rclk, 1);
    sleep_us(SN74HC595N_DELAY_US);
    gpio_put(sreg->rclk, 0);
    sleep_us(SN74HC595N_DELAY_US);
}


// Sets OE pin HIGH
void sn74hc595n_oe_hi(sn74hc595n* sreg) {
    gpio_put(sreg->oe, 1);
    sleep_us(SN74HC595N_DELAY_US);
}


// Sets OE pin LOW
void sn74hc595n_oe_lo(sn74hc595n* sreg) {
    gpio_put(sreg->oe, 0);
    sleep_us(SN74HC595N_DELAY_US);
}


// Loads a single bit into the shift register.
void sn74hc595n_shift1(sn74hc595n* sreg, bool bit) {
    gpio_put(sreg->ser, bit);
    sn74hc595n_pulse_clock(sreg);
}


// Loads byte into shift register without latching to output. LSB first
void sn74hc595n_shift8(sn74hc595n *sreg, uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        gpio_put(sreg->ser, byte&(0x80>>i));
        sleep_us(SN74HC595N_DELAY_US);
        sn74hc595n_pulse_clock(sreg);
        sleep_us(SN74HC595N_DELAY_US);
    }
}


// Loads 16-biy data into shift register without latching to output. LSB first
void sn74hc595n_shift16(sn74hc595n* sreg, uint16_t data) {
    sn74hc595n_shift8(sreg, data);
    sn74hc595n_shift8(sreg, data>>8);
}


// Loads AND latches a single bit into shift register.
void sn74hc595n_put1(sn74hc595n *sreg, bool bit) {
    sn74hc595n_shift1(sreg, bit);
    sn74hc595n_latch(sreg);
}


// Loads AND latches byte into shift register. LSB fisrt.
void sn74hc595n_put8(sn74hc595n *sreg, uint8_t byte) {
    sn74hc595n_shift8(sreg, byte);
    sn74hc595n_latch(sreg);
}


// Loads AND latches 16-bit data into shift register. LSB first.
void sn74hc595n_put16(sn74hc595n* sreg, uint16_t data) {
    sn74hc595n_shift16(sreg, data);
    sn74hc595n_latch(sreg);
}
