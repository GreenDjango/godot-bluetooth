#![allow(dead_code)]
#![allow(unused_imports)]

use btleplug::api::{
    bleuuid::uuid_from_u16, Central, CentralEvent, Manager as _, Peripheral as _, ScanFilter,
    WriteType,
};
use btleplug::platform::{Adapter, Manager, Peripheral};
use futures::stream::StreamExt;
use rand::{thread_rng, Rng};
use std::error::Error;
use std::future::{ready, IntoFuture, Ready};
use std::thread;
use std::time::Duration;
use futures::executor;
use uuid::Uuid;
use tokio;

use crate::bluetooth::{self, DeviceInfo};

#[derive(Clone)]
pub struct BluetoothModule {
}

impl BluetoothModule {
    pub fn new() -> Result<Self, Box<dyn Error>> {
        Ok(Self {})
    }

    pub fn get_manager() -> Result<Manager, Box<dyn Error>> {
        let manager = executor::block_on(Manager::new()).unwrap();
        Ok(manager)
    }

    pub fn get_adapter() -> Result<Adapter, Box<dyn Error>> {
        let manager = executor::block_on(Manager::new()).unwrap();
        let adapters = executor::block_on(manager.adapters())?;
        let central = adapters.into_iter().nth(0).unwrap();
        Ok(central)
    }

    fn start_discovery(central: &Adapter) -> Result<(), Box<dyn Error>> {
        executor::block_on(central.start_scan(ScanFilter::default()))?;
        Ok(())
    }

    fn stop_discovery(central: &Adapter) -> Result<(), Box<dyn Error>> {
        executor::block_on(central.stop_scan())?;
        Ok(())
    }

    fn devices_to_vec(central: &Adapter) -> Result<Vec<bluetooth::DeviceInfo>, ()> {
        let mut devs: Vec<DeviceInfo> = Vec::new();
        let peripherals = executor::block_on(central.peripherals()).unwrap();
        for peripheral in peripherals {
            let properties = executor::block_on(peripheral.properties()).unwrap().unwrap();
            devs.push(DeviceInfo::new(
                properties.address.to_string(),
                properties
                    .address_type
                    .unwrap_or(btleplug::api::AddressType::default()),
                properties.local_name.unwrap_or(String::from("")),
                properties.rssi.unwrap_or(0),
                properties
                    .services
                    .iter()
                    .map(|s| Uuid::to_string(s))
                    .collect(),
            ));
        }
        Ok(devs)
    }
}

// #[async_trait::async_trait]
impl bluetooth::Bluetooth for BluetoothModule {
    fn scan_devices(scan_duration: u8) -> Result<Vec<DeviceInfo>, Box<dyn Error>> {
        println!("get central");
        let central = BluetoothModule::get_adapter()?;
        println!("start disco");
        BluetoothModule::start_discovery(&central)?;
        // executor::block_on(tokio::time::sleep(Duration::from_secs(scan_duration as u64)));
        println!("device vec");
        let devs = BluetoothModule::devices_to_vec(&central).unwrap();
        println!("stop disco");
        BluetoothModule::stop_discovery(&central)?;
        Ok(devs)
    }
}