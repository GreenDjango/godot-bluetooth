extends Node

var AsyncExecutorDriver = preload("res://addons/blueplug/native/AsyncExecutorDriver.gdns")

func _ready() -> void:
	print('Bluetooth Plugin: driver ready')
	
	var driver = AsyncExecutorDriver.new()
	add_child(driver)
