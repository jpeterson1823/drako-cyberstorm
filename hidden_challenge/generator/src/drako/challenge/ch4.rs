pub struct ChInfo {
    field_size : u16,
    data       : Vec<u8>
}

impl ChInfo {
    pub fn new() -> ChInfo {
        let data: Vec<u8> = Vec::from(include_bytes!("../../../files/steg/NGC6543-stegged.png"));
        ChInfo {
            field_size : data.len() as u16,
            data
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
        println!("    challenge flag: Eltanin");
        println!("    steg offset   : 250 (labeled resistor value)");
        println!("    steg interval :  47 (correct resistor value)");
        println!("    steg mode     : bit-wise retrieve");
        println!("}}");
    }
}
