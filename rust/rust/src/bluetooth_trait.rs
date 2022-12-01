#![allow(dead_code)]

use std::error::Error;

pub struct DeviceInfo {
	pub address: String,
	pub address_type: String,
	pub name: String,
	pub alias: String,
	pub icon: String,
	pub paired: bool,
	pub trusted: bool,
	pub blocked: bool,
	pub legacy_pairing: bool,
	pub connected: bool
}

impl DeviceInfo {
    pub fn new(address: String, address_type: String, name: String) -> Self {
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
            connected: false
        }
    }
}

use async_trait::async_trait;

#[async_trait]
pub trait Bluetooth {
	async fn init(&self) -> Result<(), Box<dyn Error>>;
    async fn scan_devices(&self) -> Result<Vec<DeviceInfo>, Box<dyn Error>>;

	// fn remove_device(&self, device_address: String) ;
	// fn start_discovery(&self) ;
	// fn stop_discovery(&self) ;

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