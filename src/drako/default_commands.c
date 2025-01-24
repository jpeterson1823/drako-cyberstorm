#include <drako/drako.h>
#include <drako/default_commands.h>
#include <drako/hardware/at28c64b.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

const size_t DRAKO_N_DEFAULT_CMDS = 6;
const char* DRAKO_DEFAULT_CMDS[] = {
    "help",
    "commands",
    "clear",
    "peek",
    "putb",
    "exit"
};
const char* DRAKO_DEFAULT_CMDS_MAN_STRS[] = {
    "Usage: help <command>\nDescription: Displays useful info about a specified command.\n",
    "Usage: commands\nDescription: Lists all valid commands.\n",
    "Usage: clear\nDescription: Clears the screen\n",
    "Usage: peek <addr>\nDescription: Reads a byte from the EEPROM at a specified address.\n",
    "Usage: putb <addr> <byte>\nDescription: Writes a byte to a specified address.\n",
    "Usage: exit\nDescription: Exits the terminal session. Using this will require you to power cycle Drako to reconnect.\n"
};

/**
 * @brief Determines if string command is a default Drako command.
 * @param cmd Command string to check.
 * @return true if cmd is a default Drako command. false otherwise.
 */
bool is_default_cmd(char* cmd) {
    return _drako_get_cmd_id(cmd) != DRAKO_NOT_A_CMD;
}


/**
 * @brief Executes a valid default command.
 * @param cmd ID of command to be executed
 * @return true if provided command is a default command and was executed. false otherwise.
 */
bool default_cmd(uint8_t argc, char** argv) {
    // make sure there's a command to check
    if (argc == 0) {
        printf("EMPTY COMMAND!\n");
        return false;
    }

    // stack allocations in case they are needed
    uint16_t addr;
    uint8_t  byte;

    // execute command
    switch (_drako_get_cmd_id(argv[0])) {
        case DRAKO_DCMD_HELP:
            // if not enough arguments, command cant run
            if (argc < 2) {
                printf("Not enough arguments. Please specify a command: \"help <command>\"\n");
                printf("To see a list of valid commands: \"commands\"\n");
                return false;
            }
            // make sure specified command is a valid drako command
            uint8_t id = _drako_get_cmd_id(argv[1]);
            if (id == DRAKO_NOT_A_CMD) {
                printf("Unrecognized command provided. Please see \"commands\" for more info.\n");
                return false;
            }
            // run command and return it's execution status
            return _drako_cmd_help(id);

        case DRAKO_DCMD_CMDS:
            return _drako_cmd_cmds();

        case DRAKO_DCMD_CLRS:
            return _drako_cmd_clrs();

        case DRAKO_DCMD_PEEK:
            // if not enough arguments, command cant run
            if (argc < 2) {
                printf("Not enough arguments.\nSee \"help peek\" for more details\n");
                return false;
            }
            // make sure hex arg is valid
            if (!_drako_is_arg_hex(argv[1])) {
                printf("Invalid address arguments provided. Please provide a valid hex string.\n");
                return false;
            }
            // convert hex args to respective types
            addr = strtoull(argv[1], NULL, 16);

            return _drako_cmd_peek(addr);

        case DRAKO_DCMD_PUTB:
            // if not enough arguments, command cant run
            if (argc < 3) {
                printf("Not enough arguments.\nSee \"help putb\" for more details\n");
                return false;
            }
            // make sure hex arg is valid
            if (!_drako_is_arg_hex(argv[1]) || !_drako_is_arg_hex(argv[2])) {
                printf("Invalid arguments provided. Please use valid hex strings for the address and byte parameters.\n");
                return false;
            }
            // convert hex args to respective types
            addr = strtoull(argv[1], NULL, 16);
            byte = strtoull(argv[2], NULL, 16);
            return _drako_cmd_putb(addr, byte);

        case DRAKO_DCMD_EXIT:
            return _drako_cmd_exit();

        default:    // DRAKO_NOT_A_CMD
            return false;
    }
}


/**
 * @brief Attempts to find the command ID of a specified command.
 * @param cmd Command to identify.
 * @return DrakoDefaultCmds command ID for valid Drako command. DrakoDefaultCmds::DRAKO_NOT_A_CMD otherwise.
 */
uint8_t _drako_get_cmd_id(const char *cmd) {
    for (uint8_t id = 0; id < DRAKO_N_DEFAULT_CMDS; id++)
        if (strcmp(cmd, DRAKO_DEFAULT_CMDS[id]) == 0)
            return id;
    return DRAKO_NOT_A_CMD;
}


bool _drako_cmd_help(uint8_t cmd_id) {
    printf("%s", DRAKO_DEFAULT_CMDS_MAN_STRS[cmd_id]);
    return true;
}

bool _drako_cmd_cmds() {
    printf("Available Commands:\n");
    for (uint8_t i = 0; i < DRAKO_N_DEFAULT_CMDS; i++)
        printf("    %s\n", DRAKO_DEFAULT_CMDS[i]);
    printf("Please use the \"help\" command for more information about a specific command.\n");
    return true;
}

bool _drako_cmd_clrs() {
    // clear screen: ESC[2J
    printf("%c%s", 0x1b, "[2J");
    // return cursor to home: ESC[H
    printf("%c%s", 0x1b, "[H");

    return true;
}

bool _drako_cmd_peek(uint16_t addr) {
    uint8_t byte;
    at28c64b_select(&drako.prom);
    at28c64b_read8(&drako.prom, addr, &byte);

    //display_select(&drako.disp);
    //display_hex(&drako.disp, byte);
    //display_show(&drako.disp);

    printf("PEEK @ 0x%04x : 0x%02x\n", addr, byte);

    return true;
}

bool _drako_cmd_putb(uint16_t addr, uint8_t byte) {
    //display_select(&drako.disp);
    //display_clear(&drako.disp);
    //display_hide(&drako.disp);

    at28c64b_select(&drako.prom);
    at28c64b_write8(&drako.prom, addr, byte);

    printf("Put byte 0x%02x @ 0x%04x\n", byte, addr);
    return true;
}

bool _drako_cmd_exit() {
    drako.exit_flag = true;
    terminal_println("Exiting...");
    return true;
}

// helper functions

/**
 * @brief Determines if argument is a valid hex representation, with or without leading "0x".
 * @param arg Argument to check.
*/
bool _drako_is_arg_hex(const char *arg) {
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
