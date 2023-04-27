class_name Animal
extends Node2D

var id : String
var device_name : String
var device_address : String
var device_type : String
const animals = ['snake', 'pigeon']
var animal_id := 0
var to_position := .0
var to_direction := 0

@onready var sprite = $AnimatedSprite
@onready var label = $Front/Label
@onready var panel = $Front/Panel
@onready var description = $Front/Panel/VBoxContainer/Description

func setup(_device_name: String, _device_address: String, _device_type: String):
	device_name = _device_name
	device_address = _device_address
	device_type = _device_type
	id = generate_id(device_address, device_name)
	match device_type:
		'phone':
			animal_id = 1

func _ready():
	panel.visible = false

	label.text = device_name if device_name else "???"
	if (label.text.length() > 10):
		label.text = label.text.substr(0, 12) + "."

	description.text = "Name: {na}\nAddress: {ad}\nType: {ty}\n".format(
		{"ad":device_address,"na":device_name,"ty":device_type}
	)
	
	var frame_length = sprite.frames.get_frame_count(animals[animal_id] + "_walk")
	if (frame_length > 0):
		sprite.frame = randi() % frame_length
	sprite.play(animals[animal_id] + "_walk")

	position.x = position.x + randf_range(-760, 760)
	position.y = position.y + randf_range(-20, 25)
	_change_direction()

func _process(delta):
	if (abs(position.x - to_position) <= 1):
		_change_direction()
	else:
		position.x += to_direction * (15 * delta)

func _change_direction():
	to_position = randf_range(-760, 760)
	to_direction = int(sign(to_position - position.x))
	sprite.flip_h = true if to_direction < 0 else false

static func generate_id(address: String, name: String):
	return address + '/' + name

func _on_Area2D_mouse_entered():
	panel.visible = true

func _on_Area2D_mouse_exited():
	panel.visible = false
