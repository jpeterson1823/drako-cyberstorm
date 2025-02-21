#!/usr/bin/python3

# read in datablock
datablock = open("datablock.bin", "rb").read()

n = 0x7
interval = 0x14
offset = 0x094F + 1000 + 640 + 29 + 30
print(f"Image Start Addr: {offset-30:02x}")
print(f"Steg  Start Addr: {offset:02x}")

d = []
for i in range(n):
    d.append(datablock[offset])
    offset += interval


print("Bytes Retrieved:")
s = ""
for byte in d:
    print(f"{byte:02x} ")
    s += chr(byte)
print("String: " + s)
