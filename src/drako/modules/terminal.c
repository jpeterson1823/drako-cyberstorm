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
    while (i_start <= i_end) {
        // copy valid chars
        if (i_start <= i_end) {
            *str = str[i_start++];
            str++;
        } else {
            *str = 0;
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
    size_t i = 0;
    char c;

    // loop until newline is received
    while (true) {
        // if data is available on rx fifo, read in chars
        if (tud_cdc_available()) {
            c = tud_cdc_read_char();

            // echo the char back to terminal
            // if it's a carriage return, echo a newline with it
            printf("%c", c);
            if (c == '\r')
                printf("\n");

            // if char is a newline or carriage return, then command is finished
            if (c == '\n' || c == '\r') {
                // flush rx fifo and break loop
                tud_cdc_read_flush();
                // add null terminator to buffer
                buf[i] = 0;
                // break while loop
                break;
            }
            // if it isnt a new line, add it to the buffer if there is room
            if (i < n)
                buf[i++] = c;
        }
        // if no data available, wait until there is
    }
    //printf("terminal_get_line(...) : buf = %s\n", buf);
}


/**
 * @brief Gets command from terminal.
 * @param tcmd Pointer to tcmd_t object.
 *
 * @note Make sure to free the tcmd_t after it has been used!
 */
void terminal_get_command(tcmd_t* tcmd) {
    // stack allocation
    char buf[DRKO_TERM_BUFSIZE];
    char* token;
    size_t i;

    // get line from terminal
    terminal_get_line(buf, DRKO_TERM_BUFSIZE);

    // clean string WARNING: THIS SEGFAULTS AND PROBABLY ISNT NEEDED!
    //_terminal_clean_string(buf, DRKO_TERM_BUFSIZE);
    //printf("terminal_get_command(...) : buf after cleaning = %s\n", buf);

    // calc argc
    tcmd->argc = 0;
    for (i = 0; i < strlen(buf); i++) {
        // count number of spaces
        if (buf[i] == ' ')
            tcmd->argc++;
    }
    // if last char is valid, then increment argc to account for trailing argument
    if (_terminal_is_valid_char(buf[strlen(buf)-1]))
        tcmd->argc++;

    fflush(stdout);

    // allocate tcmd_t memory
    _tcmd_alloc(tcmd);

    // tokenize arguments and copy them into tcmd
    token = strtok(buf, " ");
    for (i = 0; i < tcmd->argc && token != NULL; i++) {
        // allocate memory for string and copy arg to tcmd
        tcmd->argv[i] = (char*)malloc(sizeof(char) * strlen(token)+1);
        strcpy(tcmd->argv[i], token);

        // tokenize next argument
        token = strtok(NULL, " ");
    }
}
