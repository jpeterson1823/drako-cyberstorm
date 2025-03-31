use crate::drako::crypto::*;

// caesar this message with c-flags words being rot-n ciphered
const C3_PLAINTEXT  : &[u8] = b"Before understanding comes discovery.\nDiscovery unveils the puzzle. Understanding places the pieces.\nDiscovery is to -XOR- as Understanding is to *Vigenere*.\nThat which is *scrambled* then -obfuscated- must be -deciphered- then *restored*...\nHere lies the penultimate challenge--no more than a covert messegner.\nHeed its words, cryptseeker. The final challenge awiats.";
const C3_CAESAR_LUT : &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz 0123456789,.~'?!*";

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
