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

static inline void hc_c2_update_clearance() {
    // otherwise, check if creds have been unlocked
    at28c64b_select(&drako.prom);
    uint8_t byte;
    at28c64b_read8(&drako.prom, HC_C1_CLEARANCE_ADDR, &byte);
    if (byte == HC_C1_CLEARANCE_BYTE) {
        // if magic creds already obtained, skip animation
        if (drako.hasMagicCreds)
            return;

        // set magic creds var
        drako.hasMagicCreds = true;

        // dramatic phrasing with dramatic timing
        printf("QUANTUM ENCHANTMENT INITIATED");
        sleep_ms(1000);
        printf(".");
        sleep_ms(1000);
        printf(".");
        sleep_ms(1000);
        printf(".");
        sleep_ms(1000);
        printf(" INITIALIZED\n");
        sleep_ms(1000);

        printf("COSMIC SIGNAL {[                    ]}\rCOSMIC SIGNAL {[");
        sleep_ms(250);
        for(uint8_t i = 0; i < 20; i++) {
            printf("=");
            sleep_ms(250);
        }
        sleep_ms(1000);
        printf("]}>>> STRONG\n");
        sleep_ms(250);
        printf("CLEARANCE: ARCHMAGE IV\n");
        sleep_ms(250);
        printf("<--- ACCESS GRANTED --->\n");
        printf("\n     KEY: discovery\n\n");
    }

    // either does not yet have creds or overwrote creds byte
    else drako.hasMagicCreds = false;
}

#endif

