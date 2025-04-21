use crate::drako::crypto::*;

// caesar this message with c-flags words being rot-n ciphered
const C3_PLAINTEXT  : &[u8] = b"The two requirements for expanding one's knowledge: discovery and understanding.\nUpon my plastic vessel lies a false label, one which was left by my creator.\nFind this incorrect label, then its true value. Decipher these notes, and your search shall be complete!\n\n\"The key lies in plain sight, hidden by the Grand Mage Stegg!\nA hint to those who seek: Incorrect is to Offset, as Correct is to Interval.\"\n\nHere lies the penultimate challenge--no more than a covert messenger.\nHeed its words, cryptseeker. The final challenge awiats.";
pub const C3_CAESAR_LUT : &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz 0123456789,.~'?!*";

pub struct ChInfo {
    field_size : u16,
    data       : Vec<u8>,
    n_caesar   : u8
}

impl ChInfo {
    pub fn new() -> ChInfo {
        // encrypt message
        let n_caesar: u8 = 31;
        let data: Vec<u8> = match caesar::encrypt(C3_PLAINTEXT, C3_CAESAR_LUT, n_caesar) {
            Ok(ciphertext) => ciphertext,
            Err(err) => { println!("ChInfo constructor encountered an error: {}", err); C3_PLAINTEXT.to_vec() }
        };

        // construct ChInfo
        ChInfo {
            field_size: C3_PLAINTEXT.len() as u16,
            data,
            n_caesar
        }
    }

    pub fn gen_field(&self) -> Vec<u8> {
        self.data.to_vec()
    }

    pub fn size(&self) -> u16 {
        self.data.len() as u16
    }

    pub fn display_info(&self, memspace_offset: u16) {
        let ciphertext: Vec<u8> = match caesar::encrypt(C3_PLAINTEXT, C3_CAESAR_LUT, self.n_caesar) {
            Ok(ciphertext) => ciphertext,
            Err(err) => {
                println!("ChInfo::display_info failed to encrypt C3_PLAINTEXT... uh oh: {}", err);
                C3_PLAINTEXT.to_vec()
            }
        };

        println!("Challenge 3 {{");
        println!("    memspace offset: {:#06x}", memspace_offset);
        print!  ("    message {{\n        ");
        for c in std::str::from_utf8(C3_PLAINTEXT).unwrap().chars() {
            if c == '\n' { print!("\\n{}        ", c) }
            else { print!("{}", c) }
        }
        println!("\n    }}");
        print!  ("    ciphertext {{\n        ");
        for c in std::str::from_utf8(ciphertext.as_slice()).unwrap().chars().enumerate() {
            if c.0 % 29 == 0 && c.0 != 0 { print!("\n        ") }
            if c.1 == '\n' { print!("\\n") }
            else { print!("{}", c.1) }
        }
        println!("\n    }}");
        println!("}}");
    }
}
