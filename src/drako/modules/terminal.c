#include <drako/modules/terminal.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// define externs
bool _term_connected = false;
char _terminal_cmdbuf[DRKO_TERM_BUFSIZE];

/**
 * @brief Cleans string of leading and tailing spaces, tabs, and/or newline chars.
 * @param str String to clean
 * @param buf Buffer to store cleaned string
 * @param nbuf Length of storage buffer
 */
void _terminal_clean_string(char* str, size_t slen) {
}


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
        if (i < n-1 && (c == ' ' || isalnum(c)))    // n-1 to guarantee null termination
            buf[i++] = c;

        // if char is a backspace, move index backwards (if possible)
        else if ((c == 0x08 || c == 0x7f) && i > 0)
            i--;

        // if char is a newline or return char, then command is completed
        else if (c == '\n' || c == '\r') {
            buf[i] = 0;     // add null terminator
            break;          // break loop
        }
        // otherwise, do nothing to the char or with the buffer
    }
}


/**
 * @brief Gets command from terminal.
 * @param tcmd Pointer to tcmd_t object.
 *
 * @note Make sure to free the tcmd_t after it has been used!
 */
void terminal_get_command(tcmd_t* tcmd) {
    // stack allocations
    size_t i;
    char *token, *saveptr, *p;

    // get line and store it in temporary buffer
    terminal_get_line(_terminal_cmdbuf, DRKO_TERM_BUFSIZE);

    // calculate argc by using strtok
    tcmd->argc = 0;
    token = strtok_r(_terminal_cmdbuf, " ", &saveptr);
    while (token != NULL) {
        // if token found, then arg has been found
        tcmd->argc++;
        // check for another token
        token = strtok_r(NULL, " ", &saveptr);
    }

    // restore _terminal_cmdbuf 
    p = _terminal_cmdbuf;
    for (i = 0; i < tcmd->argc-1; i++) {
        // move to nullchar
        while(*p != 0) { p++; }
        // replace nullchar with space
        *p = ' ';
    }

    // allocate memory for tcmd->argv
    tcmd->argv = (char**)malloc(sizeof(char*) * tcmd->argc);

    // populate tcmd->argv using strtok
    token = strtok_r(_terminal_cmdbuf, " ", &saveptr);
    for (i = 0; i < tcmd->argc; i++) {
        // allocate memory and add token to argv
        if (token != NULL) {
            tcmd->argv[i] = (char*)calloc(strlen(token), 1);
            strcpy(tcmd->argv[i], token);
            token = strtok_r(NULL, " ", &saveptr);
        }
        // if strtok fails somehow, just make arg and empty string
        else {
            printf("STRTOK RETURNED NULL!!\n");
            tcmd->argv[i] = (char*)calloc(1, 1);
            *tcmd->argv[i] = 0;
        }
    }
}


void tcmd_println(tcmd_t* tcmd) {
    printf("tcmd_t {\n    argc = %d\n    argv = [\n", tcmd->argc);
    for (size_t i = 0; i < tcmd->argc-1; i++)
        printf("        %s,\n", tcmd->argv[i]);
    printf("        %s\n    ]\n}\n", tcmd->argv[tcmd->argc-1]);
}
