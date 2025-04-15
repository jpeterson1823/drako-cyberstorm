#ifndef __drako_hidden_challenge
#define __drako_hidden_challenge

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <drako/drako.h>
#include <hidden_challenge.h>

extern bool hc_c2_complete;
extern bool hc_c2_animated;

static inline void hc_c2_update_clearance() {
    at28c64b_select(&drako.prom);
    uint8_t byte;
    at28c64b_read8(&drako.prom, HC_C1_CLEARANCE_ADDR, &byte);
    if (byte == HC_C1_CLEARANCE_BYTE) {
        hc_c2_complete = true;
        drako.currentLevel = 255;

        if (!hc_c2_animated) {
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
            hc_c2_animated = true;
        }
    }
    else
        hc_c2_complete = false;
}

#endif
