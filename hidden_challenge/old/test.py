#!/usr/bin/python3

# read in datablock
datablock = open("datablock.bin", "rb").read()


def steg_test(img_start, offset, interval, nbytes):
    print(f"Image Start Addr: {img_start-offset:02x}")
    print(f"Steg  Start Addr: {img_start:02x}")

    d = []
    for _ in range(n):
        d.append(datablock[img_start])
        img_start += interval

    s = ""
    for byte in d:
        s += chr(byte)
    print("Stegged String: \"" + s + "\"")

if __name__ == "__main__":
    n = 0x13
    offset = 250-47  # correct resistor value on PCB
    interval = 15    # SUM(FLAG1+FLAG2) % 0xFF <-- add all char bytes; steg program will store it in uint8_t
    img_start = 0x094F + 1000 + 683 + 29 + offset

    # run steg test
    steg_test(img_start, offset, interval, n)
