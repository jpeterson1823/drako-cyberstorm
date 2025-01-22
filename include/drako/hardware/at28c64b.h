#ifndef __drako_at28c64b
#define __drako_at28c64b

#include <hardware/gpio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define EEPROM_CONDITION_DELAY_US 100

typedef struct at28c64b {
    uint32_t data_bus;
    uint32_t addr_bus;
    uint8_t we, oe, ce;
    bool data_dir;
} eeprom;

void _eeprom_gpio_init(eeprom* prom);
void _eeprom_data_in(eeprom* prom);
void _eeprom_data_out(eeprom* prom);
void _eeprom_set_idle_condition(eeprom* prom);
void _eeprom_set_read_condition(eeprom* prom);
void _eeprom_set_write_condition(eeprom* prom);
void _eeprom_execute_write(eeprom* prom);

void eeprom_init(eeprom* prom, uint32_t data_bus, uint32_t addr_bus, uint8_t we, uint8_t oe, uint8_t ce);
void eeprom_select(eeprom* prom);

void eeprom_read8(eeprom* prom, uint32_t addr, uint8_t* buff);
void eeprom_write8(eeprom* prom, uint32_t addr, uint8_t data);

#endif
