mod c_header;
mod drako;

use drako::{steg, HiddenChallenge};

use std::fs::{File, create_dir};
use std::io::Write;
use std::env;

/// Writes byte data to specified path
fn save_bin(path: &str, data: &[u8]) {
    // open file
    let mut f: File = match File::create(path) {
        Ok(f) => f,
        Err(e) => panic!("[IO ERROR] Failed to open file {}\nError:{}", path, e)
    };

    //write data
    match f.write(data) {
        Ok(_) => {},
        Err(e) => panic!("[IO ERROR] Failed to open file {}\nError:{}", path, e)
    };
}

fn generator() {
    // create hidden challenge object
    let mut hc: HiddenChallenge = HiddenChallenge::new();
    println!("hidden challenge object created.");

    // generate challenge datablock
    let datablock: Vec<u8> = hc.generate_datablock();
    println!("datablock generated.");

    // generate eeprom memory space
    let memspace: Vec<u8> = hc.generate_eeprom_memspace(&datablock);
    println!("memspace generated.");

    // generate c header file
    c_header::generate(&hc, &memspace);

    // create binary dir
    match create_dir("./generated/bin") {
        Ok(_) => {},
        Err(e) => {
            if e.kind() == std::io::ErrorKind::AlreadyExists {}
            else {
                panic!("[IO ERROR] An error occured when trying to create ./generated/bin\nError Output: {}", e)
            }
        }
    };

    // save datablock and memspace to file
    save_bin("./generated/bin/datablock.bin", &datablock);
    println!("Created ./generated/bin/memspace.bin");
    save_bin("./generated/bin/memspace.bin", &memspace);
    println!("Created ./generated/bin/datablock.bin");

    // display important data
    hc.display_info();

    //let o = 2312 + 72 + hc.get_c1().size() + hc.get_c2().size() + hc.get_c3().size();
    //let extracted = steg::retrieve(&memspace, 47, o + 250, 7);
    //println!("correct offset={:#06x}", o  + 250);
    //for byte in extracted.iter() {
    //    println!("byte={:#04x}", byte);
    //}
}

fn info() {
    // info vars
    let mut cb_offset: usize;

    // load generated memspace
    let memspace: Vec<u8> = include_bytes!("../generated/bin/memspace.bin").to_vec();

    // find header within memspace and get its offset
    let header: &[u8] = "\x01name=".as_bytes();
    let mut window: &[u8];
    let mut header_found: bool = false;
    for i in 0..(memspace.len() - header.len()) {
        if header_found { break }
        window = &memspace[i..i+header.len()];
        for j in 0..header.len() {
            if window == header {
                cb_offset = i;
                header_found = true;
                break;
            }
        }
    }
}

fn main() {
    // get cli args
    //let args: Vec<String> = env::args().collect();
    
    generator();
    //info();
}
