pub mod challenge;
pub mod header;
pub mod crypto;
pub mod steg;

use challenge::*;
use header::{Field, FieldType};
use rand::prelude::*;

pub const DATABLOCK_OFFSET  : u16 = 2312;
pub const EEPROM_SIZE       : u16 = 0x2000;
pub const EEPROM_MAX_ADDR   : u16 = 0x1FFE;   // last byte is used in challenge, so cannot use it

/// Minimum allowable number of bytes between hints.
pub const MIN_HINT_SEP: u16 = 150; // setting this too high will cause infinite loop
                                   //
/// Holds all critical data for the entire hidden challenge
pub struct HiddenChallenge {
    header: Vec<Field>,
    hlen: usize,
    c1: ch1::ChInfo,
    c2: ch2::ChInfo,
    c3: ch3::ChInfo,
    c4: ch4::ChInfo,

    datablock_size: u16,
    datablock_head: u16,
    datablock_tail: u16,
}

impl HiddenChallenge {
    pub fn new() -> Self {
        // generate challenges
        print!("Generating challenges...");
        let c1: ch1::ChInfo = ch1::ChInfo::new(DATABLOCK_OFFSET);
        let c2: ch2::ChInfo = ch2::ChInfo::new();
        let c3: ch3::ChInfo = ch3::ChInfo::new();
        let c4: ch4::ChInfo = ch4::ChInfo::new();
        println!("DONE");

        // generate header
        print!("Generating header... ");
        let header: Vec<header::Field> = vec![
            Field::new("name", FieldType::Text(String::from("Drako Hidden Challenge"))),
            Field::new("c1len", FieldType::Size(c1.size())),
            Field::new("c2len", FieldType::Size(c2.size())),
            Field::new("c3len", FieldType::Size(c3.size())),
            Field::new("c4len", FieldType::Size(c4.size())),
        ];
        println!("DONE");

        // calc header size: STX byte + # of fields + field dividers + EOT byte
        let hlen = header::calc_len(&header);

        // calculate datablock size and head/tail offset
        let datablock_size: u16 = hlen as u16 + c1.size() + c2.size() + c3.size() + c4.size();
        let datablock_head: u16 = DATABLOCK_OFFSET;
        let datablock_tail: u16 = DATABLOCK_OFFSET + datablock_size;

        // construct and return struct
        HiddenChallenge {
            header, hlen, c1, c2, c3, c4,
            datablock_size,
            datablock_head,
            datablock_tail,
        }
    }

    /// Generates challenge datablock
    pub fn generate_datablock(&self) -> Vec<u8> {
        let mut datablock: Vec<u8> = Vec::new();

        print!("Generating datablock...");
        datablock.extend(header::generate(&self.header).as_bytes());
        datablock.extend(self.c1.gen_field().iter());
        datablock.extend(self.c2.gen_field().iter());
        datablock.extend(self.c3.gen_field().iter());
        datablock.extend(self.c4.gen_field().iter());
        println!("DONE");

        // return datablock
        datablock
    }

    /// Generate entire EEPROM memory space
    pub fn generate_eeprom_memspace(&mut self, datablock: &Vec<u8>) -> Vec<u8> {
        // create memory space for EEPROM, filled with random data
        let mut rng = rand::rng();
        let mut memspace: Vec<u8> = (0..EEPROM_SIZE).map(|_| rng.random::<u8>()).collect();

        // place datablock at its offset
        for i in 0..datablock.len() {
            memspace[i+(DATABLOCK_OFFSET as usize)] = datablock[i];
        }

        // generate hint offsets and place them in EEPROM memory space
        let mut offsets: Vec<u16> = Vec::new();
        for hint in self.c2.get_hints().iter() {
            // gen offset
            let offset: u16 = self.gen_rand_hint_offset(hint.0.len() as u16);
            offsets.push(offset);

            // place hint in memory space
            for byte in hint.0.bytes().enumerate() {
                memspace[(offset as usize) + byte.0] = byte.1;
            }
        }
        // update hints in c2
        self.c2.update_hint_offsets(offsets);

        // return EEPROM memspace
        memspace
    }

    /// Generates valid offset for hint and ensures proper spacing and no overwrites.
    fn gen_rand_hint_offset(&self, hint_len: u16) -> u16 {
        // get rng
        let mut rng = rand::rng();

        // attempt to generate random offset that wont overwrite occupied memory
        let mut offset: u16 = rng.random_range(0..EEPROM_MAX_ADDR);
        while !self.is_valid_hint_offset(hint_len, offset) {
            offset = rng.random_range(0..EEPROM_MAX_ADDR);
        }

        // return valid generated offset
        offset
    }

    /// Determines if the specified hint offset is valid.
    /// Returns true for the specified offset iff:
    ///   - The hint can fit on the EEPROM at the specified offset
    ///   - The hint does not overwrite occupied memory
    ///   - The distance between the hint and any other hint is >= MIN_HINT_SEP
    pub fn is_valid_hint_offset(&self, hint_len: u16, offset: u16) -> bool {
        // invalid if hint cannot fit on EEPROM at specified offset.
        if offset + hint_len > EEPROM_MAX_ADDR {
            //println!("INVALID: offset causes hint to no longer fit on EEPROM.");
            return false;
        }

        // invalid if the offset's inside challenge datablock
        if offset >= self.datablock_head && offset <= self.datablock_tail {
            //println!("INVALID: offset within datablock.");
            return false;
        }

        // check offset against every placed hint
        for hint in self.c2.get_hints().iter() {
            // invalid if offset lies inside another hint
            if offset >= hint.1 && offset <= self.hint_tail(&hint) {
                //println!("INVALID: offset within another hint.");
                return false;
            }

            // invalid if hint's tail ends up in occupied space (indicating an overwrite)
            let tail: u16 = offset+hint_len;
            if tail >= hint.1 && tail <= self.hint_tail(&hint) {
                //println!("INVALID: offset causes overwrite.");
                return false;
            }

            // invalid if hint is too close to any other hint
            if tail.abs_diff(hint.1) < MIN_HINT_SEP || self.hint_tail(&hint).abs_diff(tail) < MIN_HINT_SEP {
                //println!("INVALID: offset violates minimum hint separation.");
                return false;
            }
        }

        // valid if all checks are passed
        true
    }

    fn hint_tail(&self, hint: &(String, u16)) -> u16 {
        hint.1 + hint.0.len() as u16
    }

    pub fn get_header(&self) -> &Vec<Field> { &self.header }
    pub fn get_c1(&self) -> &ch1::ChInfo { &self.c1 }
    pub fn get_c2(&self) -> &ch2::ChInfo { &self.c2 }
    pub fn get_c3(&self) -> &ch3::ChInfo { &self.c3 }
    pub fn get_c4(&self) -> &ch4::ChInfo { &self.c4 }
    pub fn get_datablock_size(&self) -> u16 { self.datablock_size }

    pub fn calc_c1_memspace_offset(&self) -> u16 { DATABLOCK_OFFSET + self.hlen as u16 }
    pub fn calc_c2_memspace_offset(&self) -> u16 { self.calc_c1_memspace_offset() + self.c1.size() }
    pub fn calc_c3_memspace_offset(&self) -> u16 { self.calc_c2_memspace_offset() + self.c2.size() }
    pub fn calc_c4_memspace_offset(&self) -> u16 { self.calc_c3_memspace_offset() + self.c3.size() }

    pub fn display_info(&self) {
        println!("\n");
        println!("########################################");
        println!("##### HIDDEN CHALLENGE INFORMATION #####");
        println!("########################################\n");
        // display header info
        println!("Header Length: {}", self.hlen);
        println!("Challenge Header Fields {{");
        for field in self.header.iter().enumerate() {
            print!("    \"{}\"", field.1.to_string().as_str());
            if field.0 == self.header.len() - 1 {
                println!("\n}}\n");
            } else {
                print!("\n");
            }
        }

        // display challenge 1 info
        self.c1.display_info(self.calc_c1_memspace_offset()); println!();
        self.c2.display_info(self.calc_c2_memspace_offset()); println!();
        self.c3.display_info(self.calc_c3_memspace_offset()); println!();
        self.c4.display_info(self.calc_c4_memspace_offset()); println!();
    }
}
