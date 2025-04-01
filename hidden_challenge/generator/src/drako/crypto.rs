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
        let mut ciphertext: Vec<u8> = Vec::new();
        for i in 0..data.len() {
            let index: usize = table.iter().position(|b| *b == data[i]).unwrap();
            let shift: usize = n as usize;
            ciphertext.push(table[(index + shift) % table.len()]);
        }

        Ok(ciphertext)
    }

    /// Caesar cipher decryption method
    pub fn decrypt(data: &[u8], table: &[u8], n: u8) -> Result<Vec<u8>, CipherError> {
        let mut plaintext: Vec<u8> = Vec::new();
        for i in 0..data.len() {
            let index: usize = table.iter().position(|b| *b == data[i]).unwrap();
            let shift: usize = n as usize;
            if index >= shift as usize {
                plaintext.push(table[index - shift]);
            } else {
                plaintext.push(table[(table.len() - (shift - index)) % table.len()]);
            }
        }
        Ok(plaintext)
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
