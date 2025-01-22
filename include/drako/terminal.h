#ifndef __drako_terminal
#define __drako_terminal

#include <stdio.h>
#include <stdbool.h>

#include <pico/stdlib.h>
#include <tusb.h>




#define DRKO_TERM_BUFSIZE 256
#define DRKO_TERM "[-BASILISK-]"




typedef struct terminal_command_struct {
    char** argv;
    size_t argc;
} terminal_command;




void _terminal_clean_string(char* str, char* buf, size_t nbuf);

void terminal_get_line(char* buf, size_t n);
void terminal_get_command(terminal_command* tcmd);
void terminal_command_free(terminal_command* tcmd);




/**
 * @brief Determines if char is considered valid (not white space, etc)
 * @param c Character to verify
 * @return true if valid char, false otherwise.
 */
static inline bool _terminal_is_valid_char(char c) {
    return (c == ' ' || c == '\n' || c == '\t');
}




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




#endif
