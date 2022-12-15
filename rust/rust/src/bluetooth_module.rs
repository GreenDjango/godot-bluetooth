#![allow(dead_code)]
#![allow(unused_imports)]

use async_trait::async_trait;

use btleplug::api::{bleuuid::uuid_from_u16, Central, Manager as _, Peripheral as _, ScanFilter, WriteType};
use btleplug::platform::{Adapter, Manager, Peripheral};
use rand::{Rng, thread_rng};
use std::error::Error;
use std::thread;
use std::time::Duration;
use tokio::time;
use uuid::Uuid;

use crate::bluetooth::{self, DeviceInfo};

pub struct BluetoothModule {
    manager: Manager,
    central: Adapter,
}

impl BluetoothModule {
    pub async fn new() -> Result<Self, Box<dyn Error>> {
        let manager = Manager::new().await.unwrap();
        let adapters = manager.adapters().await?;
        let central = adapters.into_iter().nth(0).unwrap();       
        Ok(Self {
            manager: manager,
            central: central,
        })
    }

    async fn devices_to_vec(&self) -> Result<Vec<bluetooth::DeviceInfo>, ()> {
        let mut devs: Vec<DeviceInfo> = Vec::new();
        let peripherals = self.central.peripherals().await.unwrap();
        for peripheral in peripherals {
            let properties = peripheral.properties().await.unwrap().unwrap();
            devs.push(DeviceInfo::new(
                properties.address.to_string(),
                properties.address_type.unwrap_or(btleplug::api::AddressType::default()),
                properties.local_name.unwrap_or(String::from("")),
            ));
        }
        Ok(devs)
    }
}


// #[tokio::main]
#[async_trait]
impl bluetooth::Bluetooth for BluetoothModule {
    async fn scan_devices(&self, scan_duration: u8) -> Result<Vec<DeviceInfo>, Box<dyn Error>> {
        self.central.start_scan(ScanFilter::default()).await?;
        time::sleep(Duration::from_secs(scan_duration as u64)).await;
        let devs = self.devices_to_vec().await.unwrap();
        self.central.stop_scan().await?;
        Ok(devs)
    }
}