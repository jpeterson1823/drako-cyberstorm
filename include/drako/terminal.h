#ifndef __drako_terminal
#define __drako_terminal


#include <stdio.h>
#include <stdbool.h>

#include <pico/stdlib.h>
#include <tusb.h>

#define DRKO_TERM "[-BASILISK-]"

/**
 * @brief Displays greeting.
 */
static inline void _terminal_greet() {
    printf(
        "|==========================================================================|\n"
        "||------------------------------------------------------------------------||\n"
        "||                                                                        ||\n"
        "||   ██████████   ███████████     █████████   █████   ████    ███████     ||\n"
        "||  ░░███░░░░███ ░░███░░░░░███   ███░░░░░███ ░░███   ███░   ███░░░░░███   ||\n"
        "||   ░███   ░░███ ░███    ░███  ░███    ░███  ░███  ███    ███     ░░███  ||\n"
        "||   ░███    ░███ ░██████████   ░███████████  ░███████    ░███      ░███  ||\n"
        "||   ░███    ░███ ░███░░░░░███  ░███░░░░░███  ░███░░███   ░███      ░███  ||\n"
        "||   ░███    ███  ░███    ░███  ░███    ░███  ░███ ░░███  ░░███     ███   ||\n"
        "||   ██████████   █████   █████ █████   █████ █████ ░░████ ░░░███████░    ||\n"
        "||  ░░░░░░░░░░   ░░░░░   ░░░░░ ░░░░░   ░░░░░ ░░░░░   ░░░░    ░░░░░░░      ||\n"
        "||                                                                        ||\n"
        "||------------------------------------------------------------------------||\n"
        "|==========================================================================|\n"
    );
    printf("%s CONNECTION: ESTABLISHED\n", DRKO_TERM);
    printf("%s Welcome to DRAKO OS.\n", DRKO_TERM);
    printf("%s Type 'commands' to view list of valid commands.", DRKO_TERM);
}

/**
 * @brief Open a terminal connection via STDIN (Serial). Operation is blocking.
 */
static inline void terminal_open_connection() {
    // await connection
    while (!tud_cdc_connected()) {
        gpio_put(25, 1);
        sleep_ms(250);
        gpio_put(25, 0);
        sleep_ms(250);
    }

    // display greeting
    _terminal_greet();

    // display terminal promp
    printf("%s >$", DRKO_TERM);
}

/**
 * @brief Checks terminal connection.
 * @return true if connection is currently established. false otherwise.
 */
static inline bool terminal_is_connected() {
    return tud_cdc_connected();
}

/**
 * @brief Reads in a line from terminal connection. Does not include return character.
 * @param buf String buffer to store read data in.
 * @param n   Length of provided buffer.
 */
static inline void terminal_get_line(char* buf, size_t n) {
    // get the first char of line
    char c = getc(stdin);

    // read char into buf until newline is read or end of buffer is reached
    char* ptr = buf;
    while (c != '\n' || buf + n - 1 == ptr)
        *ptr++ = getc(stdin);

    // append null char to end of read-in string
    *ptr = 0;
}

#endif
