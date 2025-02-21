#include <drako/hardware/at28c64b.h>
#include <pico/stdlib.h>
#include <pico/rand.h>
#include <ctype.h>
#include <tusb.h>

#include <drako/drako.h>
#include <drako/commands.h>
//#include <drako/tests.h>
#include <hidden/hidden.h>
#include <level0/level.h>
#include <level1/level.h>
#include <level3/level.h>

// DO NOT REMOVE. This is extern'd, so removing it will blow everything up!
//Drako drako;

int main() {
    // initialize pico stdio and built-in led
    stdio_init_all();
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25,1);

    // initialize Drako
    drako_init();

    display_select(&drako.disp);
    display_write(&drako.disp, 0xFFFF);
    display_show(&drako.disp);

    // begin logic loop
    char input[DRAKO_BUFSIZE];
    while (!drako.exit_flag) {
        // run terminal sync to handle serial maintenance
        terminal_sync();

        // make sure terminal is connected. if it isnt, attempt reconnect.
        if (!terminal_is_connected())
            terminal_open_connection();

        // update hc clearance
        hc_c2_update_clearance();

        // prompt user and get command
        terminal_prompt();
        terminal_get_line(input, DRAKO_BUFSIZE);

        // check for eeprom reset command
        if (strcmp(input, "resetdrako") == 0) {
            drako_reset_eeprom();
            continue;
        }

        // execute command
        if (is_drako_cmd(input))
            exec_drako_cmd_str(input);

        // if non-drako command, then must be level specific
        else {
            // if on level 0, let level0 command handler take over
            if (drako.currentLevel == 0) {
                // if level0 is completed, call level1
                if (level0_cast(input))
                    level1();
            }

            else if (drako.currentLevel == 1) {
                level1_cast(input);
            }

            else if (drako.currentLevel == 3) {
                level3_main();
            }

            // hidden challenge part 3
            else if (drako.currentLevel == 255) {
                size_t i;
                char cpy[DRAKO_BUFSIZE];
                memset(cpy, 0, DRAKO_BUFSIZE);

                for (i = 0; i < strlen(input) && i < DRAKO_BUFSIZE; i++)
                    cpy[i] = input[i];
                cpy[DRAKO_BUFSIZE-1] = 0;

                char *token, *saveptr;
                token = strtok_r(cpy, " ", &saveptr);

                if (strcmp(token, "whatis") == 0) {
                    token = strtok_r(NULL, " ", &saveptr);
                    // force token to lowercase
                    for(i = 0; i < strlen(token); i++)
                        token[i] = tolower(token[i]);


                    // riddle incorrect
                    if (strcmp(token, "understanding") != 0) {
                        uint8_t delay;
                        at28c64b_select(&drako.prom);
                        at28c64b_read8(&drako.prom, AT28C64B_MAX_ADDR, &delay);
                        printf("Incorrect. Punishment: Timeout.\nChecking Timing Address: %us\nServe Your Sentence With Dignity.\n\n", delay);
                        sleep_ms(delay*1000);
                    }

                    // riddle correct
                    else {
                        printf("Indeed... I am Understanding.\n");
                        printf("I gift unto you the second key:\n\n        understanding\n\n");
                        printf("Go forth... Great power awaits you...\n");
                    }
                }
            }

            else {
                printf("Level %u not yet constructed!\n", drako.currentLevel);
            }
        }
    }

    // close serial connection
    tud_disconnect();

    // hide display
    display_select(&drako.disp);
    display_hide(&drako.disp);

    // turn off on-bard LED
    gpio_put(25, 0);

    return 0;
}
