use gdnative::api::{Reference};
use gdnative::prelude::*;
mod bluetooth_trait;
mod bluetooth_module;

#[derive(NativeClass)]
#[inherit(Reference)]
struct BluePlugUtils;

#[methods]
impl BluePlugUtils {
    fn new(_owner: &Reference) -> Self {
        Self
    }

    #[method]
    fn hello_world(&self) {
        godot_print!("hello world")
    }
}

fn init(handle: InitHandle) {
    handle.add_class::<BluePlugUtils>();
}

godot_init!(init);