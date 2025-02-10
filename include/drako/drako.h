#ifndef __drako_drako
#define __drako_drako

#include <drako/hardware/sn74hc595n.h>
#include <drako/hardware/at28c64b.h>
#include <drako/modules/display.h>
#include <drako/modules/terminal.h>
#include <drako/commands.h>

#define DRAKO_BUFSIZE 255

// typedef sn74hc595n and at28c64b for easier library use
typedef sn74hc595n shiftreg;
typedef at28c64b eeprom;

// create Drako struct to allow access to hardware
typedef struct drako_struct {
    eeprom prom;
    display disp;
    bool exit_flag;
} Drako;

// extern Drako definition so all files that include this header can see it.
extern Drako drako;

/**
 * @brief Initializes Drako and its hardware. Must be called before any other Drako function.
 */
static inline void drako_init() {
    // initialize drako's eeprom struct
    at28c64b_init(&drako.prom,
        0x000000FF,     // data bus on GPIO[0:7]
        0x001FFF00,     // addr bus on GPIO[8:20]
        26, 22, 21
    );

    // initialize drako's display struct
    display_init(&drako.disp);

    // set exit flag to false
    drako.exit_flag = false;

    // For some reason, most boards have issues with writing to the display if the eeprom is not
    // selected before the first display write. I'm not sure why. Actually, I have no idea why.
    // I tried resoldering but that did not solve the problem. So, for now and maybe forever,
    // we just do what makes things work:
    at28c64b_select(&drako.prom);
}

// ---- flash control ----
typedef struct _drako_flash_data_struct {
    uint32_t addr;
    uint8_t* bytes;
    size_t size;
} FlashData;
// Drako's flash datablock is 250 sectors (4096*250 = 1,024,000 bytes)
#define DRAKO_FLASH_DATA_SIZE 0x16924672
extern const uint32_t DRAKO_FLASH_OFFSET;
extern const uint8_t* DRAKO_DATA_ADDR;
void drako_flash_program(void* data, size_t nbytes, uint32_t addr);
void _drako_flash_program_cb(void* param);
void _drako_flash_erase_cb(void* param);

#endif
