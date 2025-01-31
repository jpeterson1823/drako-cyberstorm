#ifndef __drako_default_commands
#define __drako_default_commands

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define DRAKO_STEG_HELP_STR "Usage: steg <flag_string> [arguments]\n"\
                            "Description: Steganography command. Can store or retrieve data in various modes.\n"\
                            "More Info:\n"\
                            "  Flag String Required Format:\n"\
                            "     - flag string must be first argument\n"\
                            "     - argument order must match flag order\n"\
                            "     - flag string must start with a '-' and be followed by each desired flag without spaces\n"\
                            "  Example Command 1 (retrieve mode, bit-wise, offset at 0x05A4, interval of 15):\n"\
                            "     steg -rboi 0x05A4 0x0F\n"\
                            "  Example Command 2 (store, byte-wise, offset at 0x1fe0, interval of 1, bytes={0xde, 0xad, 0xbe, 0xef}):\n"\
                            "     steg -soi 1fe0 1 de ad be ef\n"\
                            "          [---- OR ----]\n"\
                            "     steg -sBoi 0x1fe0 0x01 0xde 0xad 0xbe 0xef\n"\
                            "  Required Flags:\n"\
                            "     -s      Store Mode. Provide bytes to be steg'd after the flag string and all required arguments.\n"\
                            "     -r      Retrieve Mode\n"\
                            "  Optional Flags:\n"\
                            "     -b      Bit mode. Steg'd data is/was stored bit-wise. If not specified, steg defaults to byte mode.\n"\
                            "     -B      Byte mode. Steg'd data is/was stored byte-wise. This is the default mode.\n"\
                            "     -o      Offset. Steg'd data starts at specified offset.\n'"\
                            "     -i      Interval. Distance between each steg'd location.\n"

#define DRAKO_STEG_PRINTF_STR "DrakoStegConfig {\n"\
                              "    operation: %s\n"\
                              "    data mode: %s\n"\
                              "    offset   : 0x%04x\n"\
                              "    interval : 0x%02x\n}\n"

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
    DRAKO_CMD_STEG = 8,
    DRAKO_CMD_ENCRYPT = 9,
    DRAKO_CMD_DECRYPT = 10,
    DRAKO_CMD_HEXDUMP = 11,
    DRAKO_NOT_A_CMD = 255
};

// Enum for steganography modes
enum DrakoStegFlag {
    DRAKO_STEG_MODE_BIT   = 'b',
    DRAKO_STEG_MODE_BYTE  = 'B',
    DRAKO_STEG_STORE = 's',
    DRAKO_STEG_RETRV = 'r',
    DRAKO_STEG_OFFSET = 'o',
    DRAKO_STEG_INTERVAL = 'i',
    DRAKO_STEG_NO_OPERATION
};

// Drako steganography config struct
typedef struct _drako_steg_config {
    enum DrakoStegFlag operation;
    enum DrakoStegFlag mode;
    uint16_t offset;
    uint8_t interval;
    uint16_t nbytes;
} DrakoStegConfig;

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
bool drako_cmd_steg(const DrakoStegConfig config, uint8_t* bytes, uint16_t nbytes);
bool drako_cmd_encrypt(uint8_t* data, size_t nbytes);
bool drako_cmd_decrypt(uint8_t* data, size_t nbytes);
bool drako_cmd_hexdump();

// helper functions
bool _is_hex_str(const char* arg);
uint16_t _drako_parse_steg_cmd(const char* cmd, DrakoStegConfig* config, uint8_t* bytebuf, uint16_t nbytes);
void _drako_print_steg_config(DrakoStegConfig* config);

#endif
