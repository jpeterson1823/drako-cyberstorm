#!/usr/bin/python3

# read in datablock
datablock = open("datablock.bin", "rb").read()

n = 0x13
offset = 0x0f
interval = 0x12

img_start = 0x094F + 1000 + 640 + 29 + 15
print(f"Image Start Addr: {img_start-30:02x}")
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
