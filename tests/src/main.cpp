/*****************************************************************************\
**  main.cpp
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2022 - Godot Bluetooth Module contributors
\*****************************************************************************/

#include "main.h"
#include "module/bluetooth_wrapper.h"

#include <iostream>

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	BluetoothWrapper BTwrapper{};

	std::cout << "Is dummy: " << (BTwrapper.is_dummy() ? "YES" : "NO") << "\n";

	BTwrapper.set_powered(true);

	if (BTwrapper.is_powered())
		std::cout << "Bluetooth: ON\n";
	else
		std::cout << "Bluetooth: OFF\n";

	std::cout << "Is pairable: " << (BTwrapper.is_pairable() ? "ON" : "OFF") << "\n";

	std::cout << BTwrapper.address_type()
			  << " " << BTwrapper.address()
			  << " " << BTwrapper.name()
			  << " (" << BTwrapper.get_alias()
			  << ")\n";
	std::cout << "Discovrable: " << (BTwrapper.is_discoverable() ? "YES" : "NO") << "\n";
	std::cout << "Discovering: " << (BTwrapper.discovering() ? "YES" : "NO") << "\n";
	//<< " " << BTwrapper.device_class()
	// << " " << BTwrapper.get_discoverable_timeout()
	std::cout << BTwrapper.modalias()
			  // << " " << BTwrapper.get_pairable_timeout()
			  << "\n\n";

	BTwrapper.start_discovery();

	// auto devices_list = BTwrapper.list_devices();
	// for (auto it = devices_list.begin(); it != devices_list.end(); ++it) {
	// 	std::cout << it->first << " | " << it->second.name << "\n";
	// 	std::cout << "  address: " << it->second.address << "\n";
	// 	std::cout << "  address_type: " << it->second.address_type << "\n";
	// 	std::cout << "  alias: " << it->second.alias << "\n";
	// 	std::cout << "  blocked: " << it->second.blocked << "\n";
	// 	std::cout << "  connected: " << it->second.connected << "\n";
	// 	std::cout << "  icon: " << it->second.icon << "\n";
	// 	std::cout << "  legacy_pairing: " << it->second.legacy_pairing << "\n";
	// 	std::cout << "  paired: " << it->second.paired << "\n";
	// 	std::cout << "  trusted: " << it->second.trusted << "\n";
	// }

	return 0;
}