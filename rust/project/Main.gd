extends Node

func _ready():
	var blue_plug := BluePlugUtils.new()
	blue_plug.hello_world()
	var res = yield(blue_plug.get_accounts(), "completed")
	print(res)
