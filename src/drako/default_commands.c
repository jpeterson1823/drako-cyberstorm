#include <drako/drako.h>
#include <drako/default_commands.h>
#include <drako/hardware/at28c64b.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const size_t DRAKO_N_DEFAULT_CMDS = 8;
const char* DRAKO_DEFAULT_CMDS[] = {
    "help",
    "commands",
    "clear",
    "peek",
    "put",
    "show",
    "hide",
    "exit"
};
const char* DRAKO_DEFAULT_CMDS_MAN_STRS[] = {
    "Usage: help <command>\nDescription: Displays useful info about a specified command.\n",
    "Usage: commands\nDescription: Lists all valid commands.\n",
    "Usage: clear\nDescription: Clears the screen\n",
    "Usage: peek <addr>\nDescription: Reads a byte from the EEPROM at a specified address.\nAddress must be valid hexidecimal (e.g. 23 or 0x23)\n",
    "Usage: put <addr> <byte>\nDescription: Writes a byte to a specified address.\nAddress and byte must be valid hexidecimal (e.g. 23 or 0x23)\n",
    "Usage: show <byte>\nDescription: Displays a byte on the dual 7-segment display.\nByte must be valid hexidecimal (e.g. 23 or 0x23)\n",
    "Usage: hide\nDescription: Turns off the dual 7-segment display.\n",
    "Usage: exit\nDescription: Exits the terminal session. Using this will require you to power cycle Drako to reconnect.\n"
};


/**
 * @brief Executes a default command
 * @param cmdstr Entire command string, including its arguments.
 * @return true if a default command is executed. false otherwise.
 */
bool exec_default_cmd_str(const char* cmdstr) {
    // stack allocations
    char cmd[DRAKO_BUFSIZE];
    char *token, *saveptr;
    uint8_t id;

    // copy cmdstr
    strcpy(cmd, cmdstr);
    cmd[DRAKO_BUFSIZE-1] = 0;   // insure null terminator

    // get cmd token and cmd id
    token = strtok_r(cmd, " ", &saveptr);
    id = _default_cmd_get_id(token);

    // execute command
    switch(id) {
        case DRAKO_DCMD_HELP:
            return _default_cmd_help(token, &saveptr);
        case DRAKO_DCMD_CMDS:
            return _default_cmd_cmds();
        case DRAKO_DCMD_CLRS:
            return _default_cmd_clrs();
        case DRAKO_DCMD_PEEK:
            return _default_cmd_peek(token, &saveptr);
        case DRAKO_DCMD_PUT:
            return _default_cmd_put(token, &saveptr);
        case DRAKO_DCMD_SHOW:
            return _default_cmd_show(token, &saveptr);
        case DRAKO_DCMD_HIDE:
            return _default_cmd_hide();
        case DRAKO_DCMD_EXIT:
            return _default_cmd_exit();
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
bool is_default_cmd(const char* cmdstr) {
    // stack allocations
    char cmd[DRAKO_BUFSIZE];

    // copy first arg
    size_t i;
    for (i = 0; i < strlen(cmdstr) && cmdstr[i] != 0; i++) {
        if (cmdstr[i] != ' ')
            cmd[i] = cmdstr[i];
        else break;
    }
    cmd[i] = 0;

    return _default_cmd_get_id(cmd) != DRAKO_NOT_A_CMD;
}


/**
 * @brief Attempts to find the command ID of a specified command.
 * @param cmd Command to identify.
 * @return DrakoDefaultCmds command ID for valid Drako command. DrakoDefaultCmds::DRAKO_NOT_A_CMD otherwise.
 */
uint8_t _default_cmd_get_id(const char *cmd) {
    for (uint8_t id = 0; id < DRAKO_N_DEFAULT_CMDS; id++) {
        //printf("%s == %s? : ", cmd, DRAKO_DEFAULT_CMDS[id]);
        if (strcmp(cmd, DRAKO_DEFAULT_CMDS[id]) == 0) {
            //printf("YES\n");
            return id;
        }
        //else
        //    printf("NO\n");
    }
    return DRAKO_NOT_A_CMD;
}


/**
 * @brief Help command function. Should not be used unless you know what you're doing.
 * @param token Previously used, via strtok_r, token pointer.
 * @param saveptr Previously used, via strtok_r, save pointer.
 */
bool _default_cmd_help(char* token, char** saveptr) {
    // get second argument and pass it to help function
    token = strtok_r(NULL, " ", saveptr);

    // if second argument is null, print help's help string
    if (token == NULL) {
        printf("%s", DRAKO_DEFAULT_CMDS_MAN_STRS[DRAKO_DCMD_HELP]);
        return false;
    }

    // make sure it's a default command
    uint8_t cmd_id = _default_cmd_get_id(token);
    if (cmd_id == DRAKO_NOT_A_CMD){
        printf("Unknown command provided as argument: \"%s\".\nSee \"commands\" for a list of valid commands.\n", token);
        return false;
    }
    printf("%s", DRAKO_DEFAULT_CMDS_MAN_STRS[cmd_id]);
    return true;
}


/**
 * @brief Commands command function. Should not be used unless you know what your're doing.
 */
bool _default_cmd_cmds() {
    printf("Available Commands:\n");
    for (uint8_t i = 0; i < DRAKO_N_DEFAULT_CMDS; i++)
        printf("    %s\n", DRAKO_DEFAULT_CMDS[i]);
    printf("Please use the \"help\" command for more information about a specific command.\n");
    return true;
}


/**
 * @brief Clear Screen command function. Should not be used unless you know what your're doing.
 */
bool _default_cmd_clrs() {
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
bool _default_cmd_peek(char* token, char** saveptr) {
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
bool _default_cmd_put(char* token, char** saveptr) {
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
bool _default_cmd_show(char* token, char** saveptr) {
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
bool _default_cmd_hide() {
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
bool _default_cmd_exit() {
    drako.exit_flag = true;
    terminal_println("Exiting...");
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
