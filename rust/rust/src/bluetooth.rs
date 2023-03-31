#![allow(dead_code)]

use std::{error::Error};
use btleplug::api::AddressType;
use gdnative::prelude::Dictionary;
// use async_trait;

#[derive(Debug)]
pub struct DeviceInfo {
	pub address: String,
	pub address_type: AddressType,
	pub name: String,
	pub alias: String,
	pub icon: String,
	pub paired: bool,
	pub trusted: bool,
	pub blocked: bool,
	pub legacy_pairing: bool,
	pub connected: bool,
	pub rssi: i16,
	pub services: Vec<String>
}

impl From<&DeviceInfo> for Dictionary {
	fn from(dev: &DeviceInfo) -> Dictionary {
        let dict = Dictionary::new_thread_local();
		dict.insert("addres", "myaddress");
		// dict.into
		Dictionary::from(dict.duplicate())
    }
}

impl DeviceInfo {
    pub fn new(address: String, address_type: AddressType, name: String, rssi: i16, services: Vec<String>) -> Self {
        Self {
            address: address,
            address_type: address_type,
            name: name,
            alias: String::from(""),
            icon: String::from(""),
            paired: false,
            trusted: false,
            blocked: false,
            legacy_pairing: false,
            connected: false,
			rssi: rssi,
			services: services
        }
    }
}

// #[async_trait::async_trait]
pub trait Bluetooth {
	fn scan_devices(scan_duration: u8) -> Result<Vec<DeviceInfo>, Box<dyn Error>>;
}