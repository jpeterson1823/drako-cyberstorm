#include <drako/drako.h>

/**
 * @brief Reads byte from Drako's flash datablock.
 * @param addr Datablock address for read.
 * @return Byte at specified address.
 */
uint8_t drako_datablock_read(uint32_t addr) {
    return DRAKO_DATABLK[addr];
}

/**
 * @brief Reads multiple bytes into provided buffer.
 * @param addr Datablock address at which to start reading.
 * @param buf Preallocated byte buffer.
 * @param bufsize Size of preallocated byte buffer in bytes.
 * @return true if all bytes are read into buffer. false otherwise.
 */
bool drako_datablock_read_buf(uint32_t addr, char* buf, size_t bufsize) {
    size_t i;
    for (i = 0; i < bufsize && addr+i < DRAKO_DATABLK_END; i++)
        buf[i] = DRAKO_DATABLK[addr+i];
    return addr+i <= DRAKO_DATABLK_END && i <= bufsize;
}
