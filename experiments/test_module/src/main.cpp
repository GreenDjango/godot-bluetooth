/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** main.cpp
*/

#include "main.h"
#include "module/bluetooth_wrapper.h"

#include <iostream>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	BluetoothWrapper BTwrapper{};

	//BTwrapper.set_powered(false);

	if (BTwrapper.is_powered())
		std::cout << "Bluetooth: ON\n";
	else
		std::cout << "Bluetooth: OFF\n";

	std::cout << "Is pairable: " << (BTwrapper.is_pairable() ? "ON": "OFF") << "\n";

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
		<< "\n";

	return 0;
}