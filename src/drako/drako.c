#include <drako/drako.h>
#include <hidden_challenge.h>


// create global drako object for externs
Drako drako;

/**
 * @brief Initializes Drako and its hardware. Must be called before any other Drako function.
 */
void drako_init() {
    // initialize drako's eeprom struct
    at28c64b_init(&drako.prom,
        0x000000FF,     // data bus on GPIO[0:7]
        0x001FFF00,     // addr bus on GPIO[8:20]
        26, 22, 21
    );

    // initialize drako's display struct
    display_init(&drako.disp);

    // For some reason, most boards have issues with writing to the display if the eeprom is not
    // selected before the first display write. I'm not sure why. Actually, I have no idea why.
    // I tried resoldering but that did not solve the problem. So, for now and maybe forever,
    // we just do what makes things work:
    at28c64b_select(&drako.prom);

    // set exit flag to false
    drako.exit_flag = false;

    // check magic credentials and update var accordingly
    uint8_t byte;
    at28c64b_read8(&drako.prom, HC_C1_CLEARANCE_ADDR, &byte);
    drako.hasMagicCreds = (byte == HC_C1_CLEARANCE_BYTE);
}

void drako_reset_eeprom() {
    printf("DRAKO RESETTING. THIS WILL TAKE ABOUT A MINUTE.\n");

    // select eeprom
    at28c64b_select(&drako.prom);

    // write entire memspace
    uint8_t byte;
    for (size_t i = 0; i < HC_MEMSPACE_SIZE; i++) {
        //printf("Writing %02x to %04x... ", HIDDEN_CHALLENGE_DATABLOCK[i], datablock_offset + i);
        at28c64b_write8(&drako.prom, i, HIDDEN_CHALLENGE_MEMSPACE[i]);

        //at28c64b_read8(&drako.prom, i, &byte);
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
    at28c64b_write8(&drako.prom, HC_C1_CLEARANCE_ADDR, 0xff);

    printf("DRAKO RESET COMPLETED.\n");
}
