extends Control

var blueplug := BluePlug.new()

func _init():
	blueplug.init_adapter(blueplug.adapters()[0])

func _ready():
	pass
	# $GridContainer/DeviceName.text += bluetooth.get_device_name() + ' (' + bluetooth.get_alias() +')'
	# $GridContainer/Address.text += bluetooth.get_address()

func _on_CheckButton2_pressed():
	$GridContainer/ScanButton/RichTextLabel.text = ""
	var new_text = ""
	var devices_list = blueplug.peripherals()
	for device in devices_list:
#		prints(device.name)
		new_text += "[u]{key}[/u]: {value}\n".format({"key":device.address,"value":device.name})
	$GridContainer/ScanButton/RichTextLabel.bbcode_text = new_text
