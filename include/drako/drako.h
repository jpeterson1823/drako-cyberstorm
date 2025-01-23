#ifndef __drako_drako
#define __drako_drako

#include <drako/hardware/sn74hc595n.h>
#include <drako/hardware/at28c64b.h>
#include <drako/modules/display.h>
#include <drako/modules/terminal.h>
#include <drako/default_commands.h>

// typedef sn74hc595n and at28c64b for easier library use
typedef sn74hc595n shiftreg;
typedef at28c64b eeprom;

// create Drako struct to allow access to hardware
typedef struct drako_struct {
    eeprom prom;
    display disp;
} Drako;

static Drako drako;

/**
 * @brief Initializes Drako and its hardware
 */
static inline void drako_init() {
    // create eeprom struct and init
    at28c64b_init(&drako.prom,
        0x000000FF,     // data bus on GPIO[0:7]
        0x001FFF00,     // addr bus on GPIO[8:20]
        26, 22, 21
    );

    // create display struct and init
    display_init(&drako.disp);
}

#endif
