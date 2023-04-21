extends Control

var gdsmble = GDSimpleBLE.new();

func process():
	print(gdsmble.peripherals())

func _ready():
	print(gdsmble.adapters())
	gdsmble.init_adapter(gdsmble.adapters()[0])
	print(gdsmble.start_scan())
	print(gdsmble.adapters())
	print(gdsmble.peripherals())
	print(gdsmble.scan_is_active())
	# while true:
	#	process()
	pass # Replace with function body.
