#!/usr/bin/python3

img =  bytearray(open("./NGC6543.png", "rb").read())

data = bytearray(open("./fc.txt", "r").read(), encoding="UTF-8")

offset = 250 - 47
interval = 15   

print("Stegging... ")
i = offset
for byte in data:
    print(f"{byte:02x} @ {i:04x}")
    img[i] = byte
    i+=interval
# write stegged image to new file
with open("./NGC6543-stegged.png", "wb") as f:
    f.write(img)
print("Done")
