#include <drako/drako.h>
#include <drako/commands.h>
#include <drako/hardware/at28c64b.h>
#include <hidden_challenge.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const size_t DRAKO_N_CMDS = 10;
const char* DRAKO_CMDS[] = {
    "help",
    "commands",
    "clear",
    "peek",
    "put",
    "show",
    "hide",
    "exit",
    "rsteg",
    "hexdump",
};
const char* DRAKO_CMD_HELP_STRS[] = {
    "Usage: help <command>\nDescription: Displays useful info about a specified command.\n",
    "Usage: commands\nDescription: Lists all valid commands.\n",
    "Usage: clear\nDescription: Clears the screen\n",
    "Usage: peek <addr>\nDescription: Reads a byte from the EEPROM at a specified address.\nAddress must be valid hexidecimal (e.g. 23 or 0x23)\n",
    "Usage: put <addr> <byte>\nDescription: Writes a byte to a specified address.\nAddress and byte must be valid hexidecimal (e.g. 23 or 0x23)\n",
    "Usage: show <byte>\nDescription: Displays a byte on the dual 7-segment display.\nByte must be valid hexidecimal (e.g. 23 or 0x23)\n",
    "Usage: hide\nDescription: Turns off the dual 7-segment display.\n",
    "Usage: exit\nDescription: Exits the terminal session. Using this will require you to power cycle Drako to reconnect.\n",
    DRAKO_STEG_HELP_STR,
    "Usage: hexdump\nDescription:",
};


/**
 * @brief Executes a default command
 * @param cmdstr Entire command string, including its arguments.
 * @return true if a default command is executed. false otherwise.
 */
bool exec_drako_cmd_str(const char* cmdstr) {
    // stack allocations
    char cmd[DRAKO_BUFSIZE];
    char *token, *saveptr;
    uint8_t id;

    // copy cmdstr
    strcpy(cmd, cmdstr);
    cmd[DRAKO_BUFSIZE-1] = 0;   // insure null terminator

    // get cmd token and cmd id
    token = strtok_r(cmd, " ", &saveptr);
    id = drako_cmd_get_id(token);

    // execute command
    switch(id) {
        case DRAKO_CMD_HELP:
            return drako_cmd_help(token, &saveptr);
        case DRAKO_CMD_CMDS:
            return drako_cmd_cmds();
        case DRAKO_CMD_CLRS:
            return drako_cmd_clrs();
        case DRAKO_CMD_PEEK:
            return drako_cmd_peek(token, &saveptr);
        case DRAKO_CMD_PUT:
            return drako_cmd_put(token, &saveptr);
        case DRAKO_CMD_SHOW:
            return drako_cmd_show(token, &saveptr);
        case DRAKO_CMD_HIDE:
            printf("\nHIDE CALLED!\n");
            return drako_cmd_hide();
        case DRAKO_CMD_EXIT:
            return drako_cmd_exit();
        case DRAKO_CMD_RSTEG:
            return drako_cmd_rsteg(token, &saveptr);
        case DRAKO_CMD_HEXDUMP:
            return drako_cmd_hexdump();
        default:
            // unrecognized command
            return false;
    }
}


/**
 * @brief Determines if string command is a default Drako command.
 * @param cmd Command string to check.
 * @return true if cmd is a default Drako command. false otherwise.
 */
bool is_drako_cmd(const char* cmd) {
    // make copy of cmd
    char cpy[DRAKO_BUFSIZE];
    strncpy(cpy, cmd, DRAKO_BUFSIZE);
    cpy[DRAKO_BUFSIZE-1] = 0;

    // get first token
    char *token, *saveptr;
    token = strtok_r(cpy, " ", &saveptr);

    // if no token, then is not a command
    if (token == NULL)
        return false;

    // otherwise, check if token is command
    return drako_cmd_get_id(token) != DRAKO_NOT_A_CMD;
}


/**
 * @brief Attempts to find the command ID of a specified command.
 * @param cmd Command to identify.
 * @return DrakoDefaultCmds command ID for valid Drako command. DrakoDefaultCmds::DRAKO_NOT_A_CMD otherwise.
 */
uint8_t drako_cmd_get_id(const char *cmd) {
    for (uint8_t id = 0; id < DRAKO_N_CMDS; id++) {
        if (strcmp(cmd, DRAKO_CMDS[id]) == 0) {
            return id;
        }
    }
    return DRAKO_NOT_A_CMD;
}


/**
 * @brief Help command function. Should not be used unless you know what you're doing.
 * @param token Previously used, via strtok_r, token pointer.
 * @param saveptr Previously used, via strtok_r, save pointer.
 */
bool drako_cmd_help(char* token, char** saveptr) {
    // get second argument and pass it to help function
    token = strtok_r(NULL, " ", saveptr);

    // if second argument is null, print help's help string
    if (token == NULL) {
        printf("%s", DRAKO_CMD_HELP_STRS[DRAKO_CMD_HELP]);
        return false;
    }

    // if token is "whatis" and hc_c3 is active, display it's help message
    else if (drako.hasMagicCreds && strcmp(token, "whatis") == 0) {
        printf(
            "Usage: whatis <guess>\n"
            "Description: Use this to guess the answer to Drako's riddle.\n"
        );
    }

    // make sure it's a default command
    uint8_t cmd_id = drako_cmd_get_id(token);
    if (cmd_id == DRAKO_NOT_A_CMD){
        printf("Unknown command provided as argument: \"%s\".\nSee \"commands\" for a list of valid commands.\n", token);
        return false;
    }

    printf("%s", DRAKO_CMD_HELP_STRS[cmd_id]);
    return true;
}


/**
 * @brief Commands command function. Should not be used unless you know what your're doing.
 */
bool drako_cmd_cmds() {
    printf("Available Commands:\n");
    for (uint8_t i = 0; i < DRAKO_N_CMDS; i++)
        printf("    %s\n", DRAKO_CMDS[i]);
    // display "whatis" command when hc_c3 is active
    if (drako.hasMagicCreds)
        printf("    whatis (unlocked via hidden challenge)\n");
    printf("Please use the \"help\" command for more information about a specific command.\n");
    return true;
}


/**
 * @brief Clear Screen command function. Should not be used unless you know what your're doing.
 */
bool drako_cmd_clrs() {
    // clear screen: ESC[2J
    printf("%c%s", 0x1b, "[2J");
    // return cursor to home: ESC[H
    printf("%c%s", 0x1b, "[H");

    return true;
}


/**
 * @brief Peek command function. Should not be used unless you know what you're doing.
 * @param token Previously used, via strtok_r, token pointer.
 * @param saveptr Previously used, via strtok_r, save pointer.
 */
bool drako_cmd_peek(char* token, char** saveptr) {
    // get address argument and make sure it's valid hex
    uint16_t addr;
    token = strtok_r(NULL, " ", saveptr);
    if (!_is_hex_str(token)) {
        printf("Invalid address argument \"%s\".\nSee \"help peek\" for more info.\n", token);
        return false;
    } else addr = (uint16_t)strtoul(token, NULL, 16);

    // read byte
    uint8_t byte;
    at28c64b_select(&drako.prom);
    at28c64b_read8(&drako.prom, addr, &byte);

    // display read byte
    if (addr >= HC_C2_MEMSPACE_OFFSET && addr <= HC_C2_MEMSPACE_TAIL  && !drako.hasMagicCreds)
        printf("PEEK <--- Overridden: REDACTED --->\n");
    else
        printf("PEEK @ 0x%04x : 0x%02x\n", addr, byte);

    // restore display as it may have been corrupted by eeprom read
    display_select(&drako.disp);
    display_write(&drako.disp, drako.disp.data);

    return true;
}


/**
 * @brief Put Byte command function. Should not be used unless you know what you're doing.
 * @param token Previously used, via strtok_r, token pointer.
 * @param saveptr Previously used, via strtok_r, save pointer.
 */
bool drako_cmd_put(char* token, char** saveptr) {
    // get address argument and make sure it's valid hex
    uint16_t addr;
    token = strtok_r(NULL, " ", saveptr);
    if (!_is_hex_str(token)) {
        printf("Invalid address argument \"%s\".\nSee \"help put\" for more info.\n", token);
        return false;
    } else addr = (uint16_t)strtoul(token, NULL, 16);

    // get byte argument and make sure it's valid hex
    uint8_t byte;
    token = strtok_r(NULL, " ", saveptr);
    if (!_is_hex_str(token)) {
        printf("Invalid byte argument \"%s\".\nSee \"help put\" for more info.\n", token);
        return false;
    } else byte = (uint8_t)strtoul(token, NULL, 16);

    // write byte
    at28c64b_select(&drako.prom);
    at28c64b_write8(&drako.prom, addr, byte);

    // restore display as it may have been corrupted by write
    display_select(&drako.disp);
    display_write(&drako.disp, drako.disp.data);

    printf("Put byte 0x%02x @ 0x%04x\n", byte, addr);
    return true;
}


/**
 * @brief Show command function. Should not be used unless you know what you're doing.
 * @param token Previously used, via strtok_r, token pointer.
 * @param saveptr Previously used, via strtok_r, save pointer.
 */
bool drako_cmd_show(char* token, char** saveptr) {
    // get next argument
    token = strtok_r(NULL, " ", saveptr);

    // if no next argument, just turn display on
    if (token == NULL) {
        display_select(&drako.disp);
        display_write(&drako.disp, drako.disp.data);
        display_show(&drako.disp);
        return true;
    }

    // if argument is hex, get byte
    uint8_t byte;
    if (!_is_hex_str(token)) {
        printf("Invalid byte argument \"%s\".\nSee \"help show\" for more info.\n", token);
        return false;
    } else byte = (uint8_t)strtoul(token, NULL, 16);

    // write byte to display and show display
    display_select(&drako.disp);
    display_hex(&drako.disp, byte);
    display_show(&drako.disp);
    return true;
}


/**
 * @brief Hide command function. Should not be used unless you know what you're doing.
 * @param token Previously used, via strtok_r, token pointer.
 * @param saveptr Previously used, via strtok_r, save pointer.
 */
bool drako_cmd_hide() {
    // select and hide display
    display_select(&drako.disp);
    display_hide(&drako.disp);
    return true;
}


/**
 * @brief Exit command function. Should not be used unless you know what you're doing.
 * @param token Previously used, via strtok_r, token pointer.
 * @param saveptr Previously used, via strtok_r, save pointer.
 */
bool drako_cmd_exit() {
    drako.exit_flag = true;
    terminal_println("Exiting...");
    return true;
}

/// Drako Retrieval Steg
/// Stegged data is stored bitwise, MSB first.
bool drako_cmd_rsteg(char* token, char** saveptr) {
    // parse steg params from command
    uint16_t offset, nbytes;
    uint8_t interval;
    bool valid = _drako_parse_rsteg_cmd(token, saveptr, &offset, &interval, &nbytes);

    printf("RSTEG\n    offset  : %#06x\n    interval: %#04x\n    nbytes  : %#04x\n", offset, interval, nbytes);

    // if provided command is invalid, display message to console and return false
    if (!valid) {
        printf("Please see the `help` command for usage instructions.\n");
        return false;
    }

    // display start of steg output
    printf("Retrieved Bytes (hex): \n");

    // extract stegged data
    uint8_t eeprom_byte, byte;
    for (uint16_t addr = offset; addr < DRAKO_EEPROM_SIZE && addr < offset + (interval*nbytes*8);) {
        byte = 0;
        // construct byte from bitwise stegged data
        for (uint8_t bit = 0; bit < 8; bit++) {
            //printf("addr=%#06x\n", addr);
            // read byte from eeprom
            at28c64b_read8(&drako.prom, addr, &eeprom_byte);
            // extract byte and shift to proper position
            byte |= (eeprom_byte&0x01)<<(7-bit);
            // increment byte index by interval
            addr += interval;
        }
        // print byte to console as hex
        printf("%#04x ", byte);
    }
    // add trailing newline
    printf("\n");
    // steg completed successfully
    return true;
}

bool _drako_parse_rsteg_cmd(char *token, char **saveptr, uint16_t *offset, uint8_t *interval, uint16_t *nbytes) {
    // get next token (which is the first arg)
    token = strtok_r(NULL, " ", saveptr);
    // first arg is offset
    if (token == NULL) {
        printf("Missing offset argument. See help for more info.\n");
        return false;
    } else if (!_drako_str_to_uint16_t(token, offset)) {
        printf("Invalid offset parameter `%s` provided.\n", token);
        return false;
    }

    // second arg is interval
    token = strtok_r(NULL, " ", saveptr);
    if (token == NULL) {
        printf("Missing interval offset. See help for more info.\n");
        return false;
    } else if (!_drako_str_to_uint8_t(token, interval)) {
        printf("Invalid interval parameter `%s` provided.\n", token);
        return false;
    }

    // third arg is nbytes
    token = strtok_r(NULL, " ", saveptr);
    if (token == NULL) {
        printf("Missing nbytes argument. See help for more info.\n");
        return false;
    } else if (!_drako_str_to_uint16_t(token, nbytes)) {
        printf("Invalid nbytes parameter `%s` provided.\n", token);
        return false;
    }

    // args successfully parsed: return true
    return true;
}

bool _drako_str_to_uint8_t(char* str, uint8_t* buff) {
    char* end;
    // intentionally allow overflow (part of challenge help)
    *buff = (uint8_t)strtol(str, &end, 16);
    return true;
}

bool _drako_str_to_uint16_t(char* str, uint16_t* buff) {
    char* end;
    // intentionally allow overflow (part of challenge help)
    *buff = (uint16_t)strtol(str, &end, 16);
    return true;
}

/**
 * @brief Drako hexdump command. Displays a hexdump of Drako's EEPROM.
 */
bool drako_cmd_hexdump() {
    uint8_t byte;
    printf("0000  |  ");
    for (uint16_t addr = 0; addr <= AT28C64B_MAX_ADDR; addr++) {
        if (addr != 0x0000 && addr % 16 == 0)
            printf("\n%04x  |  ", addr);
        at28c64b_read8(&drako.prom, addr, &byte);

        if (addr >= HC_C2_MEMSPACE_OFFSET && addr <= HC_C2_MEMSPACE_TAIL && !drako.hasMagicCreds)
            printf("== ");
        else
            printf("%02x ", byte);
    }
    printf("\n");
    return true;
}


// helper functions

/**
 * @brief Determines if argument is a valid hex representation, with or without leading "0x".
 * @param arg Argument to check.
*/
bool _is_hex_str(const char *arg) {
    // calc length
    size_t len = strlen(arg);

    // if more than two chars and "0x" is located
    // somewhere other than the beginning, then invalid
    bool prefix = false;
    if (len > 2 && strstr(arg, "0x") != arg)
        return false;
    else
        prefix = true;

    // if there's a prefix, skip first two chars
    size_t i;
    if (prefix)
        i = 2;
    else 
        i = 0;

    // determine if remaining chars are valid hex digits
    for (; i < len; i++) {
        // if invalid digit, return false
        if ((arg[i] < '0' || arg[i] > '9') && (tolower(arg[i]) < 'a' || tolower(arg[i]) > 'f'))
            return false;
    }
    return true;
}
