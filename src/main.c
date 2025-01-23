#include <pico/stdlib.h>
#include <pico/rand.h>
#include <stdio.h>
#include <tusb.h>

#include <drako/drako.h>
#include <drako/default_commands.h>


int main() {
    // initialize pico stdio and built-in led
    stdio_init_all();
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    // initialize Drako
    drako_init();

    // begin logic loop
    tcmd_t tcmd;    // terminal command
    while (true) {
        // run terminal sync to handle serial maintenance
        terminal_sync();

        // make sure terminal is connected. if it isnt, attempt reconnect.
        if (!terminal_is_connected())
            terminal_open_connection();
        
        // prompt user and get command
        terminal_prompt();
        terminal_get_command(&tcmd);

        // handle commands here
        if(!is_default_cmd(tcmd.argv[0])) {
            printf("\"%s\" is not a default command.\n", tcmd.argv[0]);
            printf("Number of arguments provided: %d\n", tcmd.argc);
        }

        // free tcmd after it's been handled
        tcmd_free(&tcmd);
    }


    // turn off on-bard LED
    gpio_put(25, 0);

    return 0;
}
