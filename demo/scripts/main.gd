extends Control

var blueplug := BluePlug.new()
var address := "TO_CHANGE"

func _ready():
	# test()
	pass

func test():
	blueplug.init_adapter(blueplug.adapters()[0])

	blueplug.connect("peripheral_found", _on_peripheral_found)

	blueplug.start_scan()
	await get_tree().create_timer(5).timeout
	blueplug.stop_scan()
	blueplug.connect_peripheral(address)
	print(blueplug.is_peripheral_connected(address))
	print(blueplug.peripheral_services(address))

func _on_peripheral_found(p_address: String):
	prints(p_address, blueplug.peripheral_identifier(p_address))

func _on_animal_demo_pressed():
	get_tree().change_scene_to_file("res://scenes/animal_demo/main.tscn")

func _on_multi_demo_pressed():
	get_tree().change_scene_to_file("res://scenes/multiplayer/main.tscn")
