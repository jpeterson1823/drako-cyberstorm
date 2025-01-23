#ifndef __drako_terminal
#define __drako_terminal

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <pico/stdlib.h>
#include <tusb.h>


#define DRKO_TERM_BUFSIZE 256
#define DRKO_TERM "[-DRAKO-]"
#define DRKO_PROMPT "drako >$ "

static bool _term_connected = false;

typedef struct tcmd_struct {
    char**  argv;
    uint8_t argc;
} tcmd_t;


void _terminal_clean_string(char* str, size_t slen);
void terminal_get_line(char* buf, size_t n);
void terminal_get_command(tcmd_t* tcmd);



/**
 * @brief Determines if char is considered valid (not white space, etc)
 * @param c Character to verify
 * @return true if valid char, false otherwise.
 */
static inline bool _terminal_is_valid_char(char c) {
    return !(c == ' ' || c == '\n' || c == '\t');
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
    printf("%s Type 'commands' to view list of valid commands.\n", DRKO_TERM);
}


static inline void terminal_prompt() {
    printf("%s", DRKO_PROMPT);
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

    // connection established. update state
    _term_connected = true;

    // clear serial terminal
    sleep_ms(500);
    printf("\x1b");
    printf("[2J");
    sleep_ms(500);

    // turn on on-board LED
    gpio_put(25, 1);

    // display greeting
    _terminal_greet();
}


/**
 * @brief Runs required connection maintenance. Needs to be called periodically.
 */
static inline void terminal_sync() {
    // check for connection drop
    if (!tud_cdc_connected()) {
        _term_connected = false;
    }
}


/**
 * @brief Checks if terminal is currently connected to a serial session.
 */
static inline bool terminal_is_connected() {
    return _term_connected;
}


/**
 * @brief Allocates tcmd_t object.
 * @param tcmd Pointer to tcmd_t object.
 */
static inline void _tcmd_alloc(tcmd_t* tcmd) {
    if (tcmd->argc != 0)
        tcmd->argv = (char**)malloc(sizeof(char*) * tcmd->argc);
}


/**
 * @brief Free's allocated tcmd_t object.
 * @param tcmd Pointer to allocated tcmd_t object.
 */
static inline void tcmd_free(tcmd_t* tcmd) {
    while (tcmd->argc-- > 0)
        free(tcmd->argv[tcmd->argc]);
    free(tcmd->argv);
}


#endif
