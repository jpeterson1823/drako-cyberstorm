#include "at28c64b.h"
#include <pico/stdlib.h>
#include <pico/rand.h>
#include <stdio.h>
#include <tusb.h>

#include <drako/drako.h>
#include <drako/default_commands.h>
#include <drako/tests.h>

// DO NOT REMOVE. This is extern'd, so removing it will blow everything up!
Drako drako;

int main() {
    // initialize pico stdio and built-in led
    stdio_init_all();
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    // initialize Drako
    drako_init();



    // begin logic loop
    tcmd_t tcmd;
    while (!drako.exit_flag) {
        // run terminal sync to handle serial maintenance
        terminal_sync();

        // make sure terminal is connected. if it isnt, attempt reconnect.
        if (!terminal_is_connected())
            terminal_open_connection();

        // get line from user
        //terminal_get_line(buf, buflen);
        //printf("You typed: \"%s\"\n", buf);
        terminal_prompt();
        terminal_get_command(&tcmd);
        tcmd_println(&tcmd);
    }

    // close serial connection
    tud_disconnect();

    // turn off on-bard LED
    gpio_put(25, 0);

    return 0;
}
