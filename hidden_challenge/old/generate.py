#!/usr/bin/python3
import random
from files.scripts import vigenere

SUBFLAG1 = [ord(x) for x in "THUBAN"]
SUBFLAG2 = [ord(x) for x in "DRACONIS"]

FLAG1 = "discovery"
FLAG2 = "understanding"

DATABLOCK_OFFSET = 2312
EEPROM_SIZE = 0x2000
EEPROM_MAX_ADDR = 0x1fff

wrapper = [ord(x) for x in "DRAKO"]

# parse riddle hints correctly
#riddle_hints = open('files/riddle-hints.txt', 'r').readlines();

def xor(data: bytearray, key: bytearray) -> bytearray:
    cipher = bytearray()
    j = 0
    for i in range(len(data)):
        if j >= len(key):
            j = 0
        cipher.append(data[i] ^ key[j])
        j += 1
    return cipher


class datablock_header:
    def __init__(self) -> None:
        self._fields: list[tuple[str, str]] = []

    def add_field(self, field: tuple[str, str]) -> None:
        self._fields.append(field)
        print(f"  Added field: {field[0]}={field[1]}")

    def update_field(self, field: tuple[str, str]) -> bool:
        # look for field with matching key and replace it
        for i in range(len(self._fields)):
            if self._fields[i][0] == field[0]:
                self._fields[i] = field
                return True
        # if field with key not found, return false
        return False

    # field format: key=value<EM>
    def _serialize(self, t: tuple[str,str]) -> bytearray:
        return bytearray(t[0] + '=' + t[1] + chr(0x1E), encoding="utf-8")

    def build(self) -> bytearray:
        # start header with ASCII SOH (Start of Heading)
        header = bytearray([0x01])

        # serialize all header fields and contiguously append them to a bytearray 
        for i in range(0, len(self._fields)):
            header.extend(self._serialize(self._fields[i]))

        # end header with ASCII STX (Start of Text [in this case, data])
        header.extend(bytearray([0x02]))
        return header

def generate_c1() -> bytearray:
    # create wrapped flag
    f1wrapped = wrapper + SUBFLAG1 + wrapper
    f2wrapped = wrapper + SUBFLAG2 + wrapper

    # randomly generate a fungload of data
    n = 1000
    data = bytearray()
    for i in range(n):
        data.append(random.randint(0x00, 0xFF))

    # store wrapped flags at predetermined location
    f1offset = 123
    for i in range(len(f1wrapped)):
        data[f1offset + i] = f1wrapped[i]
    f2offset = len(f1wrapped) + 421
    for i in range(len(f2wrapped)):
        data[f2offset + i] = f2wrapped[i]

    # calc mem addr and value for inc var
    memaddr = sum(SUBFLAG1) % 0x1fff
    memincv = sum(SUBFLAG2) % 0xff

    # display memory address and value to be written for p2 to unlock
    # mask to fit within 12 bits
    print(f"  Memory Address: {memaddr:#06x} ({memaddr})")
    # modulo to set bounds between 0x00-0xff
    print(f"  Value         : {memincv:#04x} ({memincv})")

    # display steg info
    #print(f"    Steg Offset   : {sum(SUBFLAG1) % 0xff}")
    #print(f"    Steg Interval : {sum(SUBFLAG2) % 0xff}")
    print(f"  Steg Offset   : 250")
    print(f"  Steg Interval : 15")

    return data

def generate_c2() -> bytearray:
    return bytearray(open("files/riddle.txt", 'r').read(), encoding="UTF-8")

def hexdump(barr: bytearray) -> None:
    c = 0
    for byte in barr:
        # add newline after 16 hex digits
        if c % 16 == 0 and c != 0:
            print('')
        print(f"{(byte):#04x}", end=" ")
        c+=1
    print('')

def generate_datablock_c_header(datablock: bytearray) -> None:
    with open("hidden_challenge.h", "w+") as f:
        # write include guard and byte array definition
        f.write(
            "#ifndef __drako_hidden_challenge_data\n"
          + "#define __drako_hidden_challenge_data\n\n"
          + "#include <stdint.h>\n\n"
          + f"#define HIDDEN_CHALLENGE_DATABLOCK_SIZE {len(datablock)}\n"
          + "const static uint8_t HIDDEN_CHALLENGE_DATABLOCK[] = {\n    "
        );

        # write all bytes to array
        for i in range(len(datablock)-1):
            f.write(f"{datablock[i]:#04x},")
            if (i+1) % 16 == 0:
                f.write("\n    ")
            else:
                f.write(" ")
        f.write(f"{datablock[-1]:#04x}\n")

        # close array and close include guard
        f.write("};\n\n#endif");
    print("Generated Challenge C Header File @ ./hidden_challenge.h")


def main():
    random.seed(sum([ord(x) for x in "S-B312: Assigned Noble 6"]))

    print("Generating challenge datablocks...")
    # generate challenge datablocks
    c1data = generate_c1()
    c2data = generate_c2()
    c3data = bytearray("Good things come in threes...", encoding="UTF-8")
    fcdata = bytearray(open("./files/steg/NGC6543-stegged.png", "rb").read())
    print("DONE\n")

    print("Generating datablock header...")
    # create header
    header = datablock_header()
    # add header data
    header.add_field(("name", "Drako Hidden Challenge"))
    header.add_field(("c1len", str(len(c1data))))
    header.add_field(("c2len", str(len(c2data))))
    header.add_field(("c3len", str(len(c3data))))
    header.add_field(("fclen", str(len(fcdata))))
    print("DONE\n")

    # generate full datablock
    datablock = header.build() + c1data + c2data + c3data + fcdata
    print("Datablock built.")

    # generate c header file
    generate_datablock_c_header(datablock)

    # generate raw binary file and fill empty space with random bytes
    bin = [random.randint(0x00, 0xFF) for _ in range(DATABLOCK_OFFSET)]
    bin.extend(datablock)
    bin.extend([random.randint(0x00, 0xFF) for _ in range(EEPROM_SIZE - len(bin))])

    # riddle stuffs
    riddle_answer_addr_hint_addr = 65 # only whole number value in Drako's astonomical coords (found on handout)
    riddle_answer_addr_hint = "Heed not the deceptive sigil upon my resistor, inscribed as \"250 ohms\" by mortal hands--for the answer to your question is found at the difference between the false and true."
    riddle_answer_addr = 250 - 47   # incorrect resistor value - actual resistor value
    riddle_answer_vig_key = "DRAKO"
    riddle_xor_key = bytearray(wrapper)
    riddle_answer_cipher = xor(bytearray(vigenere.encode(FLAG2, riddle_answer_vig_key),"UTF-8"), riddle_xor_key)
    riddle_answer_barr = "Who am I? I am ".encode("UTF-8") + riddle_answer_cipher;
    riddle_answer_xor_key_hint = "Amidst arcane whispers, 'DRAKO' encircles its most hallowed treasures."
    riddle_answer_xor_key_hint_addr = 523
    hint_occam_addr = 1234
    hint_occam = "Occam's Razor will serve you well in this challenge."

    interval_hint_addr = 1800
    interval_hint = "The key to all requires that which drives the curious; a pair in which the second cannot exist without the first. Combine these two, piece by piece, to obtain the interval!"
    offset_hint_addr = 986
    offset_hint = "Use the correct resistor value for the offset."

    # place riddle answer
    for i in range(len(riddle_answer_barr)):
        bin[riddle_answer_addr + i] = riddle_answer_barr[i]

    # place riddle answer address hint
    for i in range(len(riddle_answer_addr_hint)):
        bin[riddle_answer_addr_hint_addr + i] = ord(riddle_answer_addr_hint[i])

    # place riddle xor key hint
    for i in range(len(riddle_answer_xor_key_hint)):
        bin[riddle_answer_xor_key_hint_addr + i] = ord(riddle_answer_xor_key_hint[i])

    # place occam advisory
    for i in range(len(hint_occam)):
        bin[hint_occam_addr + i] = ord(hint_occam[i])

    # place interval hint
    for i in range(len(interval_hint)):
        bin[interval_hint_addr + i] = ord(interval_hint[i])

    # place offset hint
    for i in range(len(offset_hint)):
        bin[offset_hint_addr + i] = ord(offset_hint[i])

    # write binary to file
    with open("datablock.bin", "wb") as f:
        f.write(bytearray(bin))
    print("Generated binary file: datablock.bin")

if __name__ == "__main__":
    main()
