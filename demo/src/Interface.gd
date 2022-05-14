extends Control

onready var bluetooth := Bluetooth.new()
onready var timer := Timer.new()

func _ready():
	add_child(timer)
	$GridContainer/CheckButton.pressed = bluetooth.powered
	$GridContainer/DeviceName.text += bluetooth.get_device_name() + ' (' + bluetooth.get_alias() +')'
	$GridContainer/Address.text += bluetooth.get_address()

func _process(_delta):
	if timer.is_stopped():
		$GridContainer/CheckButton.pressed = bluetooth.powered
		timer.start()

func _on_CheckButton_toggled(button_pressed):
	bluetooth.powered = button_pressed
	
func _on_CheckButton2_pressed():
	$GridContainer/CheckButton2/RichTextLabel.text = ""
	var new_text = ""
	var devices_list = bluetooth.list_devices()
	for device in devices_list:
#		prints(device.name)
		new_text += "[u]{key}[/u]: {value}\n".format({"key":device.address,"value":device.name})
	$GridContainer/CheckButton2/RichTextLabel.bbcode_text = new_text
