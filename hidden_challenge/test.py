#!/usr/bin/python3

# read in datablock
datablock = open("datablock.bin", "rb").read()

n = 0x13
offset = 250  # resistor value on PCB is incorrect
interval = 15 # check other files for description

img_start = 0x094F + 1000 + 640 + 29 + offset
print(f"Image Start Addr: {img_start-offset:02x}")
print(f"Steg  Start Addr: {img_start:02x}")

d = []
for i in range(n):
    d.append(datablock[img_start])
    img_start += interval


print("Bytes Retrieved:")
s = ""
for byte in d:
    print(f"{byte:02x} ")
    s += chr(byte)
print("String: " + s)
