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
