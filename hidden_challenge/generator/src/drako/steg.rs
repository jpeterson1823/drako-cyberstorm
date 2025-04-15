pub fn store(wrap: &[u8], data: &[u8], interval: u8, offset: u8) -> Vec<u8> {
    let mut s: Vec<u8> = wrap.to_vec();
    let mut i: usize = offset as usize;

    for j in 0..data.len() {
        let byte: u8 = data[j];
        for bit in (0u8..8u8).rev() {
            s[i] &= 0xfe;
            s[i] |= (byte&(0x01<<bit))>>bit;
            i += interval as usize;
        }
    }

    s
}

pub fn retrieve(wrap: &[u8], interval: u8, offset: u16, n: u8) -> Vec<u8> {
    let mut s: Vec<u8> = Vec::new();
    let mut i: usize = offset as usize;

    for _ in 0..n {
        let mut byte: u8 = 0u8;
        for bit in (0u8..8u8).rev() {
            byte |= (wrap[i]&0x01)<<bit;
            i += interval as usize;
        }
        s.push(byte);
    }

    s
}
