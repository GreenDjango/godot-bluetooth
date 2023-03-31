extends Node

func _ready():
	var driver := AsyncExecutorDriver.new()
	add_child(driver)

	var blue_plug := BluePlugUtils.new()

	blue_plug.hello_world()
	var res = blue_plug.scan_devices()
	print(res)

