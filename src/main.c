#include <pico/stdlib.h>
#include <stdio.h>
#include <basilisk/hardware/sn74hc595n.h>
#include <basilisk/display.h>

void display_shell(display* disp);


int main() {
    stdio_init_all();
    sleep_ms(2000);

    // create display struct and init the shift register struct
    display disp;
    display_init(&disp);

    // Turn on built-in LED
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    // Start Serial display shell
    display_shell(&disp);

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
                ldata = BSLK_DISPL_0;
                break;
            case '1':
                ldata = BSLK_DISPL_1;
                break;
            case '2':
                ldata = BSLK_DISPL_2;
                break;
            case '3':
                ldata = BSLK_DISPL_3;
                break;
            case '4':
                ldata = BSLK_DISPL_4;
                break;
            case '5':
                ldata = BSLK_DISPL_5;
                break;
            case '6':
                ldata = BSLK_DISPL_6;
                break;
            case '7':
                ldata = BSLK_DISPL_7;
                break;
            case '8':
                ldata = BSLK_DISPL_8;
                break;
            case '9':
                ldata = BSLK_DISPL_9;
                break;
            case 'a':
                ldata = BSLK_DISPL_A;
                break;
            case 'b':
                ldata = BSLK_DISPL_B;
                break;
            case 'c':
                ldata = BSLK_DISPL_C;
                break;
            case 'd':
                ldata = BSLK_DISPL_D;
                break;
            case 'e':
                ldata = BSLK_DISPL_E;
                break;
            case 'f':
                ldata = BSLK_DISPL_F;
                break;
            default:
                ldata = BSLK_DISPL_P;
                break;
        };
        switch (dbuf[1]) {
            case '0':
                mdata = BSLK_DISPM_0;
                break;
            case '1':
                mdata = BSLK_DISPM_1;
                break;
            case '2':
                mdata = BSLK_DISPM_2;
                break;
            case '3':
                mdata = BSLK_DISPM_3;
                break;
            case '4':
                mdata = BSLK_DISPM_4;
                break;
            case '5':
                mdata = BSLK_DISPM_5;
                break;
            case '6':
                mdata = BSLK_DISPM_6;
                break;
            case '7':
                mdata = BSLK_DISPM_7;
                break;
            case '8':
                mdata = BSLK_DISPM_8;
                break;
            case '9':
                mdata = BSLK_DISPM_9;
                break;
            case 'a':
                mdata = BSLK_DISPM_A;
                break;
            case 'b':
                mdata = BSLK_DISPM_B;
                break;
            case 'c':
                mdata = BSLK_DISPM_C;
                break;
            case 'd':
                mdata = BSLK_DISPM_D;
                break;
            case 'e':
                mdata = BSLK_DISPM_E;
                break;
            case 'f':
                mdata = BSLK_DISPM_F;
                break;
            default:
                mdata = BSLK_DISPM_P;
                break;
        };

        display_write(disp, mdata|ldata);
    }
}
