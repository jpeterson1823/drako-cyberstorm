pub struct FlagOffset {
    local: u16,
    memspace: u16
}

impl FlagOffset {
    pub fn new(local: u16, memspace: u16) -> Self {
        FlagOffset{ local, memspace }
    }

    pub fn local(&self) -> u16 { self.local }
    pub fn memspace(&self) -> u16 { self.memspace }
}




pub struct Subflag {
    value: String,
    offset: FlagOffset
}

impl Subflag {
    pub fn new(value: &str, offset: FlagOffset) -> Self {
        Subflag {
            value: String::from(value),
            offset
        }
    }

    pub fn conflicts_with(&self, other: &Subflag, min_sep: u16) -> bool {
        // check if other's offset lies within self
        if other.offset.local >= self.offset.local && other.offset.local <= self.local_tail() { false }
        // check if other's offset results in overwrite of self
        else if other.local_tail() >= self.offset.local && other.local_tail() <= self.local_tail() { false }
        // check if subflags are too close
        else if other.local_tail().abs_diff(self.offset.local) < min_sep || self.local_tail().abs_diff(self.offset.local) < min_sep { false }
        // all checks have passed; no conflict found
        else { true }
    }

    pub fn value(&self) -> &str { self.value.as_str() }
    pub fn offset(&self) -> &FlagOffset { &self.offset }
    pub fn local_tail(&self) -> u16 { self.offset.local + self.value.len() as u16 }
    pub fn memspace_tail(&self) -> u16 { self.offset.memspace + self.value.len() as u16 }
}
impl std::fmt::Display for Subflag {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut s = String::new();
        s += format!("{} : {{\n", self.value).as_str();
        s += format!("    local    : {:#06x}\n", self.offset.local).as_str();
        s += format!("    memspace : {:#06x}\n", self.offset.memspace).as_str();
        s += "}";

        write!(f, "{}", s)
    }
}
