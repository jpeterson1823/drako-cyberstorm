#ifndef __drako_hardware_tests
#define __drako_hardware_tests

#include <drako/display.h>
#include <drako/hardware/at28c64b.h>

void full_test(eeprom* prom, display* disp);
size_t randomized_full_test(eeprom* prom, display* disp, size_t nbytes);

#endif
