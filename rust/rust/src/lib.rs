use bluetooth_module::BluetoothModule;
use bluetooth::{Bluetooth, DeviceInfo};
use gdnative::api::{Reference};
use gdnative::tasks::{Async, AsyncMethod};
use gdnative::prelude::*;
mod bluetooth;
mod bluetooth_module;
use std::{cell::RefCell, sync::Arc};
use gdnative::{prelude::*, tasks::Context};
use tokio;
use std::thread;

////
///
///
///
//// 

use tokio::task::LocalSet;

#[derive(Default)]
struct SharedLocalPool {
    local_set: LocalSet,
}

impl futures::task::LocalSpawn for SharedLocalPool {
    fn spawn_local_obj(
        &self,
        future: futures::task::LocalFutureObj<'static, ()>,
    ) -> Result<(), futures::task::SpawnError> {
        self.local_set.spawn_local(future);

        Ok(())
    }
}

thread_local! {
    static EXECUTOR: &'static SharedLocalPool = {
        Box::leak(Box::new(SharedLocalPool::default()))
    };
}

use tokio::runtime::{Builder, Runtime};

#[derive(NativeClass)]
#[inherit(Node)]
struct AsyncExecutorDriver {
    runtime: Runtime,
}

impl AsyncExecutorDriver {
    fn new(_base: &Node) -> Self {
        AsyncExecutorDriver {
            runtime: Builder::new_current_thread()
                .enable_io() 	// optional, depending on your needs
                .enable_time() 	// optional, depending on your needs
                .build()
                .unwrap(),
        }
    }
}

#[methods]
impl AsyncExecutorDriver {
    #[method]
    fn _process(&self, #[base] _base: &Node, _delta: f64) {
        EXECUTOR.with(|e| {
            self.runtime
                .block_on(async {
                    e.local_set
                        .run_until(async {
                            tokio::task::spawn_local(async {}).await
                        })
                        .await
                })
                .unwrap()
        })
    }
}

////
///
///
///
//// 

#[derive(NativeClass, Clone)]
#[inherit(Reference)]
struct BluePlugUtils {
}


struct AsTest;
impl AsyncMethod<BluePlugUtils> for AsTest {
    fn spawn_with(& self, spawner: gdnative::tasks::Spawner<'_, BluePlugUtils>) {
        spawner.spawn(|_ctx, this, _args| {
            async move {
                let res = BluetoothModule::scan_devices(3);
                println!("{:#?}", res.unwrap());
                godot_print!("Scanned !");
                42.to_variant()
                // let a: Vec<Variant> = Vec::new();
                // let b: Variant = Variant::new(String::from("str"));
                // b
            }
        })
    }
}

#[methods]
impl BluePlugUtils {
    fn new(_owner: &Reference) -> Self {
        Self {}
    }
    
    #[method]
    fn hello_world(&self) {
        godot_print!("hello rust world !")
    }
    
    // #[method]
    // fn init(& mut self) {
    //     if self.bluetooth_mod.init().is_err() == true {
    //         godot_error!("Error init()")
    //     }
    //     godot_print!("Initialized")
    // }
    // https://github.com/529124368/rust_mir2
    // #[method]
    // fn scan_devices(& self) {
    //     thread::spawn(move || {
    //         let a = tokio::runtime::Runtime::new().unwrap();
    //         a.block_on(async move {
    //             let res = BluetoothModule::scan_devices(3);
    //             println!("{:#?}", res.unwrap());
    //             godot_print!("Scanned !");
    //             42.to_variant()
    //         });
    //     });
    // }

    // #[method]
    // fn scan_devices(& mut self) { // -> Dictionary {
    //     let res = BluetoothModule::scan_devices(3);
    //     println!("{:#?}", res.unwrap());
    //     // let a = Dictionary::from(res.unwrap().first().unwrap());
    //     // a
    //     // if res.is_err() == true {
    //     //     godot_error!("Error list_devices()")
    //     // }
    // }
}

#[tokio::main]
async fn init(handle: InitHandle) {
    gdnative::tasks::register_runtime(&handle);
    gdnative::tasks::set_executor(EXECUTOR.with(|e| *e));
    handle.add_class::<AsyncExecutorDriver>();
    handle.add_class::<BluePlugUtils>();
}

godot_init!(init);