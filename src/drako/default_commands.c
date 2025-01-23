#include <drako/default_commands.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Determines if string command is a default drako command. If so, it also executes that command.
 * @param cmd Command string to check
 * @return true if cmd is a default drako command. false otherwise.
 */
bool is_default_cmd(const char* cmd) {
    // loop through all default commands
    for (size_t i = 0; i < DRAKO_N_DEFAULT_CMDS; i++) {
        if (strcmp(cmd, DRAKO_DEFAULT_CMDS[i]) == 0) {
            default_cmd(i);
            return true;
        }
    }
    // if code reaches this point, then cmd is not a default command
    return false;
}


/**
 * @brief Executes a default command.
 * @param cmd ID of command to be executed
 * @return true if provided command is a default command and was executed. false otherwise.
 */
bool default_cmd(enum DrakoDefaultCmds cmd_id) {
    switch (cmd_id) {
        case DRAKO_DCMD_HELP:
            _drako_cmd_help();
            return true;
        case DRAKO_DCMD_CMDS:
            _drako_cmd_commands();
            return true;
        default:
            return false;
    }
}

void _drako_cmd_help() {
    printf("TODO: Help String\n");
}

void _drako_cmd_commands() {
    printf("TODO: Commands String\n");
}
