use rand::prelude::*;

pub const C1_WRAPPER: &str = "DRAKO";
const C3_MESSAGE    : &str = "To obtain the "


// Challenge Structs
pub struct Challenge1 {
    field_size  : u16,
    subflags    : Vec<String>,
    sf_offsets  : Vec<usize>,
    flag        : String
}

pub struct Challenge2 {
    field_size : u16,
    hints      : Vec<(String, u16)>,
    riddle     : String
}

pub struct Challenge3 {
    field_size : u16,
    data       : String
}

pub struct Challenge4 {
    field_size : u16,
    data       : Vec<u8>
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
        // generate Vec<u8> of length field_size that's filled w/rng bytes
        let mut rng = rand::rng();
        let mut bin: Vec<u8> = (0..self.field_size).map(|_| rng.random::<u8>()).collect();

        // wrap subflags
        let wrapped1: String = format!(
            "{}{}{}",
            C1_WRAPPER,
            self.subflags[0],
            C1_WRAPPER
        );
        let wrapped2: String = format!(
            "{}{}{}",
            C1_WRAPPER,
            self.subflags[1],
            C1_WRAPPER
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

    pub fn size(&self) -> u16 {
        self.field_size
    }

    pub fn display_info(&self, memspace_offset: u16) {
        // get max len of subflag for display padding
        let subflag_padding: usize = self.subflags.iter().map(|sf: &String| sf.len()).max().unwrap();

        // calculate magic credential offset and password byte
        let mc_addr: u16 = self.subflags[0].chars().map(|c| c as u32).sum::<u32>() as u16;
        let mc_pass: u8  = self.subflags[1].chars().map(|c| c as u32).sum::<u32>() as u8;

        // do the displaying
        println!("Challenge 1 {{");
        println!("    challenge flag  : discovery");
        println!("    memspace offset : {:#06x}", memspace_offset);
        println!("    size            : {}", self.field_size);
        println!("    subflags {{");
        println!("        {:<subflag_padding$} : local offset = {:#06x}", self.subflags[0], self.sf_offsets[0]);
        println!("        {:<subflag_padding$} : local offset = {:#06x}", self.subflags[1], self.sf_offsets[1]);
        println!("    }}");
        println!("    magic credentials {{");
        println!("        {:<subflag_padding$} ---> {:#06x}", self.subflags[0], mc_addr);
        println!("        {:<subflag_padding$} ---> {:#06x}", self.subflags[1], mc_pass);
        println!("    }}");
    }
}

impl Challenge2 {
    pub fn new() -> Challenge2 {
        // load riddle into string
        let riddle: String = String::from(include_str!("../../files/riddle.txt"));

        // load hints into string vector
        let mut hints: Vec<(String, u16)> = Vec::new();
        let mut hint: String = String::new();
        for line in include_str!("../../files/riddle-hints.txt").lines() {
            // if part of hint, add to subriddle var
            if line != "" {
                // add newline if not first thing in hint
                if hint != "" {
                    hint.push('\n');
                }
                hint.push_str(line);
            }
            // raw newline means end of hint, so push to hints vec
            else {
                hints.push((hint, 0u16));
                // make sure to create new string as ownership has changed
                // bonus cuz we needed to clear the string anyhow
                hint = String::new();
            }
        }
        
        // return constructed struct
        Challenge2 {
            field_size: riddle.len() as u16,
            hints,
            riddle
        }
    }

    /// Generates vector of challenge 2's binary data
    pub fn gen_field(&self) -> Vec<u8> {
        self.riddle.as_bytes().to_vec()
    }

    pub fn get_hints(&self) -> &Vec<(String, u16)> {
        &self.hints
    }

    pub fn update_hint_offsets(&mut self, offsets: Vec<u16>) {
        for i in 0..self.hints.len() {
            self.hints[i].1 = offsets[i];
        }
    }

    pub fn size(&self) -> u16 {
        self.field_size
    }

    pub fn display_info(&self, memspace_offset: u16) {
        println!("Challenge 2 {{");
        println!("    challenge flag  : understanding");
        println!("    memspace offset : {:#06x},", memspace_offset + self.field_size);
        println!("    hint offsets (local) {{ ");
        for hint in self.hints.iter().enumerate() {
            print!("        \"{}...\" : {:#06x}", &hint.1.0[..11], hint.1.1);
            if hint.0 == self.hints.len() - 1 {
                println!("\n    }}");
            } else {
                print!("\n");
            }
        }
        println!("}}");
    }
}

impl Challenge3 {
    pub fn new() -> Challenge3 {
        let data: String = String::from("Good things come in threes...");
        Challenge3 {
            field_size : data.len() as u16,
            data
        }
    }

    pub fn gen_field(&self) -> Vec<u8> {
        self.data.as_bytes().to_vec()
    }

    pub fn size(&self) -> u16 {
        self.data.len() as u16
    }

    pub fn display_info(&self, memspace_offset: u16) {
        println!("Challenge 3 {{");
        println!("    message : \"{}\"", self.data);
        println!("}}");
    }
}

impl Challenge4 {
    pub fn new() -> Challenge4 {
        let data: Vec<u8> = Vec::from(include_bytes!("../../files/steg/NGC6543-stegged.png"));
        Challenge4 {
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
