#include <pico/stdlib.h>
#include <pico/rand.h>
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

        // prompt user and get command
        terminal_prompt();
        terminal_get_line(input, DRAKO_BUFSIZE);

        // execute command
        if (is_default_cmd(input))
            exec_default_cmd_str(input);
        else
            printf("Unknown command \"%s\"\n", input);
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
