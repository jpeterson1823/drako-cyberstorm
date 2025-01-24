#ifndef __drako_default_commands
#define __drako_default_commands

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

extern const size_t DRAKO_N_DEFAULT_CMDS;
extern const char* DRAKO_DEFAULT_CMDS[];
extern const char* DRAKO_DEFAULT_CMDS_MAN_STRS[];

enum DrakoDefaultCmds {
    DRAKO_DCMD_HELP = 0,
    DRAKO_DCMD_CMDS = 1,
    DRAKO_DCMD_CLRS = 2,
    DRAKO_DCMD_PEEK = 3,
    DRAKO_DCMD_PUTB = 4,
    DRAKO_DCMD_EXIT = 5,
    DRAKO_NOT_A_CMD = 255
};

bool is_default_cmd( char* argv);
bool default_cmd( uint8_t argc, char** argv);

uint8_t _drako_get_cmd_id(const char* cmd);
bool _drako_cmd_help(uint8_t cmd_id);
bool _drako_cmd_cmds();
bool _drako_cmd_clrs();
bool _drako_cmd_peek(uint16_t addr);
bool _drako_cmd_putb(uint16_t addr, uint8_t byte);
bool _drako_cmd_exit();

// helper functions
bool _drako_is_arg_hex(const char* arg);

#endif
