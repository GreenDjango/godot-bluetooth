#![allow(dead_code)]
#![allow(unused_imports)]

use async_trait::async_trait;

// use btleplug::Result;
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
use tokio::time;
use futures::executor;
use uuid::Uuid;

use crate::bluetooth::{self, DeviceInfo};

pub struct BluetoothModule {
    discovering_enable: bool,
    event_enable: bool,
    manager: Option<Manager>,
    central: Option<Adapter>,
}

impl BluetoothModule {
    pub fn new() -> Result<Self, Box<dyn Error>> {
        Ok(Self {
            discovering_enable: false,
            event_enable: false,
            manager: None,
            central: None,
        })
    }

    pub fn init(& mut self) -> Result<(), Box<dyn Error>> {
        let manager = executor::block_on(Manager::new()).unwrap();
        let adapters = executor::block_on(manager.adapters())?;
        let central = adapters.into_iter().nth(0).unwrap();
        self.manager = Some(manager);
        self.central = Some(central);
        Ok(())
    }

    fn devices_to_vec(&self) -> Result<Vec<bluetooth::DeviceInfo>, ()> {
        let mut devs: Vec<DeviceInfo> = Vec::new();
        let peripherals = executor::block_on(self.central.as_ref().expect("Must call the init method").peripherals()).unwrap();
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

#[async_trait]
impl bluetooth::Bluetooth for BluetoothModule {
    fn start_discovery(&self) -> Result<(), Box<dyn Error>> {
        executor::block_on(self.central.as_ref().expect("Must call the init method").start_scan(ScanFilter::default()))?;
        Ok(())
    }

    fn stop_discovery(&self) -> Result<(), Box<dyn Error>> {
        executor::block_on(self.central.as_ref().expect("Must call the init method").stop_scan())?;
        Ok(())
    }

    fn list_devices(&self) -> Result<Vec<DeviceInfo>, Box<dyn Error>> {
        let devs = self.devices_to_vec().unwrap();
        Ok(devs)
    }

    fn scan_devices(&self, scan_duration: u8) -> Result<Vec<DeviceInfo>, Box<dyn Error>> {
        self.start_discovery()?;
        executor::block_on(time::sleep(Duration::from_secs(scan_duration as u64)));
        let devs = self.list_devices()?;
        self.stop_discovery()?;
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
        let mut events = self.central.as_ref().expect("Must call the init method").events().await?;
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
