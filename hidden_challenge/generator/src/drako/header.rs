use std::fmt;

pub enum FieldType {
    Text(String),
    Size(u16)
}

impl fmt::Display for FieldType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            FieldType::Text(text) => write!(f, "{}", text),
            FieldType::Size(size) => write!(f, "{}", size)
        }
    }
}


pub struct Field {
    name: String,
    data: FieldType
}

impl Field {
    pub fn new(name: &str, data: FieldType) -> Field {
        Field {
            name: String::from(name),
            data
        }
    }

    pub fn get_name(&self) -> &String {
        &self.name
    }

    pub fn get_data(&self) -> &FieldType {
        &self.data
    }

    pub fn to_string(&self) -> String {
        format!("{}={}", self.name, self.data)
    }
}


pub fn generate(fields: &Vec<Field>) -> String  {
    // create header string; starts with ASCII "start of heading" byte
    let mut header: String = String::from("\x01");

    // add each field to the header separated by ASCII "record separator"
    for field in fields.iter() {
        header += field.to_string().as_str();
        header.push('\x1E');
    }
    // append ASCII "start of text"
    header.push('\x02');
    return header;
}

pub fn calc_len(header: &Vec<Field>) -> usize {
    // create len variable, starting at 1 to account for STX byte at start of header
    let mut l: usize = 1;

    // iterate over each field, adding it's length to the len counter
    // as well as including the ascii record separator
    for field in header.iter() {
        l += field.to_string().len() + 1;
    }

    // count the EOT byte at the end
    l += 1;

    // return the length
    l
}
