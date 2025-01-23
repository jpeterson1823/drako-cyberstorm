#ifndef __drako_at28c64b
#define __drako_at28c64b

#include <hardware/gpio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define EEPROM_CONDITION_DELAY_US 100

typedef struct at28c64b_struct {
    uint32_t data_bus;
    uint32_t addr_bus;
    uint8_t we, oe, ce;
    bool data_dir;
} at28c64b;

void _at28c64b_gpio_init(at28c64b* prom);
void _at28c64b_data_in(at28c64b* prom);
void _at28c64b_data_out(at28c64b* prom);
void _at28c64b_set_idle_condition(at28c64b* prom);
void _at28c64b_set_read_condition(at28c64b* prom);
void _at28c64b_set_write_condition(at28c64b* prom);
void _at28c64b_execute_write(at28c64b* prom);

void at28c64b_init(at28c64b* prom, uint32_t data_bus, uint32_t addr_bus, uint8_t we, uint8_t oe, uint8_t ce);
void at28c64b_select(at28c64b* prom);

void at28c64b_read8(at28c64b* prom, uint32_t addr, uint8_t* buff);
void at28c64b_write8(at28c64b* prom, uint32_t addr, uint8_t data);

#endif
