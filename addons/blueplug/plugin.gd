tool
extends EditorPlugin

const AUTOLOAD_NAME = "BluePlug"

# Hooks

# func enable_plugin() -> void:
# 	enable_singleton()

# func disable_plugin() -> void:
# 	disable_singleton()

func _enter_tree() -> void:
	# add_custom_type("BluePlugUtils", "Reference", preload("res://addons/blueplug/native/BluePlugUtils.gdns"), preload("res://addons/blueplug/logos/logo.svg"));
	enable_singleton()

func _exit_tree() -> void:
	# remove_custom_type("BluePlugUtils");
	disable_singleton()

# Utils

func enable_singleton() -> void:
	add_autoload_singleton(AUTOLOAD_NAME, "res://addons/blueplug/autoload.gd")

func disable_singleton() -> void:
	remove_autoload_singleton(AUTOLOAD_NAME)