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
    DRAKO_DCMD_PUT  = 4,
    DRAKO_DCMD_SHOW = 5,
    DRAKO_DCMD_HIDE = 6,
    DRAKO_DCMD_EXIT = 7,
    DRAKO_NOT_A_CMD = 255
};

bool is_default_cmd(const char* cmdstr);
//bool execute_default_command(const char* cmdstr);
bool exec_default_cmd_str(const char* cmdstr);

uint8_t _default_cmd_get_id(const char* cmd);
bool _default_cmd_help(char* token, char** saveptr);
bool _default_cmd_cmds();
bool _default_cmd_clrs();
bool _default_cmd_peek(char* token, char** saveptr);
bool _default_cmd_put(char* token, char** saveptr);
bool _default_cmd_show(char* token, char** saveptr);
bool _default_cmd_hide();
bool _default_cmd_exit();

// helper functions
bool _is_hex_str(const char* arg);

#endif
