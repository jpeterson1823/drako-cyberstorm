#include "basilisk/hardware/at28c64b.h"

/**
 * @brief Initialize a previously allocated EEPROM struct
 * @param prom      Pointer to EEPROM struct
 * @param data_bus  GPIO mask of pins used for EEPROM I/O
 * @param addr_bus  GPIO mask of pins used for EEPROM addressing
 * @param we        Write Enable GPIO pin
 * @param oe        Output Enable GPIO pin
 * @param ce        Chip Enable GPIO pin
 */
void eeprom_init(eeprom* prom, uint32_t data_bus, uint32_t addr_bus, uint8_t we, uint8_t oe, uint8_t ce) {
    prom->data_bus = data_bus;
    prom->addr_bus = addr_bus;
    prom->we = we;
    prom->oe = oe;
    prom->ce = ce;
    prom->data_dir = GPIO_OUT;

    _eeprom_gpio_init(prom);
}


/**
 * @brief Allows EEPROM to take control of shared GPIO pins.
 * @param prom Pointer to EEPROM struct
 */
void eeprom_select(eeprom* prom) {
    // set direction of control pins
    gpio_set_dir(prom->we, GPIO_OUT);
    gpio_set_dir(prom->oe, GPIO_OUT);
    gpio_set_dir(prom->ce, GPIO_OUT);

    // set direction of buses
    gpio_set_dir_out_masked(prom->addr_bus);
    if (prom->data_dir == GPIO_OUT)
        gpio_set_dir_out_masked(prom->data_bus);
    else
        gpio_set_dir_in_masked(prom->data_bus);
    
    // set eeprom to idle state
    _eeprom_set_idle_condition(prom);
}


/**
 * @brief Reads byte at specified address into buffer
 * @param prom Pointer to EEPROM struct
 * @param addr Address to read from
 * @param buff Buffer for storing read data
 */
void eeprom_read8(eeprom* prom, uint32_t addr, uint8_t* buff) {
    // set data bus to input
    _eeprom_data_in(prom);

    // put address out on addr_bus
    gpio_put_masked(prom->addr_bus, addr<<8);

    // set read condition for EEPROM
    _eeprom_set_read_condition(prom);
    sleep_ms(1);

    // read all GPIO pins, allowing for data truncation
    // since only the lower 8 bits are of interest
    *buff = gpio_get_all();
    
    // return to idle condition
    _eeprom_set_idle_condition(prom);
}


/**
 * @brief Write byte to EEPROM at specified address
 * @param prom Pointer to EEPROM struct
 * @param addr Address at which to write data
 * @param data Byte data to write to address
 */
void eeprom_write8(eeprom* prom, uint32_t addr, uint8_t data) {
    // put address and data out on their respective buses
    gpio_put_masked(prom->addr_bus, addr<<8);
    gpio_put_masked(prom->data_bus, data);

    _eeprom_set_write_condition(prom);
    _eeprom_execute_write(prom);

    // return to idle condition
    _eeprom_set_idle_condition(prom);
}
