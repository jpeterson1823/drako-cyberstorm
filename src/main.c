#include <pico/stdlib.h>
#include <pico/rand.h>
#include <stdio.h>
#include <tusb.h>

#include <drako/hardware/at28c64b.h>
#include <drako/display.h>
#include <drako/tests.h>

// uncomment to see full printout of randomized_full_test
//#define RNG_FULL_TEST_DEBUG

void display_shell(display* disp);




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

    // run full test
    //full_test(&prom, &disp);
    randomized_full_test(&prom, &disp, 1000);

    // turn off on-bard LED
    gpio_put(25, 0);

    return 0;
}




void display_shell(display* disp) {
    char dbuf[2];
    uint16_t mdata, ldata;
    while (true) {
        dbuf[1] = getchar();
        display_write(disp, 0x1c1c);
        dbuf[0] = getchar();
        switch (dbuf[0]) {
            case '0':
                ldata = DRKO_DISPL_0;
                break;
            case '1':
                ldata = DRKO_DISPL_1;
                break;
            case '2':
                ldata = DRKO_DISPL_2;
                break;
            case '3':
                ldata = DRKO_DISPL_3;
                break;
            case '4':
                ldata = DRKO_DISPL_4;
                break;
            case '5':
                ldata = DRKO_DISPL_5;
                break;
            case '6':
                ldata = DRKO_DISPL_6;
                break;
            case '7':
                ldata = DRKO_DISPL_7;
                break;
            case '8':
                ldata = DRKO_DISPL_8;
                break;
            case '9':
                ldata = DRKO_DISPL_9;
                break;
            case 'a':
                ldata = DRKO_DISPL_A;
                break;
            case 'b':
                ldata = DRKO_DISPL_B;
                break;
            case 'c':
                ldata = DRKO_DISPL_C;
                break;
            case 'd':
                ldata = DRKO_DISPL_D;
                break;
            case 'e':
                ldata = DRKO_DISPL_E;
                break;
            case 'f':
                ldata = DRKO_DISPL_F;
                break;
            default:
                ldata = DRKO_DISPL_P;
                break;
        };
        switch (dbuf[1]) {
            case '0':
                mdata = DRKO_DISPM_0;
                break;
            case '1':
                mdata = DRKO_DISPM_1;
                break;
            case '2':
                mdata = DRKO_DISPM_2;
                break;
            case '3':
                mdata = DRKO_DISPM_3;
                break;
            case '4':
                mdata = DRKO_DISPM_4;
                break;
            case '5':
                mdata = DRKO_DISPM_5;
                break;
            case '6':
                mdata = DRKO_DISPM_6;
                break;
            case '7':
                mdata = DRKO_DISPM_7;
                break;
            case '8':
                mdata = DRKO_DISPM_8;
                break;
            case '9':
                mdata = DRKO_DISPM_9;
                break;
            case 'a':
                mdata = DRKO_DISPM_A;
                break;
            case 'b':
                mdata = DRKO_DISPM_B;
                break;
            case 'c':
                mdata = DRKO_DISPM_C;
                break;
            case 'd':
                mdata = DRKO_DISPM_D;
                break;
            case 'e':
                mdata = DRKO_DISPM_E;
                break;
            case 'f':
                mdata = DRKO_DISPM_F;
                break;
            default:
                mdata = DRKO_DISPM_P;
                break;
        };

        display_write(disp, mdata|ldata);
    }
}

