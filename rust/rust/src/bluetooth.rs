#![allow(dead_code)]

use std::{error::Error};
use async_trait::async_trait;
use btleplug::api::AddressType;

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

#[async_trait]
pub trait Bluetooth {
	fn start_discovery(&self) -> Result<(), Box<dyn Error>>;
	fn stop_discovery(&self) -> Result<(), Box<dyn Error>>;

    fn list_devices(&self) -> Result<Vec<DeviceInfo>, Box<dyn Error>>;
    
	fn scan_devices(&self, scan_duration: u8) -> Result<Vec<DeviceInfo>, Box<dyn Error>>;

	fn enable_event(&mut self) -> Result<(), Box<dyn Error>>;
	fn disable_event(&mut self) -> Result<(), Box<dyn Error>>;

    async fn event_thread(&self) -> Result<(), Box<dyn Error>>;

	// fn remove_device(&self, device_address: String) ;

	// fn address(&self) -> String;
	// fn address_type(&self) -> String;
	// fn set_alias(&self, alias: String) -> String;
	// fn get_alias(&self) -> String;

    // fn set_discoverable(enable: bool);
	// fn is_discoverable(&self) -> bool;

    // fn discovering(&self) ;
	// fn modalias(&self) -> String;
	// fn name(&self) -> String;
	// fn set_pairable(&self, enable: bool) ;
	// fn is_pairable(&self) ;

    // fn set_powered(&self, enable: bool) ;
	// fn is_powered(&self) -> bool;

	// fn is_dummy(&self);
}