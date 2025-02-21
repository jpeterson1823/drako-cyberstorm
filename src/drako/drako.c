#include <drako/drako.h>
#include <hidden_challenge.h>
#include <hidden/hidden.h>

// define global drako for externs
Drako drako;

void drako_reset_eeprom() {
    printf("DRAKO RESETTING. THIS WILL TAKE ABOUT A MINUTE.\n");
    // place datablock 2312 bytes from beginning
    uint16_t datablock_offset = 0x0908;

    // select eeprom
    at28c64b_select(&drako.prom);

    uint8_t byte;
    for (size_t i = 0; i < HIDDEN_CHALLENGE_DATABLOCK_SIZE; i++) {
        //printf("Writing %02x to %04x... ", HIDDEN_CHALLENGE_DATABLOCK[i], datablock_offset + i);
        at28c64b_write8(&drako.prom, datablock_offset + i, HIDDEN_CHALLENGE_DATABLOCK[i]);

        at28c64b_read8(&drako.prom, datablock_offset + i, &byte);
        //if (byte == HIDDEN_CHALLENGE_DATABLOCK[i])
        //    printf("VERIFIED\n");
        //else {
        //    printf("FAILED! Read %02x\nHALTING\n", byte);
        //    break;
        //}
    }

    // set timeout byte to 0x1fff (decimal 30)
    at28c64b_write8(&drako.prom, 0x1fff, 30);
    
    // reset credential byte
    at28c64b_write8(&drako.prom, HC_C2_CLEARANCE_ADDR, 0xff);

    printf("DRAKO RESET COMPLETED.\n");
}
