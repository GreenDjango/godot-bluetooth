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

use crate::bluetooth_trait;

struct BluetoothModule {
    manager: Manager,
    adapters: Adapter,
}

// #[tokio::main]
#[async_trait]
impl bluetooth_trait::Bluetooth for BluetoothModule {
    async fn init(&self) -> Result<(), Box<dyn Error>> {
        let manager = Manager::new().await.unwrap();

        // get the first bluetooth adapter
        let adapters = manager.adapters().await?;
        let central = adapters.into_iter().nth(0).unwrap();
        Ok(())
    }

    async fn scan_devices(&self) -> Result<Vec<bluetooth_trait::DeviceInfo>, Box<dyn Error>> {
        Ok(Vec::new())
    }
}