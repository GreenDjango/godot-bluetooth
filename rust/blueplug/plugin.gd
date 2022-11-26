tool
extends EditorPlugin

const AUTOLOAD_NAME = "Bluetooth"

func _enter_tree() -> void:
	add_autoload_singleton(AUTOLOAD_NAME, "res://addons/blueplug/autoload.gd")

func _exit_tree() -> void:
	remove_autoload_singleton(AUTOLOAD_NAME)
