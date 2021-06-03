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

	BluetoothWrapper BTwrapper;

	//BTwrapper.set_powered(false);

	if (BTwrapper.is_powered())
		std::cout << "ON\n";
	else
		std::cout << "OFF\n";

	std::cout << BTwrapper.address() << " " << BTwrapper.name() << " (" << BTwrapper.alias() << ")\n";

	return 0;
}