mod challenges;

use challenges::*;
use rand::prelude::*;

pub const DATABLOCK_OFFSET  : usize = 2312;
pub const EEPROM_SIZE       : usize = 0x2000;
pub const EEPROM_MAX_ADDR   : usize = 0x1FFE;   // last byte is used in challenge, so cannot use it

/// Minimum allowable number of bytes between hints.
pub const MIN_HINT_SEP: usize = 150; // setting this too high will cause infinite loop

/// Contains the hint itself and its offset within the EEPROM address space.
type Hint = (String, usize);
/// Calculates the address of the last byte of the hint
fn hint_tail(hint: &Hint) -> usize { hint.0.len() + hint.1 }

/// Holds all critical data for the entire hidden challenge
pub struct HiddenChallenge {
    header: String,
    c1: Challenge1,
    c2: Challenge2,
    c3: Challenge3,
    c4: Challenge4,

    datablock_head: usize,
    datablock_tail: usize,
    placed_hints: Vec<Hint>
}

impl HiddenChallenge {
    pub fn new() -> Self {
        // create empty vec for hints as they need to be added later
        let placed_hints = Vec::new();

        // generate challenges
        print!("Generating challenges...");
        let c1: Challenge1 = Challenge1::new();
        let c2: Challenge2 = Challenge2::new();
        let c3: Challenge3 = Challenge3::new();
        let c4: Challenge4 = Challenge4::new();
        println!("DONE");

        // generate header
        print!("Generating header... ");
        let header: String = format!(
            "\x01name=Drako Hidden Challenge\x1Ec1len={}\x1Ec2len={}\x1Ec3len={}\x1Ec4len={}\x02",
            c1.size(),
            c2.size(),
            c3.size(),
            c4.size()
        );
        println!("DONE");

        // calculate datablock size and head/tail offset
        let datablock_size: usize = header.len()+c1.size()+c2.size()+c3.size()+c4.size();
        let datablock_head: usize = DATABLOCK_OFFSET;
        let datablock_tail: usize = DATABLOCK_OFFSET + datablock_size;

        // construct and return struct
        HiddenChallenge {
            header, c1, c2, c3, c4,
            datablock_head,
            datablock_tail,
            placed_hints
        }
    }

    /// Generates challenge datablock
    pub fn generate_datablock(&self) -> Vec<u8> {
        let mut datablock: Vec<u8> = Vec::new();

        print!("Generating datablock...");
        datablock.extend(self.header.as_bytes());
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
            memspace[i+DATABLOCK_OFFSET] = datablock[i];
        }

        // generate hint offsets and place them in EEPROM memory space
        for hint in self.c2.get_hints() {
            // gen offset and store in placed_hints
            let offset = self.gen_rand_hint_offset(hint.len());
            self.placed_hints.push((hint.clone(), offset));

            // place hint in memory space
            for byte in hint.bytes().enumerate() {
                memspace[offset + byte.0] = byte.1;
            }
        }

        // return EEPROM memspace
        memspace
    }

    /// Generates valid offset for hint and ensures proper spacing and no overwrites.
    fn gen_rand_hint_offset(&self, hint_len: usize) -> usize {
        // get rng
        let mut rng = rand::rng();

        // attempt to generate random offset that wont overwrite occupied memory
        let mut offset: usize = rng.random_range(0..EEPROM_MAX_ADDR);
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
    pub fn is_valid_hint_offset(&self, hint_len: usize, offset: usize) -> bool {
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
        for placed_hint in self.placed_hints.iter() {
            // invalid if offset lies inside another hint
            if offset >= placed_hint.1 && offset <= hint_tail(&placed_hint) {
                //println!("INVALID: offset within another hint.");
                return false;
            }

            // invalid if hint's tail ends up in occupied space (indicating an overwrite)
            let tail: usize = offset+hint_len;
            if tail >= placed_hint.1 && tail <= hint_tail(&placed_hint) {
                //println!("INVALID: offset causes overwrite.");
                return false;
            }

            // invalid if hint is too close to any other hint
            if tail.abs_diff(placed_hint.1) < MIN_HINT_SEP || hint_tail(&placed_hint).abs_diff(tail) < MIN_HINT_SEP {
                //println!("INVALID: offset violates minimum hint separation.");
                return false;
            }
        }

        // valid if all checks are passed
        true
    }

    pub fn get_header(&self) -> &String { &self.header }
    pub fn get_c1(&self) -> &Challenge1 { &self.c1 }
    pub fn get_c2(&self) -> &Challenge2 { &self.c2 }
    pub fn get_c3(&self) -> &Challenge3 { &self.c3 }
    pub fn get_c4(&self) -> &Challenge4 { &self.c4 }
}
