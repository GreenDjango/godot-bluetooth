use gdnative::api::{EditorPlugin, Script, Texture};
use gdnative::prelude::*;

#[derive(NativeClass)]
#[inherit(EditorPlugin)]
struct CustomNode;

#[methods]
impl CustomNode {
    fn new(_owner: TRef<EditorPlugin>) -> Self {
        CustomNode
    }

    #[method]
    fn _enter_tree(&self, #[base] owner: TRef<EditorPlugin>) {
        // Initialization of the plugin goes here.
        // Add the new type with a name, a parent type, a script and an icon.
        let script = load::<Script>("res://my_button.gdns").unwrap();
        let texture = load::<Texture>("res://making_plugins-custom_node_icon.png").unwrap();
        owner.add_custom_type("MyButton", "Button", script, texture);
    }

    #[method]
    fn _exit_tree(&self, #[base] owner: TRef<EditorPlugin>) {
        // Clean-up of the plugin goes here.
        // Always remember to remove it from the engine when deactivated.
        owner.remove_custom_type("MyButton");
    }
}

#[derive(NativeClass)]
#[inherit(Button)]
struct MyButton;

#[methods]
impl MyButton {
    fn new(_owner: TRef<Button>) -> Self {
        MyButton
    }

    #[method]
    fn _enter_tree(&self, #[base] owner: TRef<Button>) {
        owner
            .connect("pressed", owner, "clicked", VariantArray::new_shared(), 0)
            .unwrap();
    }

    #[method]
    fn clicked(&self) {
        godot_print!("You clicked me!");
    }
}

fn init(handle: InitHandle) {
    handle.add_tool_class::<CustomNode>();
    handle.add_tool_class::<MyButton>();
}

godot_init!(init);