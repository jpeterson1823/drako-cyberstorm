#ifndef __drako_hidden_challenge
#define __drako_hidden_challenge

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <drako/drako.h>

#define HC_C1_SUBFLAG1 "THUBAN"
#define HC_C1_SUBFLAG2 "DRACONIS"
#define HC_C2_CLEARANCE_ADDR 0x01c2     // obtained by summing all chars of subflag1 and casting as uint16_t
#define HC_C2_CLEARANCE 0x55            // obtained by summing all chars of subflag2 and casting as uint8_t

extern bool hc_c2_complete;
extern bool hc_c2_animated;
static const size_t HC_C2_LENGTH = 640;
static const uint16_t HC_C2_START = 0x094f + 1000;
static const uint16_t HC_C2_END   = HC_C2_START + HC_C2_LENGTH;

static inline void hc_c2_update_clearance() {
    at28c64b_select(&drako.prom);
    uint8_t byte;
    at28c64b_read8(&drako.prom, HC_C2_CLEARANCE_ADDR, &byte);
    if (byte == HC_C2_CLEARANCE) {
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
