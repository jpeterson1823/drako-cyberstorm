#include "drako/hardware/at28c64b.h"

/**
 * @brief Sets data bus to input mode
 * @param prom Pointer to EEPROM struct
 */
void _eeprom_data_in(eeprom* prom) {
    gpio_set_dir_in_masked(prom->data_bus);
    prom->data_dir = GPIO_IN;
}


/**
 * @brief Sets data bus to input mode
 * @param prom Pointer to EEPROM struct
 */
void _eeprom_data_out(eeprom* prom) {
    gpio_set_dir_out_masked(prom->data_bus);
    prom->data_dir = GPIO_OUT;
    sleep_us(EEPROM_CONDITION_DELAY_US);
}


/**
 * @brief Initialize GPIO pins used for EEPROM
 * @param prom Pointer to EEPROM struct
**/
void _eeprom_gpio_init(eeprom* prom) {
    // init control pins
    gpio_init(prom->we);
    gpio_init(prom->oe);
    gpio_init(prom->ce);

    // initialize data and address bus
    gpio_init_mask(prom->addr_bus | prom->data_bus);
}


/**
 * @brief Sets all control pins to HIGH (inactive)
 * @param prom Pointer to EEPROM struct
 */
void _eeprom_set_idle_condition(eeprom* prom) {
    gpio_put(prom->we, 1);
    gpio_put(prom->ce, 1);
    gpio_put(prom->oe, 1);
    sleep_us(EEPROM_CONDITION_DELAY_US);
}


/**
 * @brief Sets EEPROM read condition. Must be called before executing a READ.
 * @param prom Pointer to EEPROM struct
 */
void _eeprom_set_read_condition(eeprom* prom) {
    gpio_put(prom->we, 1);
    gpio_put(prom->ce, 0);
    gpio_put(prom->oe, 0);
    //sleep_ms(10);
    sleep_us(EEPROM_CONDITION_DELAY_US);
}


/**
 * @brief Sets EEPROM write condition. Must be called before executing a WRITE.
 * @param prom Pointer to EEPROM struct
 */
void _eeprom_set_write_condition(eeprom* prom) {
    gpio_put(prom->we, 1);
    gpio_put(prom->ce, 0);
    gpio_put(prom->oe, 1);
    sleep_us(EEPROM_CONDITION_DELAY_US);
}


/**
 * @brief Executes WRITE. Must be called AFTER setting a WRITE condition.
 * @param prom Pointer to EEPROM struct
 */
void _eeprom_execute_write(eeprom* prom) {
    gpio_put(prom->we, 0);
    sleep_us(1);
    gpio_put(prom->we, 1);
    sleep_us(1);

    // datasheet specifies that write time can take 10ms MAX, so sleep for 10ms
    sleep_ms(10);
}
