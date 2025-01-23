#include <drako/terminal.h>
#include <stdlib.h>




/**
 * @brief Cleans string of leading and tailing spaces, tabs, and/or newline chars.
 * @param str String to clean
 * @param buf Buffer to store cleaned string
 * @param nbuf Length of storage buffer
 */
void _terminal_clean_string(char* str, char* buf, size_t nbuf) {
    // skip all leading unwanted chars and get pointer to first valid char
    size_t i_start;
    for (i_start = 0; i_start < nbuf; i_start++)
        if (_terminal_is_valid_char(str[i_start]))
            break;

    // get index of last valid char
    size_t i_end;
    for (i_end = strlen(str) - 1; i_end >= 0; i_end--)
        if (_terminal_is_valid_char(str[i_end]))
            break;

    // copy valid part of string into buf
    strncpy(buf, str+i_start, i_end-i_start);
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
 * @brief Gets command from terminal in argc/argv format.
 * @param tcmd Pointer to terminal_command buffer.
 *
 * @note Do not forget to free the terminal_command object after using.
 */
void terminal_get_command(terminal_command* tcmd) {
    // get line from terminal
    char buf[DRKO_TERM_BUFSIZE];
    terminal_get_line(buf, DRKO_TERM_BUFSIZE);

    // clean string
    char clean[DRKO_TERM_BUFSIZE];
    _terminal_clean_string(buf, clean, DRKO_TERM_BUFSIZE);

    // calc argc
    tcmd->argc = 0;
    for (size_t i = 0; i < strlen(clean); i++) {
        // count number of spaces
        if (clean[i] == ' ')
            tcmd->argc++;
    }
    // if last char is valid, then increment argc to account for trailing argument
    if (_terminal_is_valid_char(clean[strlen(clean)-1]))
        tcmd->argc++;

    // allocate necessary memory for argv
    tcmd->argv = (char**)malloc(sizeof(char*) * tcmd->argc);

    // TODO: split cleaned string into separate arguments
}




void terminal_command_free(terminal_command* tcmd) {
    if (tcmd->argv != NULL)
        free(tcmd->argv);
}
