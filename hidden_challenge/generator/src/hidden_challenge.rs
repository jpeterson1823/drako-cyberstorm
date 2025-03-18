use rand::prelude::*;
use std::fs::File;
use std::io::Write;

pub mod constants {
    pub const C1_WRAPPER        : &str = "DRAKO";
    pub const DATABLOCK_OFFSET  : u16  = 2312;
    pub const EEPROM_SIZE       : u16  = 0x2000;
    pub const EEPROM_MAX_ADDR   : u16  = 0x1FFF;
    pub const HEADER_PATH       : &str = "./hidden_challenge.h";
}

/// Generates C Header file for EEPROM data.
pub fn generate_c_header(data: &Vec<u8>) {
    // create header string that will be written to a file later
    let mut header: String = format!(
        "#ifndef __drako_hc_data\n\
        #define __drako_hc_data\n\
        #include <stdint.h>\n\
        #define HIDDEN_CHALLENGE_DATABLOCK_SIZE {}\n\
        const uint8_t HC_EEPROM_DATA[] = {{\n",
        data.len());

    // iterate through data
    for byte in data.iter().enumerate() {
        // start new line in array literal every 16 bytes.
        // this is done by adding a newline, then adding 4 spaces for tabbing.
        if byte.0 > 0 && byte.0 % 16 == 0 {
            header += format!("{:0>2x},\n    ", byte.1).as_str();
        }

        // if final byte, add new line and closing squiggle and endif.
        else if byte.0 == data.len() - 1 {
            header += format!("{:0>2x}\n}};\n#endif", byte.1).as_str();
        }
    }

    // write header string to file
    let mut f = File::create(constants::HEADER_PATH).expect(format!("Failed to open file \"{}\"", constants::HEADER_PATH).as_str());
    f.write(header.as_bytes()).expect(format!("Failed to write data to {}", constants::HEADER_PATH).as_str());
    println!("C Header written to {}", constants::HEADER_PATH);

}

pub struct Challenge1 {
    field_size  : usize,
    subflags    : Vec<String>,
    sf_offsets  : Vec<usize>,
    flag        : String
}

pub struct Challenge2 {
    field_size: usize,
    hints: Vec<String>,
    riddle: String
}

pub struct Challenge3 {
    data: String
}

pub struct Challenge4 {
    data: Vec<u8>
}

impl Challenge1 {
    pub fn new() -> Challenge1{
        Challenge1 {
            field_size: 1000,
            subflags: vec![String::from("THUBAN"), String::from("DRACONIS")],
            sf_offsets: vec![123, 421],
            flag: String::from("discovery")
        }
    }

    /// Generate vector of challenge 1's binary data
    pub fn gen_field(&self) -> Vec<u8> {
        // create rng and field vector
        let mut rng = rand::rng();
        // generate Vec<u8> of length field_size that's filled w/rng bytes
        let mut bin: Vec<u8> = (0..self.field_size).map(|_| rng.random::<u8>()).collect();

        // wrap subflags
        let wrapped1: String = format!(
            "{}{}{}",
            constants::C1_WRAPPER,
            self.subflags[0],
            constants::C1_WRAPPER
        );
        let wrapped2: String = format!(
            "{}{}{}",
            constants::C1_WRAPPER,
            self.subflags[1],
            constants::C1_WRAPPER
        );

        // place wrapped subflags at their offset
        for t in wrapped1.chars().enumerate() {
            bin[t.0] = t.1 as u8;
        }
        for t in wrapped2.chars().enumerate() {
            bin[t.0] = t.1 as u8;
        }

        return bin;
    }

    pub fn size(&self) -> usize {
        self.field_size
    }
}

impl Challenge2 {
    pub fn new() -> Challenge2 {
        // load riddle into string
        let riddle: String = String::from(include_str!("../files/riddle.txt"));

        // load hints into string vector
        let mut hints: Vec<String> = Vec::new();
        for line in include_str!("../files/riddle-hints.txt").lines() {
            if line != "" {
                hints.push(String::from(line));
            }
        }
        
        // return constructed struct
        Challenge2 {
            field_size: riddle.len(),
            hints,
            riddle
        }
    }

    /// Generates vector of challenge 2's binary data
    pub fn gen_field(&self) -> Vec<u8> {
        self.riddle.as_bytes().to_vec()
    }

    pub fn get_hints(&self) -> &Vec<String> {
        &self.hints
    }

    pub fn size(&self) -> usize {
        self.field_size
    }
}

impl Challenge3 {
    pub fn new() -> Challenge3 {
        Challenge3 {
            data: String::from("Good things come in threes...")
        }
    }

    pub fn gen_field(&self) -> Vec<u8> {
        self.data.as_bytes().to_vec()
    }

    pub fn size(&self) -> usize {
        self.data.len()
    }
}

impl Challenge4 {
    pub fn new() -> Challenge4 {
        Challenge4 {
            data: Vec::from(include_bytes!("../files/steg/NGC6543-stegged.png"))
        }
    }

    pub fn gen_field(&self) -> &Vec<u8> {
        &self.data
    }

    pub fn size(&self) -> usize {
        self.data.len()
    }
}
