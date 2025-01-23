#include <drako/modules/terminal.h>
#include <stdlib.h>


/**
 * @brief Cleans string of leading and tailing spaces, tabs, and/or newline chars.
 * @param str String to clean
 * @param buf Buffer to store cleaned string
 * @param nbuf Length of storage buffer
 */
void _terminal_clean_string(char* str, size_t slen) {
    // skip all leading unwanted chars and get pointer to first valid char
    size_t i_start;
    for (i_start = 0; i_start < slen; i_start++)
        if (_terminal_is_valid_char(str[i_start]))
            break;

    // get index of last valid char
    size_t i_end;
    for (i_end = strlen(str) - 1; i_end >= 0; i_end--)
        if (_terminal_is_valid_char(str[i_end]))
            break;

    // overwrite leading spaces and add null char after last valid char
    for (size_t i = 0; i < slen; i++) {
        // copy valid chars
        if (i_start <= i_end) {
            str[i] = str[i_start++];
        } else {
            str[i] = 0;
            break;
        }
    }
}


/**
 * @brief Reads in a line from terminal connection. Does not include return character.
 * @param buf String buffer to store read data in.
 * @param n   Length of provided buffer.
 */
void terminal_get_line(char* buf, size_t n) {
    // get the first char of line
    char c = getc(stdin);

    // read char into buf until newline is read or end of buffer is reached
    char* ptr = buf;
    while (c != '\n' || buf + n - 1 == ptr)
        *ptr++ = getc(stdin);

    // append null char to end of read-in string
    *ptr = 0;
}


/**
 * @brief Gets command from terminal.
 * @param tcmd Pointer to tcmd_t object.
 *
 * @note Make sure to free the tcmd_t after it has been used!
 */
void terminal_get_command(tcmd_t* tcmd) {
    // get line from terminal
    char buf[DRKO_TERM_BUFSIZE];
    terminal_get_line(buf, DRKO_TERM_BUFSIZE);

    // clean string
    _terminal_clean_string(buf, DRKO_TERM_BUFSIZE);

    // calc argc
    tcmd->argc = 0;
    for (size_t i = 0; i < strlen(buf); i++) {
        // count number of spaces
        if (buf[i] == ' ')
            tcmd->argc++;
    }
    // if last char is valid, then increment argc to account for trailing argument
    if (_terminal_is_valid_char(buf[strlen(buf-1)]))
        tcmd->argc++;

    // allocate tcmd_t memory
    _tcmd_alloc(tcmd);

    // TODO: split cleaned string into separate arguments
}
