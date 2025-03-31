pub enum CipherError {
    InvalidTableLength
}
impl std::fmt::Display for CipherError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::InvalidTableLength => write!(f, "table's length does not match the provided data's.")
        }
    }
}


pub mod caesar {
    use super::CipherError;

    /// Caesar cipher encryption method
    pub fn encrypt(data: &[u8], table: &[u8], n: u8) -> Result<Vec<u8>, CipherError> {
        // encrypt each byte
        let mut ciphertext: Vec<u8> = data.to_vec();
        for byte in ciphertext.iter_mut() {
            encrypt_byte(byte, table, n);
        }
        Ok(ciphertext)
    }

    /// Caesar cipher decryption method
    pub fn decrypt(data: &[u8], table: &[u8], n: u8) -> Result<String, CipherError> {
        let mut plaintext: Vec<u8> = data.to_vec();
        for byte in plaintext.iter_mut() {
            decrypt_byte(byte, table, n);
        }
        Ok(String::from(std::str::from_utf8(plaintext.as_slice()).unwrap()))
    }

    /// Attempts to encrypt a provided byte.
    /// If the provided byte is not found within the provided table, no encrypting occurs.
    fn encrypt_byte(byte: &mut u8, table: &[u8], n: u8){
        for b in table.iter().enumerate() {
            if byte == b.1 { *byte = table[(b.0 + n as usize) % table.len()]; }
        }
    }

    /// Attempts to decrypt a provided byte.
    /// If the provided byte is not found within in the prodived table, no encrypting occurs.
    fn decrypt_byte(byte: &mut u8, table: &[u8], n: u8) {
        for b in table.iter().enumerate() {
            if byte == b.1 { *byte = table[b.0.abs_diff(n as usize) & table.len()]; }
        }
    }
}

pub mod vigenere {
    use super::CipherError;

}
