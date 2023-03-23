#![allow(dead_code)]
#![allow(unused_imports)]

use async_trait::async_trait;

// use btleplug::Result;
use btleplug::api::{bleuuid::uuid_from_u16, Central, CentralEvent, Manager as _, Peripheral as _, ScanFilter, WriteType};
use btleplug::platform::{Adapter, Manager, Peripheral};
use rand::{Rng, thread_rng};
use std::error::Error;
use std::thread;
use std::time::Duration;
use tokio::time;
use uuid::Uuid;
use futures::stream::StreamExt;

use crate::bluetooth::{self, DeviceInfo};

pub struct BluetoothModule {
    discovering_enable: bool,
    event_enable: bool,
    manager: Manager,
    central: Adapter,
}

impl BluetoothModule {
    pub async fn new() -> Result<Self, Box<dyn Error>> {
        let manager = Manager::new().await.unwrap();
        let adapters = manager.adapters().await?;
        let central = adapters.into_iter().nth(0).unwrap();       
        Ok(Self {
            discovering_enable: false,
            event_enable: false,
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
                properties.rssi.unwrap_or(0),
                properties.services.iter().map(|s| Uuid::to_string(s)).collect()
            ));
        }
        Ok(devs)
    }
}


#[async_trait]
impl bluetooth::Bluetooth for BluetoothModule {
    async fn start_discovery(&self) -> Result<(), Box<dyn Error>> {
        self.central.start_scan(ScanFilter::default()).await?;
        Ok(())
    }

    async fn stop_discovery(&self) -> Result<(), Box<dyn Error>>{
        self.central.stop_scan().await?;
        Ok(())
    }

    async fn list_devices(&self) -> Result<Vec<DeviceInfo>, Box<dyn Error>> {
        let devs = self.devices_to_vec().await.unwrap();
        Ok(devs)
    }

    async fn scan_devices(&self, scan_duration: u8) -> Result<Vec<DeviceInfo>, Box<dyn Error>> {
        self.start_discovery().await?;
        time::sleep(Duration::from_secs(scan_duration as u64)).await;
        let devs = self.list_devices().await?;
        self.stop_discovery().await?;
        Ok(devs)
    }


	fn enable_event(&mut self) -> Result<(), Box<dyn Error>> {
        self.event_enable = true;
        Ok(())
    }

	fn disable_event(&mut self) -> Result<(), Box<dyn Error>> {
        self.event_enable = false;
        Ok(())
    }

    async fn event_thread(&self) -> Result<(), Box<dyn Error>> {
        let mut events = self.central.events().await?;
        while let Some(event) = events.next().await {
            match event {
                CentralEvent::DeviceDiscovered(id) => {
                    println!("DeviceDiscovered: {:?}", id);
                }
                CentralEvent::DeviceConnected(id) => {
                    println!("DeviceConnected: {:?}", id);
                }
                CentralEvent::DeviceDisconnected(id) => {
                    println!("DeviceDisconnected: {:?}", id);
                }
                CentralEvent::ManufacturerDataAdvertisement {
                    id,
                    manufacturer_data,
                } => {
                    println!(
                        "ManufacturerDataAdvertisement: {:?}, {:?}",
                        id, manufacturer_data
                    );
                }
                CentralEvent::ServiceDataAdvertisement { id, service_data } => {
                    println!("ServiceDataAdvertisement: {:?}, {:?}", id, service_data);
                }
                CentralEvent::ServicesAdvertisement { id, services } => {
                    let services: Vec<String> =
                        services.into_iter().map(|s| s.to_string()).collect();
                    println!("ServicesAdvertisement: {:?}, {:?}", id, services);
                }
                _ => {}
            }
        }
        Ok(())
    }
}