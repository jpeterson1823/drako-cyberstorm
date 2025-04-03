#!/usr/bin/python3

# file paths
og_file = '../steg/NGC6543.png'
steg_file = '../steg/NGC6543-stegged.png'

# steg vars
img =  bytearray(open(og_file, 'rb').read())
data = b'Eltanin'
offset = 250
interval = 47

# steg `data` into `img` (bitwise, MSB first)
print('Stegging (bitwise, MSB first)... ', end='')
i = offset
c = 0
for byte in data:
    #print(f'{byte:02x} @ {i:04x}')
    # bitwise steg MSB first
    for bit in range(7, -1, -1):
        img[i] &= 0xFE
        img[i] |= (byte&(0x01<<bit))>>bit
        i+=interval
        c += 1
print(f'DONE!\n  Wrote {c} bits')

# write stegged image to new file
print(f'Writing stegged file to `{steg_file}`... ', end='')
with open(steg_file, 'wb') as f:
    f.write(img)
print("DONE")


print('Self Test')
print('    Extracting Stegged data...')
i = offset
ex = []
for _ in range(7):
    b = 0
    for bit in range(7, -1, -1):
        b |= (img[i]&0x01)<<bit
        i+=interval
    print(f'      BYTE={b:#04x}')
    ex.append(b)
print("    DONE! Extracted: ", end='')
for c in ex:
    print(f'{chr(c)}', end='')
print()

# check that extracted data matches `data`
p = True
for i in range(len(data)):
    if ex[i] != data[i]:
        p = False
        break
print('PASS' if p else 'FAIL')
