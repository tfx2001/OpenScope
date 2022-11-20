use std::time::Duration;
use probe_rs::{Permissions, config};
use crate::ffi::DebugProbeInfo;

#[cxx::bridge(namespace = "probe_rust")]
mod ffi {
    #[derive(Debug)]
    pub struct DebugProbeInfo {
        pub name: String,
        pub serial: String,
    }

    extern "Rust" {
        type Probe;

        fn get_chips() -> Vec<String>;
        fn list_probes() -> Vec<DebugProbeInfo>;
        fn new_probe() -> Box<Probe>;
        fn open(self: &mut Probe, index: usize, target: &str) -> Result<()>;
        fn close(self: &mut Probe) -> Result<()>;
    }
}

fn get_chips() -> Vec<String> {
    config::families().unwrap().iter()
        .map(|v| v.variants.iter().map(|chip| chip.name.clone()).collect())
        .collect::<Vec<Vec<String>>>().concat()
}

fn list_probes() -> Vec<DebugProbeInfo> {
    probe_rs::Probe::list_all().iter().map(|v| DebugProbeInfo {
        name: v.identifier.clone(),
        serial: v.serial_number.clone().unwrap_or(String::default()),
    }).collect()
}

struct Probe {
    session: Option<probe_rs::Session>,
}

impl Probe {
    fn open(&mut self, index: usize, target: &str) -> Result<(), String> {
        let probes: Vec<probe_rs::DebugProbeInfo> = probe_rs::Probe::list_all();
        if let Some(probe_info) = probes.get(index) {
            let probe: probe_rs::Probe = probe_info.open().map_err(|e| e.to_string())?;
            self.session = Some(probe.attach(target, Permissions::default()).map_err(|e| e.to_string())?);
            Ok(())
        } else {
            Err("Out of index".to_string())
        }
    }

    fn close(self: &mut Probe) -> Result<(), String> {
        if self.session.is_some() {
            self.session.take();
            Ok(())
        } else {
            Err("Probe is not connect.".to_string())
        }
    }
}

fn new_probe() -> Box<Probe> {
    Box::new(Probe { session: None })
}

#[cfg(test)]
mod tests {
    use crate::*;

    #[test]
    fn test_chip_family() {
        println!("{:#?}", get_chips());
    }

    #[test]
    fn test_list_probes() {
        println!("{:#?}", list_probes());
    }

    // #[test]
    // fn test_open_probe() {
    //     assert!(open_probe(usize::MAX).is_err());
    //     assert!(open_probe(0).is_ok());
    // }
}
