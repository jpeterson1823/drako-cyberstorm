#ifndef __drako_drako
#define __drako_drako

#include <drako/hardware/sn74hc595n.h>
#include <drako/hardware/at28c64b.h>
#include <drako/modules/display.h>
#include <drako/modules/terminal.h>
#include <hidden_challenge.h>

#define DRAKO_BUFSIZE 255
#define DRAKO_EEPROM_SIZE 0x2000


// create Drako struct to allow access to hardware
typedef struct drako_struct {
    at28c64b prom;
    display disp;
    bool exit_flag;
    bool hasMagicCreds;
} Drako;

// extern Drako definition so all files that include this header can see it.
extern Drako drako;

void drako_init();
void drako_reset_eeprom();
void hc_c2_update_clearance();

#endif

