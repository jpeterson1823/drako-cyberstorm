pub struct ChInfo {
    field_size : u16,
    hints      : Vec<(String, u16)>,
    riddle     : String
}

impl ChInfo {
    pub fn new() -> ChInfo {
        // load riddle into string
        let riddle: String = String::from(include_str!("../../../files/riddle.txt"));

        // load hints into string vector
        let mut hints: Vec<(String, u16)> = Vec::new();
        let mut hint: String = String::new();
        for line in include_str!("../../../files/riddle-hints.txt").lines() {
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
        ChInfo {
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
