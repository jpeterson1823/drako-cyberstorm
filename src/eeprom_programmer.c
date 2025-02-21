#include <drako/drako.h>
#include <drako/hardware/at28c64b.h>
#include <hidden_challenge.h>

int main() {
    stdio_init_all();
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    drako_init();

    // place datablock 2312 bytes from beginning
    uint16_t datablock_offset = 0x0908;

    while (!tud_cdc_connected())
        sleep_ms(1000);
    sleep_ms(1000);

    at28c64b_select(&drako.prom);
    uint8_t byte;
    for (size_t i = 0; i < HIDDEN_CHALLENGE_DATABLOCK_SIZE; i++) {
        printf("Writing %02x to %04x... ", HIDDEN_CHALLENGE_DATABLOCK[i], datablock_offset + i);
        at28c64b_write8(&drako.prom, datablock_offset + i, HIDDEN_CHALLENGE_DATABLOCK[i]);

        at28c64b_read8(&drako.prom, datablock_offset + i, &byte);
        if (byte == HIDDEN_CHALLENGE_DATABLOCK[i])
            printf("VERIFIED\n");
        else {
            printf("FAILED! Read %02x\nHALTING\n", byte);
            break;
        }
    }

    // write increment to 0x1fff (decimal 30)
    at28c64b_write8(&drako.prom, 0x1fff, 30);

    gpio_put(25, 0);

    //uint8_t byte;
    //for (size_t i = 0; i < HIDDEN_CHALLENGE_DATABLOCK_SIZE; i++) {
    //    at28c64b_read8(&drako.prom, i, &byte);
    //    printf("%02x", byte);
    //}

    return 0;
}
