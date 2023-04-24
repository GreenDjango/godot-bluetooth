extends Control

var blueplug = BluePlug.new()

func _ready():
	print(blueplug.adapters())
	blueplug.init_adapter(blueplug.adapters()[0])
	print(blueplug.start_scan())
	print(blueplug.peripherals())
	print(blueplug.scan_is_active())
