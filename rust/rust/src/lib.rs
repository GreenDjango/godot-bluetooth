use gdnative::prelude::*;
mod bluetooth;
mod bluetooth_module;
mod native_class;

/// register classes
fn init(handle: InitHandle) {
    gdnative::tasks::register_runtime(&handle);
    gdnative::tasks::set_executor(native_class::async_executor_driver::EXECUTOR.with(|e| *e));

    handle.add_class::<native_class::async_executor_driver::AsyncExecutorDriver>();
    handle.add_class::<native_class::blueplug_utils::BluePlugUtils>();
}

godot_init!(init);
