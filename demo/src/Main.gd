extends Control

func _ready():
	$CheckButton.pressed = $Bluetooth.powered

func _on_CheckButton_toggled(button_pressed):
	$Bluetooth.powered = button_pressed
