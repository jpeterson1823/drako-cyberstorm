#ifndef __basilisk_terminal
#define __basilisk_terminal


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <pico/stdlib.h>
#include <tusb.h>

#define BSLK_TERM "[-BASILISK-]"

inline void terminal_puts_raw(const char* str) {
}

inline void _terminal_greet() {
    printf("+---------------------------------+\n");
    printf("|                                 |\n");
    printf("|   BASILISK TERMINAL INTERFACE   |\n");
    printf("|                                 |\n");
    printf("+---------------------------------+\n");
    printf("%s CONNECTION: ESTABLISHED\n", BSLK_TERM);
    printf("%s Type 'commands' to view list of valid commands.", BSLK_TERM);
}

/**
 * @brief Open a terminal connection via STDIN (Serial). Operation is blocking.
 */
inline void terminal_open_connection() {
    // await connection
    while (!tud_cdc_connected()) {
        gpio_put(25, 1);
        sleep_ms(250);
        gpio_put(25, 0);
        sleep_ms(250);
    }

    // display greeting
    // display terminal promp
    printf("%s >$", BSLK_TERM);
}

/**
 * @brief Checks terminal connection.
 * @return true if connection is established. false otherwise.
 */
inline bool terminal_is_connected() {
    return tud_cdc_connected();
}

/**
 * @brief Reads in a line from terminal connection.
 * @param buf String buffer to store read data in.
 * @param n   Length of provided buffer.
 */
inline void terminal_get_line(char* buf, size_t n) {

}

#endif
