#include <drako/drako.h>
#include <drako/commands.h>
#include <drako/hardware/at28c64b.h>
#include <level0/level.h>
#include <hidden/hidden.h>
#include <hidden_challenge.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>



const size_t DRAKO_N_CMDS = 11;
const char* DRAKO_CMDS[] = {
    "help",
    "commands",
    "clear",
    "peek",
    "put",
    "show",
    "hide",
    "exit",
    "steg",
    "hexdump",
    "enter"
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
    "Usage: enter\nDescription: Begin your adventure.\n"
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

    DrakoStegConfig cfg;
    uint8_t bytebuf[DRAKO_BUFSIZE];
    uint16_t nbytes;

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
            return drako_cmd_hide();
        case DRAKO_CMD_EXIT:
            return drako_cmd_exit();
        case DRAKO_CMD_STEG:
            nbytes = _drako_parse_steg_cmd(cmdstr, &cfg, bytebuf, DRAKO_BUFSIZE);
            return drako_cmd_steg(cfg, bytebuf, nbytes);
        case DRAKO_CMD_HEXDUMP:
            return drako_cmd_hexdump();
        case DRAKO_CMD_ENTER:
            level0();
            return true;
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
    else if (hc_c2_complete && strcmp(token, "whatis") == 0) {
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
    if (hc_c2_complete)
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
    if (addr >= HC_C2_MEMSPACE_OFFSET && addr <= HC_C2_MEMSPACE_TAIL  && !hc_c2_complete)
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


/**
 * @brief Parses string command into DrakoStegConfig struct
 * @param cmd String command to be parsed
 * @param config Pointer to DrakoStegConfig struct in which to save config options
 * @param bytes Byte buffer to either pass in bytes to be steg'd or pass out retrieved bytes
 * @param nbytes Either the length of bytes being passed in or the number of bytes to be retrieved.
 */
uint16_t _drako_parse_steg_cmd(const char* cmd, DrakoStegConfig* config, uint8_t* bytebuf, uint16_t nbytes) {
    // set default config
    config->operation = DRAKO_STEG_NO_OPERATION;
    config->mode = DRAKO_STEG_MODE_BYTE;
    config->offset = 0x0000;
    config->interval = 1;
    config->nbytes = 1;

    // copy cmd for tokenization
    char cpy[DRAKO_BUFSIZE];
    strncpy(cpy, cmd, DRAKO_BUFSIZE);
    cpy[DRAKO_BUFSIZE-1] = 0;
    
    // verify command is steg
    char *token, *saveptr;
    token = strtok_r(cpy, " ", &saveptr);
    if (token != NULL && strcmp(token, "steg") != 0)
        return false;

    // get flag token
    token = strtok_r(NULL, " ", &saveptr);
    // if no flag token, cannot continue
    if (token == NULL) {
        printf("[ERROR] Required flags were not set! See \"help steg\" for more info.\n");
        return false;
    }

    // create flag stack to keep track of flags that need an argument
    char flags[DRAKO_BUFSIZE];
    uint8_t nflags = 0;

    // parse flag string
    uint8_t i;
    if (token[0] == '-') {
        for (i = 1; i < 0xff && i < strlen(token); i++) {
            // set config
            switch (token[i]) {
                case DRAKO_STEG_MODE_BIT: // no arg required
                    config->mode = DRAKO_STEG_MODE_BIT;
                    break;
                case DRAKO_STEG_MODE_BYTE:// no arg required
                    config->mode = DRAKO_STEG_MODE_BYTE;
                    break;
                case DRAKO_STEG_RETRV:    // arg required!
                    config->operation = DRAKO_STEG_RETRV;
                    flags[nflags++] = token[i];
                    break;
                case DRAKO_STEG_STORE:    // no arg required
                    config->operation = DRAKO_STEG_STORE;
                    break;
                case DRAKO_STEG_OFFSET:   // arg required!
                    flags[nflags++] = token[i];
                    break;
                case DRAKO_STEG_INTERVAL: // arg required!
                    flags[nflags++] = token[i];
                    break;
                default:
                    printf("[ERROR] Unknown steg flag '%c'\n", token[i]);
                    return false;
            }
        }
    }
    // otherwise, param is an arg without any provided flag
    else {
        printf("[ERROR] Provided orphan argument '%s'. Which parameter(s) are you trying to set?\n", token);
        return false;
    }
    
    // parse arguments required by specified flags
    for (i = 0; i < nflags; i++) {
        // get next token
        token = strtok_r(NULL, " ", &saveptr);
        // if no further token, then missing args
        if (token == NULL) {
            printf("[ERROR] Missing argument for flag '-%c'\n", flags[i]);
            return false;
        }

        // store args in config
        switch (flags[i]) {
            case DRAKO_STEG_RETRV:
                if (_is_hex_str(token))
                    config->nbytes = strtoul(token, NULL, 16);
                else {
                    printf("[ERROR] All number arguments must be valid hexidecimal!\n");
                    return false;
                }
            case DRAKO_STEG_OFFSET:
                config->offset = strtoul(token, NULL, 16);
                break;
            case DRAKO_STEG_INTERVAL:
                config->interval = strtoull(token, NULL, 16);
                // make sure interval is not zero. if it is, cannot continue
                if (config->interval <= 0) {
                    printf("[ERROR] Interval must be greater than zero!\n");
                    return false;
                }
                break;
            default:    // this should never trigger due to previous flag parsing
                printf("[CRITICAL ERROR] Unknown flag passed through flag parsing!!\nOffending Flag = '-%c'", flags[i]);
                return false;
        }
    }

    // if in store mode, all remaining args are bytes to be stored
    token = strtok_r(NULL, " ", &saveptr);
    uint16_t j = 0;
    while (token != NULL) {
        if (j < nbytes)
            bytebuf[j++] = strtoul(token, NULL, 16);
        else break;
        token = strtok_r(NULL, " ", &saveptr);
    }

    // make sure operation is set before returning true
    if (config->operation == DRAKO_STEG_NO_OPERATION) {
        printf("[ERROR] No operation provided!\n");
        return false;
    }

    // otherwise, config has been parsed successfully
    // if in store mode, return number of bytes to be stored
    if (config->operation == DRAKO_STEG_STORE)
        return j;
    // otherwise, return nbytes
    else
        return nbytes;
}

///**
// * @brief Drako steganography command
// * @param config Preconfigured DrakoStegConfig struct
// * @param bytes Byte buffer to either pass in bytes to be steg'd or pass out retrieved bytes
// * @param nbytes Either the length of bytes being passed in or the number of bytes to be retrieved.
// */
//bool drako_cmd_steg(const DrakoStegConfig config, uint8_t* bytes, uint16_t nbytes) {
//    // set offset address and select eeprom
//    uint16_t addr = config.offset;
//    at28c64b_select(&drako.prom);
//
//    // store mode
//    if (config.operation == DRAKO_STEG_STORE) {
//        // steg each bit or byte
//        if (config.mode == DRAKO_STEG_MODE_BIT) {
//            uint8_t byte;   // used in storage mask
//            for (uint16_t i = 0; i < nbytes; i++) {
//                at28c64b_read8(&drako.prom, addr, &byte);
//                at28c64b_write8(&drako.prom, addr, bytes[i]&0x01 ? byte | 0x01 : byte & 0xfe);
//                addr += config.interval;    // inc address by interval
//            }
//        } else {
//            for (uint16_t i = 0; i < nbytes; i++) {
//                at28c64b_write8(&drako.prom, addr, bytes[i]);
//                addr += config.interval;    // inc address by interval
//            }
//        }
//        return true;
//    }
//
//    // retrieve mode
//    else if (config.operation == DRAKO_STEG_RETRV) {
//        uint8_t byte;
//        uint16_t i;
//        if (config.mode == DRAKO_STEG_MODE_BIT) {
//            for (i = 0; i < config.nbytes; i++) {
//                at28c64b_read8(&drako.prom, addr, &byte);
//                bytes[i] = 0x01 & byte;
//                addr += config.interval;
//            }
//        } else {
//            for (i = 0; i < nbytes; i++) {
//                at28c64b_read8(&drako.prom, addr, &byte);
//                bytes[i] = byte;
//                addr += config.interval;
//            }
//        }
//        // display retrieved bytes to terminal
//        for(i = 0; i < config.nbytes; i++)
//            printf("%02x ", bytes[i]);
//        printf("\n");
//        return true;
//    }
//
//    // no operation set: error out
//    else if (config.operation == DRAKO_STEG_NO_OPERATION) {
//        printf("[ERROR] Steg operation mode was not specified!\n");
//        return false;
//    }
//    
//    // pray this does not occur; for if it does, the code gods have certainly forsaken thee.
//    else {
//        printf("[CRITICAL ERROR] Unknown steg operation provided in config!\n");
//        return false;
//    }
//}


/**
 * @brief Drako steganography command
 * @param config Preconfigured DrakoStegConfig struct
 * @param bytes Byte buffer to either pass in bytes to be steg'd or pass out retrieved bytes
 * @param nbytes Either the length of bytes being passed in or the number of bytes to be retrieved.
 */
bool drako_cmd_steg(const DrakoStegConfig config, uint8_t* bytes, uint16_t nbytes) {
    // set offset address and select eeprom
    uint16_t addr = config.offset;
    at28c64b_select(&drako.prom);

    // store mode
    if (config.operation == DRAKO_STEG_STORE) {
        printf("%s STEG store mode has been disabled!\n", DRKO_TERM);
        return false;
    }

    // retrieve mode. stegged bits are stored MSB first
    else if (config.operation == DRAKO_STEG_RETRV) {
        uint8_t byte, temp;
        uint16_t i;
        if (config.mode == DRAKO_STEG_MODE_BIT) {
            for (i = 0; i < config.nbytes; i++) {
                temp = 0;
                for (uint8_t bit = 0; bit < 8; bit++) {
                    // read byte
                    at28c64b_read8(&drako.prom, addr, &byte);
                    // get stegged bit (MSB first) from byte and put it in temp
                    temp |= (byte & 0x01)<<(7-bit);
                    // inc address by interval
                    addr += config.interval;
                }
                // store byte in bytes array
                bytes[i] = temp;
            }
        } else {
            for (i = 0; i < nbytes; i++) {
                at28c64b_read8(&drako.prom, addr, &byte);
                bytes[i] = byte;
                addr += config.interval;
            }
        }
        // display retrieved bytes to terminal
        for(i = 0; i < config.nbytes; i++)
            printf("%02x ", bytes[i]);
        printf("\n");
        return true;
    }

    // no operation set: error out
    else if (config.operation == DRAKO_STEG_NO_OPERATION) {
        printf("[ERROR] Steg operation mode was not specified!\n");
        return false;
    }
    
    // pray this does not occur; for if it does, the code gods have certainly forsaken thee.
    else {
        printf("[CRITICAL ERROR] Unknown steg operation provided in config!\n");
        return false;
    }
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

        if (addr >= HC_C2_MEMSPACE_OFFSET && addr <= HC_C2_MEMSPACE_TAIL && !hc_c2_complete)
            printf("-- ");
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
