use bluetooth_module::BluetoothModule;
use bluetooth::Bluetooth;
use gdnative::api::{Reference};
use gdnative::prelude::*;
mod bluetooth;
mod bluetooth_module;

#[derive(NativeClass)]
#[inherit(Reference)]
struct BluePlugUtils {
    bluetooth_mod: BluetoothModule,
}

#[methods]
impl BluePlugUtils {
    fn new(_owner: &Reference) -> Self {
        Self {
            bluetooth_mod: bluetooth_module::BluetoothModule::new().unwrap()
        }
    }
    
    #[method]
    fn hello_world(&self) {
        godot_print!("hello rust world !")
    }
    
    #[method]
    fn init(& mut self) {
        if self.bluetooth_mod.init().is_err() == true {
            godot_error!("Error init()")
        }
    }

    #[method]
    fn start_discovery(& mut self) {
        if self.bluetooth_mod.start_discovery().is_err() == true {
            godot_error!("Error start_discovery()")
        }
    }

    #[method]
    fn stop_discovery(& mut self) {
        if self.bluetooth_mod.stop_discovery().is_err() == true {
            godot_error!("Error stop_discovery()")
        }
    }

    #[method]
    fn scan_devices(& mut self, scan_time: u8) {
        if self.bluetooth_mod.scan_devices(scan_time).is_err() == true {
            godot_error!("Error scan_devices()")
        }
    }

    #[method]
    fn list_devices(& mut self) {
        if self.bluetooth_mod.list_devices().is_err() == true {
            godot_error!("Error list_devices()")
        }
    }
}

#[tokio::main]
async fn init(handle: InitHandle) {
    handle.add_class::<BluePlugUtils>();
}

godot_init!(init);