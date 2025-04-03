use std::fs::{File, create_dir};
use std::io::Write;
use super::drako::HiddenChallenge;
use super::drako::challenge;

/// Path where generated header will be written
pub const HEADER_PATH: &str = "./generated/hidden_challenge.h";

/// Generate C Header file containing all relevant info
pub fn generate(hc: &HiddenChallenge, memspace: &Vec<u8>) {
    print!("Generating and writing C header... ");

    // generate header content
    let mut content: String = String::new(); 
    content += generate_includes_and_guard().as_str();
    content += generate_constants(hc, memspace).as_str();
    content += generate_memspace_array(memspace).as_str();

    // create output directory if it isnt there already.
    match create_dir("./generated") {
        Ok(_) => {},
        Err(e) => {
            if e.kind() != std::io::ErrorKind::AlreadyExists {
                panic!("[PANIC] An error has occured during the attempted creation of ./generated/\nError: {}", e)
            }
        }
    };

    // open file at HEADER_PATH
    let mut header_file = match File::create(HEADER_PATH) {
        Ok(f) => f,
        Err(e) => panic!("[IO ERROR] Failed to create/open {}\nError: {}", HEADER_PATH, e)
    };

    // write header contents to file
    match header_file.write(content.as_bytes()) {
        Ok(_) => {},
        Err(e) => panic!("[IO ERROR] Failed to write to {}\nError: {}", HEADER_PATH, e)
    };

    println!("DONE");
}

/// Generates string containing include guard and required includes
fn generate_includes_and_guard() -> String {
    format!("\
        #ifndef __drako_hidden_challenge_data\n\
        #define __drako_hidden_challenge_data\n\n\
        #include <stdint.h>\n\n"
    )
}

/// Generates string containing the constants for the hidden challenge
fn generate_constants(hc: &HiddenChallenge, memspace: &Vec<u8>) -> String {
    let mut content: String = String::new();
    content += format!("#define HC_DATABLOCK_SIZE {}\n", hc.get_datablock_size()).as_str();
    content += format!("#define HC_MEMSPACE_SIZE {}\n", memspace.len()).as_str();

    content += format!("#define HC_C1_MEMSPACE_OFFSET {}\n", hc.calc_c1_memspace_offset()).as_str();
    content += format!("#define HC_C1_MEMSPACE_TAIL {}\n", hc.calc_c1_memspace_offset() + hc.get_c1().size()-1).as_str();

    content += format!("#define HC_C2_MEMSPACE_OFFSET {}\n", hc.calc_c2_memspace_offset()).as_str();
    content += format!("#define HC_C2_MEMSPACE_TAIL {}\n", hc.calc_c2_memspace_offset() + hc.get_c2().size()-1).as_str();

    content += format!("#define HC_C3_MEMSPACE_OFFSET {}\n", hc.calc_c3_memspace_offset()).as_str();
    content += format!("#define HC_C3_MEMSPACE_TAIL {}\n", hc.calc_c3_memspace_offset() + hc.get_c3().size()-1).as_str();

    content += format!("#define HC_C4_MEMSPACE_OFFSET {}\n", hc.calc_c4_memspace_offset()).as_str();
    content += format!("#define HC_C4_MEMSPACE_TAIL {}\n", hc.calc_c4_memspace_offset() + hc.get_c4().size()-1).as_str();

    content += format!("#define HC_C4_STEG_OFFSET {}\n", challenge::ch4::STEG_OFFSET).as_str();
    content += format!("#define HC_C4_STEG_INTERVAL {}\n", challenge::ch4::STEG_INTERVAL).as_str();
    content += format!("#define HC_C4_STEG_N_BYTES {}\n", challenge::ch4::STEG_N_BYTES).as_str();
    content += "\n";
    content
}

/// Generates string containing the datablock constant static array
fn generate_memspace_array(memspace: &Vec<u8>) -> String {
    // create content string initially containing the definition of the array
    let mut content: String = String::new();
    content += "const static uint8_t HIDDEN_CHALLENGE_MEMSPACE[] = {\n    ";

    // write datablock bytes as hex values to array
    for byte in memspace.iter().enumerate() {
        // add new line after 16 bytes written on that line
        if byte.0 > 1 && byte.0 % 15 == 0 {
            content += format!("{:#04x},\n    ", byte.1).as_str();
        }

        // if last element, close out the array properly
        else if byte.0 == memspace.len()-1 {
            content += format!("{:#04x}\n}};\n\n#endif", byte.1).as_str();
        }

        // otherwise it's a normal byte
        else {
            content += format!("{:#04x}, ", byte.1).as_str();
        }
    }

    // return content string
    content
}
