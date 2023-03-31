use gdnative::prelude::*;
mod bluetooth;
mod bluetooth_module;
use tokio::runtime::{Builder, Runtime};
use tokio::task::LocalSet;

/// register classes
fn init(handle: InitHandle) {
    gdnative::tasks::register_runtime(&handle);
    gdnative::tasks::set_executor(EXECUTOR.with(|e| *e));

    handle.add_class::<AsyncExecutorDriver>();
    handle.add_class::<BluePlugUtils>();
}

godot_init!(init);
