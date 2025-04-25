#ifndef __drako_terminal
#define __drako_terminal

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <pico/stdlib.h>
#include <tusb.h>


#define DRKO_TERM "[-DRAKO-]"
#define DRKO_PROMPT "drako >$ "

extern bool _term_connected;

void terminal_get_line(char* buf, size_t n);
bool terminal_get_line_timed(char* buf, size_t n, uint32_t timeout_ms);

bool _is_char_backspace(char c);
void terminal_println(const char* str);
void terminal_print(const char* str);
bool terminal_is_connected();
void terminal_sync();
void terminal_open_connection();
void terminal_prompt();
void _terminal_greet();
bool terminal_get_line_timed(char* buf, size_t n, uint32_t timeout_ms);
void terminal_get_line(char* buf, size_t n);


#endif
