use gdnative::api::Reference;
use gdnative::prelude::*;
use gdnative::tasks::Async;
use gdnative::tasks::AsyncMethod;

use crate::bluetooth::Bluetooth;
use crate::bluetooth_module::BluetoothModule;

#[derive(NativeClass)]
#[inherit(Reference)]
#[register_with(Self::_register)]
pub struct BluePlugUtils {
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
    // }

    // #[method]
    // fn start_discovery(& mut self) {
    //     if self.bluetooth_mod.start_discovery().is_err() == true {
    //         godot_error!("Error start_discovery()")
    //     }
    // }

    // #[method]
    // fn stop_discovery(& mut self) {
    //     if self.bluetooth_mod.stop_discovery().is_err() == true {
    //         godot_error!("Error stop_discovery()")
    //     }
    // }

    // #[method]
    // fn scan_devices(& mut self, scan_time: u8) {
    //     if self.bluetooth_mod.scan_devices(scan_time).is_err() == true {
    //         godot_error!("Error scan_devices()")
    //     }
    // }

    // #[method]
    // fn list_devices(& mut self) {
    //     if self.bluetooth_mod.list_devices().is_err() == true {
    //         godot_error!("Error list_devices()")
    //     }
    // }

    fn _register(builder: &ClassBuilder<Self>) {
        builder
            .method("get_accounts", Async::new(GetAccounts))
            .done();

        // builder
        //     .property("url")
        //     .with_hint(StringHint::Placeholder {
        //         placeholder: "RPC URL".into(),
        //     })
        //     .with_setter(Self::_set_url)
        //     .with_default("http://localhost:8545".into())
        //     .done();

        // builder.property::<u64>("chain_id").with_default(1).done();
    }
}

struct GetAccounts;

impl AsyncMethod<BluePlugUtils> for GetAccounts {
    fn spawn_with(&self, spawner: gdnative::tasks::Spawner<'_, BluePlugUtils>) {
        godot_print!("GetAccounts");

        spawner.spawn(|_ctx, this, _args| {
            godot_print!("GetAccounts spawn");

            // let provider = this.map(|provider, _owner| {
            //     match provider.active_provider.as_ref().unwrap() {
            //         ActiveProvider::JsonRpc(ref json_rpc) => json_rpc.clone(),
            //         ActiveProvider::LocalWallet(ref local) => local.provider().clone(),
            //     }
            // }).unwrap();
            async move {
                godot_print!("GetAccounts async");
                let res = BluetoothModule::scan_devices(3).await.unwrap();
                println!("{:#?}", res);
                
                // let accounts = provider.get_accounts().await.unwrap();
                
                // accounts
                //     .iter()
                //     .map(|address| {
                //         address.to_fixed_bytes().owned_to_variant()
                //     })
                //     .collect::<Vec<Variant>>()
                //     .to_variant()
                    
                godot_print!("End async");
                42.to_variant()
            }
        })
    }
}
