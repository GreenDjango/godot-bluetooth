extends Control

func _ready():
	$GridContainer/CheckButton.pressed = $Bluetooth.powered
	$GridContainer/DeviceName.text += $Bluetooth.get_device_name() + ' (' + $Bluetooth.get_alias() +')'
	$GridContainer/Address.text += $Bluetooth.get_address()

func _process(_delta):
	if $Timer.is_stopped():
		$GridContainer/CheckButton.pressed = $Bluetooth.powered
		$Timer.start()

func _on_CheckButton_toggled(button_pressed):
	$Bluetooth.powered = button_pressed
	
func _on_CheckButton2_pressed():
	# var res := {'a':'b','c':'d'}
	$GridContainer/CheckButton2/RichTextLabel.text = ""
	var dev = $Bluetooth.list_devices()
	for i in dev:
		# prints(i, res[i])
		$GridContainer/CheckButton2/RichTextLabel.bbcode_text += "[u]{key}[/u]: {value}\n".format({"key":i,"value":dev[i]})
