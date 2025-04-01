pub enum CipherError {
    InvalidTableLength,
    InvalidKey,
}
impl std::fmt::Display for CipherError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::InvalidTableLength => write!(f, "table's length does not match the provided data's."),
            Self::InvalidKey => write!(f, "char in key is not found in provided table.")
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
    pub fn decrypt(data: &[u8], table: &[u8], n: u8) -> Result<Vec<u8>, CipherError> {
        let mut plaintext: Vec<u8> = data.to_vec();
        for byte in plaintext.iter_mut() {
            decrypt_byte(byte, table, n);
        }
        Ok(plaintext)
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

    pub fn encrypt(data: &[u8], table: &[u8], key: &[u8]) -> Result<Vec<u8>, CipherError> {
        // make sure key is valid
        if !is_valid_key(key, table) {
            return Err(CipherError::InvalidKey);
        }

        // encrypt data
        let mut ciphertext: Vec<u8> = Vec::new();
        for i in 0..data.len() {
            let index: usize = table.iter().position(|b| *b == data[i]).unwrap();
            let shift: usize = table.iter().position(|b| *b == key[i%key.len()]).unwrap() + 1usize;
            ciphertext.push(table[(index + shift) % table.len()]);
        }

        // return ciphertext vector
        Ok(ciphertext)
    }

    pub fn decrypt(data: &[u8], table: &[u8], key: &[u8]) -> Result<Vec<u8>, CipherError> {
        // make sure key is valid
        if !is_valid_key(key, table) {
            return Err(CipherError::InvalidKey);
        }

        // decrypt data
        let mut plaintext: Vec<u8> = Vec::new();
        for i in 0..data.len() {
            let index: usize = table.iter().position(|b| *b == data[i]).unwrap();
            let shift: usize = table.iter().position(|b| *b == key[i%key.len()]).unwrap() + 1usize;

            if index >= shift {
                plaintext.push(table[(index - shift) % table.len()]);
            } else {
                plaintext.push(table[table.len() - (shift - index)]);
            }
        }

        // return plaintext vector
        Ok(plaintext)
    }

    /// Returns true if all chars in `key` are found in `table`
    fn is_valid_key(key: &[u8], table: &[u8]) -> bool {
        for b in key.iter() {
            if !table.contains(b) { return false; }
        }
        return true;
    }
}

pub mod xor {
    pub fn encrypt(data: &[u8], key: u128) -> Vec<u8> {
        let mut ciphertext: Vec<u8> = Vec::new();
        for i in 0..data.len() {
            ciphertext.push(data[i] ^ ((key>>((8*i)%128)) as u8));
        }
        ciphertext
    }

    pub fn decrypt(data: &[u8], key: u128) -> Vec<u8> {
        let mut plaintext: Vec<u8> = Vec::new();
        for i in 0..data.len() {
            plaintext.push(data[i] ^ ((key>>((8*i)%128)) as u8));
        }
        plaintext 
    }
}
