#ifndef __drako_default_commands
#define __drako_default_commands

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define DRAKO_STEG_HELP_STR "Usage: steg <offset> <interval> <nbytes>\n"

extern const size_t DRAKO_N_DEFAULT_CMDS;
extern const char* DRAKO_DEFAULT_CMDS[];
extern const char* DRAKO_DEFAULT_CMDS_MAN_STRS[];

// Enum of all command IDs
enum DrakoCmd {
    DRAKO_CMD_HELP = 0,
    DRAKO_CMD_CMDS = 1,
    DRAKO_CMD_CLRS = 2,
    DRAKO_CMD_PEEK = 3,
    DRAKO_CMD_PUT  = 4,
    DRAKO_CMD_SHOW = 5,
    DRAKO_CMD_HIDE = 6,
    DRAKO_CMD_EXIT = 7,
    DRAKO_CMD_RSTEG = 8,
    DRAKO_CMD_HEXDUMP = 9,
    DRAKO_CMD_ENTER = 10,
    DRAKO_NOT_A_CMD = 255
};

// main command functions
uint8_t drako_cmd_get_id(const char* cmd);
bool exec_drako_cmd_str(const char* cmdstr);
bool is_drako_cmd(const char* cmdstr);

// individual command functions
bool drako_cmd_help(char* token, char** saveptr);
bool drako_cmd_cmds();
bool drako_cmd_clrs();
bool drako_cmd_peek(char* token, char** saveptr);
bool drako_cmd_put(char* token, char** saveptr);
bool drako_cmd_show(char* token, char** saveptr);
bool drako_cmd_hide();
bool drako_cmd_exit();
bool drako_cmd_rsteg(char* token, char** saveptr);
bool drako_cmd_encrypt(uint8_t* data, size_t nbytes);
bool drako_cmd_decrypt(uint8_t* data, size_t nbytes);
bool drako_cmd_hexdump();

// helper functions
bool _is_hex_str(const char* arg);
bool _drako_parse_rsteg_cmd(char* token, char** saveptr, uint16_t* offset, uint8_t* interval, uint16_t* nbytes);
bool _drako_str_to_uint8_t(char* str, uint8_t* buff);
bool _drako_str_to_uint16_t(char* str, uint16_t* buff);

#endif
