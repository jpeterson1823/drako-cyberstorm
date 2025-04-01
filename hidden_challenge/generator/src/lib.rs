mod drako;

#[cfg(test)]
mod tests {
    use super::*;
    use rand::prelude::*;

    #[test]
    fn xor_test() {
        let mut rng = rand::rng();
        let key: u128 = rng.random::<u128>();
        let rng_vec: Vec<u8> = (0..100).map(|_| rng.random::<u8>()).collect();

        let ciphertext: Vec<u8> = drako::crypto::xor::encrypt(&rng_vec, key);
        let plaintext: Vec<u8> = drako::crypto::xor::decrypt(&ciphertext, key);
        assert!(byte_vecs_eq(&plaintext, &rng_vec));
    }

    #[test]
    fn vigenere_test() {
        let table = drako::challenge::ch3::C3_CAESAR_LUT;
        let key: &[u8] = b"Vigenere Key!";
        let test_vec: Vec<u8> = b"This is the plaintext used for the test. Hopefully, this'll work out... please!".to_vec();

        let ciphertext: Vec<u8> = match drako::crypto::vigenere::encrypt(&test_vec, table, key) {
            Ok(ciphertext) => ciphertext,
            Err(_) => { assert!(false); Vec::new() }
        };
        let plaintext: Vec<u8> = match drako::crypto::vigenere::decrypt(&ciphertext, table, key) {
            Ok(plaintext) => plaintext,
            Err(_) => { assert!(false); Vec::new() }
        };

        println!("Original:");
        for b in test_vec.iter() { print!("{}", *b as char) }
        println!("\nEncrypted then Decrypted:");
        for b in plaintext.iter() { print!("{}", *b as char) }
        println!("");

        assert!(byte_vecs_eq(&plaintext, &test_vec));
    }

    #[test]
    fn caesar_test() {
        let table = drako::challenge::ch3::C3_CAESAR_LUT;
        let n: u8 = 31;
        let test_vec: Vec<u8> = b"This is the plaintext used for the test. Hopefully, this'll work out... please!".to_vec();

        let ciphertext: Vec<u8> = match drako::crypto::caesar::encrypt(&test_vec, table, n) {
            Ok(ciphertext) => ciphertext,
            Err(_) => { assert!(false); Vec::new() }
        };
        let plaintext: Vec<u8> = match drako::crypto::caesar::decrypt(&ciphertext, table, n) {
            Ok(plaintext) => plaintext,
            Err(_) => { assert!(false); Vec::new() }
        };
        println!("Original:");
        for b in test_vec.iter() { print!("{}", *b as char) }
        println!("\nEncrypted then Decrypted:");
        for b in plaintext.iter() { print!("{}", *b as char) }
        println!("");

        assert!(byte_vecs_eq(&plaintext, &test_vec));
    }

    fn byte_vecs_eq(a: &Vec<u8>, b: &Vec<u8>) -> bool {
        if a.len() != b.len() { return false; }
        for i in 0..a.len() {
            if a[i] != b[i] { return false; }
        }
        true
    }
}
