use super::super::steg;

pub const STEG_OFFSET  : u8 = 250;
pub const STEG_INTERVAL: u8 = 47;
pub const STEG_N_BYTES : u8 = "Eltanin".len() as u8;

pub struct ChInfo {
    field_size : u16,
    data       : Vec<u8>,
    extracted  : String
}

impl ChInfo {
    pub fn new() -> ChInfo {
        let data: Vec<u8> = Vec::from(include_bytes!("../../../files/steg/NGC6543-stegged.png"));
        let extracted: String = String::from_utf8(steg::retrieve(&data, STEG_INTERVAL, STEG_OFFSET as u16, STEG_N_BYTES)).unwrap();
        ChInfo {
            field_size : data.len() as u16,
            data,
            extracted
        }
    }

    pub fn gen_field(&self) -> &Vec<u8> {
        &self.data
    }

    pub fn size(&self) -> u16 {
        self.data.len() as u16
    }

    pub fn display_info(&self, memspace_offset: u16) {
        println!("Challenge 4 {{");
        println!("    memspace offset: {:#06x}", memspace_offset);
        println!("    challenge flag: Eltanin");
        println!("    extracted flag: {} (should match `challenge flag`)", self.extracted);
        println!("    steg offset   : 250 (labeled resistor value)");
        println!("    steg interval :  47 (correct resistor value)");
        println!("    steg mode     : bit-wise retrieve");
        println!("    RSTEG Params  : {{");
        println!("        offset   : {:#06x}", memspace_offset + STEG_OFFSET as u16);
        println!("        interval : {:#04x}", STEG_INTERVAL);
        println!("        n_bytes  : {:#04x}", STEG_N_BYTES);
        println!("    }}");
        print!  ("    expected byte output  : ");
        for c in self.extracted.chars() {
            print!("{:02x} ", c as u8);
        } println!();
        println!("    correct rsteg command : rsteg {:#06x} {:#04x} {:#04x}", STEG_OFFSET as u16 + memspace_offset, STEG_INTERVAL, STEG_N_BYTES);
        println!("}}");
        //let mut i = memspace_offset + STEG_OFFSET as u16;
        //let bytes = include_bytes!("../../../generated/bin/memspace.bin");
        //while i < (STEG_OFFSET as u16)+memspace_offset+(STEG_INTERVAL as usize * self.extracted.len() * 8) as u16 {
        //    println!("addr={:04x}    byte={:02x}", i, bytes[i as usize]);
        //    i += STEG_INTERVAL as u16;
        //}
    }
}
