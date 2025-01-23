#include <pico/stdlib.h>
#include <pico/rand.h>
#include <stdio.h>
#include <tusb.h>

#include <drako/hardware/at28c64b.h>
#include <drako/modules/display.h>
#include <drako/modules/terminal.h>
#include <drako/tests.h>


int main() {
    stdio_init_all();
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    // wait for serial connection
    while (!tud_cdc_connected()) {
        gpio_put(25, 1);
        sleep_ms(250);
        gpio_put(25, 0);
        sleep_ms(250);
    }
    // clear serial terminal
    sleep_ms(500);
    printf("\x1b");
    printf("[2J");
    sleep_ms(500);

    // turn on on-board LED
    gpio_put(25, 1);

    // create eeprom struct and init
    eeprom prom;
    eeprom_init(&prom,
        0x000000FF,     // data bus on GPIO[0:7]
        0x001FFF00,     // addr bus on GPIO[8:20]
        26, 22, 21
    );

    // create display struct and init
    display disp;
    display_init(&disp);

    // run full test for 1000 randomized eeprom locations
    randomized_full_test(&prom, &disp, 1000);

    // turn off on-bard LED
    gpio_put(25, 0);

    return 0;
}
