extends Control

var blueplug := BluePlug.new()
var is_scanning := false

func _init():
	blueplug.init_adapter(blueplug.adapters()[0])
	blueplug.connect("peripheral_found", _on_peripheral_found)

func _ready():
	$GridContainer/DeviceName.text += blueplug.identifier() + ' (' + OS.get_environment("USER") +')'
	$GridContainer/Address.text += blueplug.adapters()[0]

func _on_scan_button_pressed():
	if not is_scanning:
		blueplug.start_scan()
		is_scanning = true
		$GridContainer/ScanButton.text = "Stop scan"
		$GridContainer/RichTextLabel.text = ""
	else:
		blueplug.stop_scan()
		is_scanning = false
		$GridContainer/ScanButton.text = "Start scan"

func _on_peripheral_found(address: String):
	if not blueplug.peripheral_has_no_identifier(address):
		var new_text = "[u]%s[/u]: %s\n" % [address, blueplug.peripheral_identifier(address)]
		$GridContainer/RichTextLabel.append_text(new_text)
