use rand::prelude::*;

pub const C1_WRAPPER        : &str  = "DRAKO";

// Challenge Structs
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

    pub fn size(&self) -> usize {
        self.field_size
    }
}

impl Challenge2 {
    pub fn new() -> Challenge2 {
        // load riddle into string
        let riddle: String = String::from(include_str!("../../files/riddle.txt"));

        // load hints into string vector
        let mut hints: Vec<String> = Vec::new();
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
                hints.push(hint);
                // make sure to create new string as ownership has changed
                // bonus cuz we needed to clear the string anyhow
                hint = String::new();
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
            data: Vec::from(include_bytes!("../../files/steg/NGC6543-stegged.png"))
        }
    }

    pub fn gen_field(&self) -> &Vec<u8> {
        &self.data
    }

    pub fn size(&self) -> usize {
        self.data.len()
    }
}
