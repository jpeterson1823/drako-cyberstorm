#include <drako/hardware/at28c64b.h>


/**
 * @brief Sets data bus to input mode
 * @param prom Pointer to EEPROM struct
 */
void _at28c64b_data_in(at28c64b* prom) {
    gpio_set_dir_in_masked(prom->data_bus);
    prom->data_dir = GPIO_IN;
    sleep_us(AT28C64B_CONDITION_DELAY_US);
}


/**
 * @brief Sets data bus to input mode
 * @param prom Pointer to EEPROM struct
 */
void _at28c64b_data_out(at28c64b* prom) {
    gpio_set_dir_out_masked(prom->data_bus);
    prom->data_dir = GPIO_OUT;
    sleep_us(AT28C64B_CONDITION_DELAY_US);
}


/**
 * @brief Initialize GPIO pins used for EEPROM
 * @param prom Pointer to EEPROM struct
**/
void _at28c64b_gpio_init(at28c64b* prom) {
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
void _at28c64b_set_idle_condition(at28c64b* prom) {
    gpio_put(prom->we, 1);
    gpio_put(prom->ce, 1);
    gpio_put(prom->oe, 1);
    sleep_us(AT28C64B_CONDITION_DELAY_US);
}


/**
 * @brief Sets EEPROM read condition. Must be called before executing a READ.
 * @param prom Pointer to EEPROM struct
 */
void _at28c64b_set_read_condition(at28c64b* prom) {
    gpio_put(prom->we, 1);
    gpio_put(prom->ce, 0);
    gpio_put(prom->oe, 0);
    //sleep_ms(10);
    sleep_us(AT28C64B_CONDITION_DELAY_US);
}


/**
 * @brief Sets EEPROM write condition. Must be called before executing a WRITE.
 * @param prom Pointer to EEPROM struct
 */
void _at28c64b_set_write_condition(at28c64b* prom) {
    gpio_put(prom->we, 1);
    gpio_put(prom->ce, 0);
    gpio_put(prom->oe, 1);
    sleep_us(AT28C64B_CONDITION_DELAY_US);
}


/**
 * @brief Executes WRITE. Must be called AFTER setting a WRITE condition.
 * @param prom Pointer to EEPROM struct
 */
void _at28c64b_execute_write(at28c64b* prom) {
    gpio_put(prom->we, 0);
    sleep_us(1);
    gpio_put(prom->we, 1);
    sleep_us(1);

    // datasheet specifies that write time can take 10ms MAX, so sleep for 10ms
    sleep_ms(10);
}
