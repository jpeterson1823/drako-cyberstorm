#ifndef __drako_default_commands
#define __drako_default_commands

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static const size_t DRAKO_N_DEFAULT_CMDS = 2;
static const char* DRAKO_DEFAULT_CMDS[] = {
    "help",
    "commands"
};

enum DrakoDefaultCmds {
    DRAKO_DCMD_HELP = 0,
    DRAKO_DCMD_CMDS = 1
};

bool is_default_cmd(const char* cmd);
bool default_cmd(enum DrakoDefaultCmds cmd_id);

void _drako_cmd_help();
void _drako_cmd_commands();
void _drako_cmd_peek(uint16_t addr);

#endif
