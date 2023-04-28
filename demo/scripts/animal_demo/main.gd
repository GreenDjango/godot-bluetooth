extends Node2D

var time_elapse := 0.0
const animal_scene := preload("res://scenes/animal_demo/animal.tscn")

@onready var animals_node := $World/Animals

func _ready():
	get_window().content_scale_mode = Window.CONTENT_SCALE_MODE_VIEWPORT
	get_window().content_scale_aspect = Window.CONTENT_SCALE_ASPECT_KEEP
	randomize()
	refresh_devices()

func _process(delta):
	time_elapse += delta

	if (time_elapse > 5):
		time_elapse = 0
		refresh_devices()

func refresh_devices():
	var animals := animals_node.get_children()
	var devices_list = $CanvasLayer/Interface.blueplug.peripherals()

	for device in devices_list:
		var exist := false

		for current_animal in animals:
			if (current_animal.device_address == device):
				exist = true
				break
		if (exist):
			continue

		var animal : Animal = animal_scene.instantiate()
		if $CanvasLayer/Interface.blueplug.peripheral_has_no_identifier(device):
			animal.setup("", device, "phone")
		else:
			animal.setup($CanvasLayer/Interface.blueplug.peripheral_identifier(device), device, "other")
		animals_node.add_child(animal, true)
		
	for current_animal in animals:
		var to_remove := true

		for device in devices_list:
			if (current_animal.device_address == device):
				to_remove = false
				break
		if (!to_remove):
			continue
		current_animal.queue_free()

