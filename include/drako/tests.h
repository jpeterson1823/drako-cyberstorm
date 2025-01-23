#ifndef __drako_hardware_tests
#define __drako_hardware_tests

#include <drako/modules/display.h>
#include <drako/hardware/at28c64b.h>

// uncomment to see full printout of randomized_full_test
//#define RNG_FULL_TEST_DEBUG


void full_test(eeprom* prom, display* disp);
size_t randomized_full_test(eeprom* prom, display* disp, size_t nbytes);

#endif
