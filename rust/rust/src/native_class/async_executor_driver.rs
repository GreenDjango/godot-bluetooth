
use gdnative::prelude::*;
use tokio::runtime::{Builder, Runtime};
use tokio::task::LocalSet;

thread_local! {
    pub static EXECUTOR: &'static SharedLocalPool = {
        Box::leak(Box::new(SharedLocalPool::default()))
    };
}

#[derive(Default)]
pub struct SharedLocalPool {
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

#[derive(NativeClass)]
#[inherit(Node)]
pub struct AsyncExecutorDriver {
    runtime: Runtime,
}

impl AsyncExecutorDriver {
    fn new(_owner: &Node) -> Self {
        AsyncExecutorDriver {
            runtime: Builder::new_current_thread().enable_all().build().unwrap(),
        }
    }
}

#[methods]
impl AsyncExecutorDriver {

    #[method]
    fn _process(&mut self, #[base] _owner: &Node, _delta: f32) {
        EXECUTOR.with(|e| {
            self.runtime
                .block_on(async {
                    e.local_set
                        .run_until(async { tokio::task::spawn_local(async {}).await })
                        .await
                })
                .unwrap()
        });
    }
}
