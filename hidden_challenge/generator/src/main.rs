mod hidden_challenge;
use hidden_challenge::*;
use rand;
use rand::prelude::*;

use std::fs::File;
use std::io::prelude::*;

fn main() {
    // generate challenges
    print!("Generating challenges...");
    let c1: Challenge1 = Challenge1::new();
    let c2: Challenge2 = Challenge2::new();
    let c3: Challenge3 = Challenge3::new();
    let c4: Challenge4 = Challenge4::new();
    println!("DONE");

    // generate challenge header
    print!("Generating header... ");
    let header: String = format!(
        "\x01name=Drako Hidden Challenge\x1Ec1len={}\x1Ec2len={}\x1Ec3len={}\x1Ec4len={}\x02",
        c1.size(),
        c2.size(),
        c3.size(),
        c4.size()
    );
    println!("DONE");

    // generate fill data
    let nbytes: usize = (constants::EEPROM_SIZE as usize) - header.len() - c1.size() - c2.size() - c3.size() - c4.size();
    let filler = (0..nbytes).map(|_| rand::rng().random::<u8>());

    // generate datablock
    let mut datablock: Vec<u8> = Vec::new();
    print!("Generating datablock...");
    datablock.extend(header.chars().map(|c| c as u8));
    datablock.extend(c1.gen_field().iter());
    datablock.extend(c2.gen_field().iter());
    datablock.extend(c3.gen_field().iter());
    datablock.extend(c4.gen_field().iter());
    datablock.extend(filler);
    println!("DONE");

    // generate and write c header
    print!("Generating and writing C header... ");
    generate_c_header(&datablock);
    println!("DONE");

    // write datablock to binary file
    print!("Writing datablock to file (datablock.bin)...");
    let mut file = File::create("datablock.bin").expect("Failed to create datablock.bin");
    file.write(datablock.as_slice()).expect("Failed to write datablock to datablock.bin");
    println!("DONE");
}
