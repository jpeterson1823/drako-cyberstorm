#include <drako/drako.h>
#include <hardware/flash.h>
#include <pico/flash.h>
#include <math.h>

// Drako's flash datablock starts 1MB from end of XIP
const uint32_t DRAKO_FLASH_OFFSET = XIP_END - DRAKO_FLASH_DATA_SIZE;

// Address of the first byte of Drako's flash datablock
const uint8_t* DRAKO_DATA_ADDR = (const uint8_t*) (XIP_END - DRAKO_FLASH_DATA_SIZE);

void drako_flash_program(void* data, size_t nbytes, uint32_t addr) {
    // construct FlashData
    FlashData flashData = {
        .bytes = (uint8_t*)data,
        .size = nbytes,
        .addr = addr
    };

    // create flash status integer
    int flashStatus;

    // erase necessary range
    flashStatus = flash_safe_execute(
            _drako_flash_erase_cb,
            (void*)&flashData,
            UINT32_MAX
    );
    hard_assert(flashStatus == PICO_OK);

    // program data
    flashStatus = flash_safe_execute(
            _drako_flash_program_cb,
            (void*)&flashData,
            UINT32_MAX
    );
    hard_assert(flashStatus == PICO_OK);
}

void _drako_flash_program_cb(void* param) {
    // convert param back into FlashData
    FlashData* data = (FlashData*)param;
    flash_range_program(
            DRAKO_FLASH_OFFSET + data->addr,
            data->bytes,
            FLASH_PAGE_SIZE * ceil((float)data->size / FLASH_PAGE_SIZE)
    );
}

// param is a pointer to flash_data_struct
void _drako_flash_erase_cb(void* param) {
    // convert param back into FlashData
    FlashData* data = (FlashData*)param;
    // erase necessary area
    flash_range_erase(
            DRAKO_FLASH_OFFSET + data->addr,
            FLASH_PAGE_SIZE * ceil((float)data->size / FLASH_PAGE_SIZE)
    );
}

