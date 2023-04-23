extends Control

var gdsmble = GDSimpleBLE.new();

func _ready():
	print(gdsmble.adapters())
	gdsmble.init_adapter(gdsmble.adapters()[0])
	print(gdsmble.start_scan())
	print(gdsmble.peripherals())
	print(gdsmble.scan_is_active())
