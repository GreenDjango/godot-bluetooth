use bluetooth_module::BluetoothModule;
use gdnative::api::{Reference};
use gdnative::prelude::*;
use gdnative::tasks::*;
mod bluetooth;
mod bluetooth_module;
use futures::Future;

#[derive(NativeClass)]
#[inherit(Reference)]
struct BluePlugUtils {
    bluetooth_mod: BluetoothModule
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
    
    // #[method(async)]
    async fn init(& mut self) { // impl Future<Output = u8> + 'static{
        self.bluetooth_mod.init().await;
    }
    // #[method(async)]
    // fn answer(&self) -> impl Future<Output = i32> + 'static {
    //     async { 42 }
    // }
}

#[tokio::main]
async fn init(handle: InitHandle) {
    handle.add_class::<BluePlugUtils>();
}

godot_init!(init);