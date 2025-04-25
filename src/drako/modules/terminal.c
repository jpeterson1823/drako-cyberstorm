#include <pico/time.h>
#include <drako/modules/terminal.h>
#include <drako/commands.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// define externs
bool _term_connected = false;

/**
 * @brief Reads in a line from terminal connection. Does not include return character.
 * @param buf String buffer to store read data in.
 * @param n   Length of provided buffer.
 */
void terminal_get_line(char* buf, size_t n) {
    // stack allocations
    size_t i = 0;
    char c;

    // fill buffer with nullchars and reset index variable
    for (i = 0; i < n; i++)
        buf[i] = 0;
    i = 0;

    // loop until line is read or buff runs out of space
    while (true) {
        // if no data available, wait until there is
        if (!tud_cdc_available())
            continue;

        // read char if available on rx fifo
        c = tud_cdc_read_char();

        // echo the char back to the terminal if buffer isnt full.
        if (i < n-1) {      // n-1 to guarantee null termination
            // if buffer is empty, dont echo backspace
            if (!(i <= 0 && _is_char_backspace(c))) {
                printf("%c", c);
                if (c == '\r')      // if carriage return, also print a newline
                    printf("\n");
            }
        }
        // if buffer is full, only allow backspace to echo
        else if (i >= n-1 && (c == 0x7f))   // n-1 to guarantee null termination
            printf("%c", c);

        // save char if it's a letter, number, or space AND buf has room. also increment index
        if (i < n-1 && (c == ' ' || isalnum(c) || c == '-' || c == '<' || c == '>'))    // n-1 to guarantee null termination
            buf[i++] = c;

        // if char is a backspace, move index backwards (if possible)
        else if ((c == 0x08 || c == 0x7f) && i > 0) {
            i--;
            // for minicom to display the backspace properly
            printf(" \b");
        }

        // if char is a newline or return char, then command is completed
        else if (c == '\n' || c == '\r') {
            buf[i] = 0;     // add null terminator
            break;          // break loop
        }
        // otherwise, do nothing to the char or with the buffer
    }
    // flush the rx fifo
    tud_cdc_read_flush();
}


/**
 * @brief Reads in a line from terminal connection. Does not include return character.
 * @param buf String buffer to store read data in.
 * @param n   Length of provided buffer.
 * @param timeout_ms Amount of time before returning false
 * @return false if timeout occurs. true otherwise.
 */
bool terminal_get_line_timed(char* buf, size_t n, uint32_t timeout_ms) {
    // stack allocations
    size_t i = 0;
    char c;

    // fill buffer with nullchars and reset index variable
    for (i = 0; i < n; i++)
        buf[i] = 0;
    i = 0;

    // get timestamp of timeout (current_time + timeout_ms)
    absolute_time_t timeout_timestamp = make_timeout_time_ms(timeout_ms);

    // loop until line is read, buff runs out of space, or timeout is reached
    while (true) {
        // if timeout occurs, flush rx fifo and return false
        if (timeout_timestamp < get_absolute_time()) {
            tud_cdc_read_flush();
            return false;
        }

        // if no data available, wait until there is
        if (!tud_cdc_available())
            continue;

        // read char if available on rx fifo
        c = tud_cdc_read_char();

        // echo the char back to the terminal if buffer isnt full.
        if (i < n-1) {      // n-1 to guarantee null termination
            // if buffer is empty, dont echo backspace
            if (!(i <= 0 && _is_char_backspace(c))) {
                printf("%c", c);
                if (c == '\r')      // if carriage return, also print a newline
                    printf("\n");
            }
        }
        // if buffer is full, only allow backspace to echo
        else if (i >= n-1 && (c == 0x7f))   // n-1 to guarantee null termination
            printf("%c", c);

        // save char if it's a letter, number, or space AND buf has room. also increment index
        if (i < n-1 && (c == ' ' || isalnum(c) || c == '-'))    // n-1 to guarantee null termination
            buf[i++] = c;

        // if char is a backspace, move index backwards (if possible)
        else if ((c == 0x08 || c == 0x7f) && i > 0) {
            i--;
            // for minicom to display the backspace properly
            printf(" \b");
        }

        // if char is a newline or return char, then command is completed
        else if (c == '\n' || c == '\r') {
            buf[i] = 0;     // add null terminator
            break;          // break loop
        }
        // otherwise, do nothing to the char or with the buffer
    }
    // flush the rx fifo
    tud_cdc_read_flush();
    return true;
}


/**
 * @brief Displays greeting.
 */
void _terminal_greet() {
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
    printf("%s Welcome to DRAKO OS. -DISCOVER- secrets; -UNDERSTAND- their meaning...\n", DRKO_TERM);
    printf("%s Type 'commands' to view list of valid commands.\n", DRKO_TERM);
}


/**
 * @brief Prints shell-like prompt to terminal
 */
void terminal_prompt() {
    printf("%s", DRKO_PROMPT);
}


/**
 * @brief Open a terminal connection via STDIN (Serial). Operation is blocking.
 */
void terminal_open_connection() {
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
void terminal_sync() {
    // check for connection drop
    if (!tud_cdc_connected()) {
        _term_connected = false;
    }
}


/**
 * @brief Checks if terminal is currently connected to a serial session.
 */
bool terminal_is_connected() {
    return _term_connected;
}


/**
 * @brief Name says it all. It also adds a terminal prefix.
 * @param str String to print to terminal.
 */
void terminal_print(const char* str) {
    printf("%s %s", DRKO_TERM, str);
}


/**
 * @brief Really? You got this one. It also adds a terminal prefix.
 * @param str String to print to terminal (with new line attached).
 */
void terminal_println(const char* str) {
    printf("%s %s\n", DRKO_TERM, str);
}

bool _is_char_backspace(char c) {
    return c == 0x08 || c == 0x7f;
}
