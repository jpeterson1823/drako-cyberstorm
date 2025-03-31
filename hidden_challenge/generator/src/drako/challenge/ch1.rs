use rand::prelude::*;
use super::meta::*;

// Constants
pub const C1_WRAPPER: &str = "DRAKO";
const C1_SF1: &str = "THUBAN";
const C1_SF2: &str = "DRACONIS";
const C1_SF_MIN_SEP: u16 = 250;
const C1_SF_MIN_OFFSET: u16 = 100;
const C1_SIZE: u16 = 1000u16;

/// Challenge Information Struct
pub struct ChInfo {
    size: u16,
    memspace_offset: u16,
    subflags: Vec<Subflag>
}

impl ChInfo {
    pub fn new(memspace_offset: u16) -> Self {
        ChInfo {
            size: C1_SIZE,
            memspace_offset,
            subflags: Self::gen_subflags(memspace_offset)
        }
    }

    pub fn size(&self) -> u16 { self.size }
    pub fn memspace(&self) -> u16 { self.memspace_offset }
    pub fn subflags(&self) -> &Vec<Subflag> { &self.subflags }

    /// Generates subflags with their relevant data
    fn gen_subflags(memspace_offset: u16) -> Vec<Subflag> {
        // generate wrapped subflag strings
        let subflag_strs: (String, String) = (
            Self::sfstr(C1_SF1),
            Self::sfstr(C1_SF2)
        );

        // generate subflags vector and add both subflags
        let mut subflags: Vec<Subflag> = Vec::new();
        subflags.push(Self::place_subflag(memspace_offset, subflag_strs.0.as_str(), &subflags));
        subflags.push(Self::place_subflag(memspace_offset, subflag_strs.1.as_str(), &subflags));

        // return subflag vector
        subflags
    }

    /// Generates a random challenge offset for a subflag, making sure to not overwrite previously
    /// placed subflags.
    fn place_subflag(memspace_offset: u16, sf_value: &str, subflags: &Vec<Subflag>) -> Subflag {
        // get random object and declare offset object
        let mut rng = rand::rng();
        let rng_range = C1_SF_MIN_OFFSET..C1_SIZE;

        // if no subflags placed, return rng'd subflag
        if subflags.len() == 0 {
            let local_offset = rng.random_range(rng_range);
            return Subflag::new(sf_value, FlagOffset::new(local_offset,local_offset + memspace_offset));
        }

        // otherwise, make sure rng offset doesnt conflict with placed subflags
        let mut local_offset = rng.random_range(rng_range.clone());
        let mut subflag = Subflag::new(sf_value, FlagOffset::new(local_offset, local_offset + memspace_offset));
        loop {
            for sf in subflags.iter() {
                // if subflag has conflict, regen offset and restart loop
                if subflag.conflicts_with(sf, C1_SF_MIN_SEP) {
                    local_offset = rng.random_range(rng_range);
                    subflag = Subflag::new(sf_value, FlagOffset::new(local_offset, local_offset + memspace_offset));
                    break;
                }
            }

            // no conflicts found; subflag location is good, so break loop.
            break;
        }

        // return subflag
        subflag
    }

    /// Creates wrapped subflag string
    fn sfstr(flag: &str) -> String {
        format!("{}{}{}", C1_WRAPPER, flag, C1_WRAPPER)
    }

    /// Generate vector of challenge 1's binary data
    pub fn gen_field(&self) -> Vec<u8> {
        // generate Vec<u8> of length field_size that's filled w/rng bytes
        let mut rng = rand::rng();
        let mut bin: Vec<u8> = (0..self.size).map(|_| rng.random::<u8>()).collect();

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

    pub fn display_info(&self, memspace_offset: u16) {
        // calculate magic credential offset and password byte
        let mc_addr: u16 = self.subflags[0].value().chars().map(|c| c as u32).sum::<u32>() as u16;
        let mc_pass: u8  = self.subflags[1].value().chars().map(|c| c as u32).sum::<u32>() as u8;

        // do the displaying
        println!("Challenge 1 {{");
        println!("    challenge flag  : discovery");
        println!("    memspace offset : {:#06x}", memspace_offset);
        println!("    size            : {}", self.size);
        println!("    subflags {{");
        println!("        {} : local_offset = {:#06x}", C1_SF1, self.subflags[0].offset().local());
        println!("        {} : local_offset = {:#06x}", C1_SF2, self.subflags[1].offset().local());
        println!("    }}");
        println!("    magic credentials {{");
        println!("        {} ---> {:#06x}", C1_SF1, mc_addr);
        println!("        {} ---> {:#06x}", C1_SF2, mc_pass);
        println!("    }}");
    }
}
