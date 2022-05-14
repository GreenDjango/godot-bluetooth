extends Node2D

var time_elapse := 0.0
const animal_scene := preload("res://src/Animal.tscn")

onready var animals_node := $World/Animals
onready var bluetooth := Bluetooth.new()

func _ready():
	randomize()
	refresh_devices()

func _process(delta):
	time_elapse += delta

	if (time_elapse > 5):
		time_elapse = 0
		refresh_devices()

func refresh_devices():
	var animals := animals_node.get_children()
	var devices_list := bluetooth.list_devices()

	for device in devices_list:
		var exist := false
		var id = Animal.generate_id(device.address, device.name)

		for current_animal in animals:
			if (current_animal.id == id):
				exist = true
				break
		if (exist):
			continue

		var animal : Animal = animal_scene.instance()
		animal.setup(device.name, device.address, device.icon)
		animals_node.add_child(animal, true)
		
	for current_animal in animals:
		var to_remove := true

		for device in devices_list:
			var id = Animal.generate_id(device.address, device.name)
			if (current_animal.id == id):
				to_remove = false
				break
		if (!to_remove):
			continue
		current_animal.queue_free()

