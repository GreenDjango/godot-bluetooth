/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** bluez.cpp
*/

#include "bluetooth_wrapper.h"
// fix vscode linter
#ifndef BLUETOOTH_WRAPPER_H
#define BLUETOOTH_WRAPPER_H
#include <bluetooth_wrapper.h>
#endif
#include <sdbus-c++/sdbus-c++.h>

const char* bluezName = "org.bluez";
const char* AdapterPath = "/org/bluez/hci0";
const char* Adapter = "org.bluez.Adapter1";


void BluetoothWrapper::set_powered(const bool enable) const {
	auto hciProxy = sdbus::createProxy(bluezName, AdapterPath);

	hciProxy->setProperty("Powered").onInterface(Adapter).toValue(enable);
}

bool BluetoothWrapper::is_powered() const {
	auto hciProxy = sdbus::createProxy(bluezName, AdapterPath);

	auto powered = hciProxy->getProperty("Powered").onInterface(Adapter);
	if (powered)
		return true;
	return false;
}

std::string BluetoothWrapper::name() const {
	auto hciProxy = sdbus::createProxy(bluezName, AdapterPath);

	auto name = hciProxy->getProperty("Name").onInterface(Adapter);
	return name.get<std::string>();
}

std::string BluetoothWrapper::alias() const {
	auto hciProxy = sdbus::createProxy(bluezName, AdapterPath);

	auto alias = hciProxy->getProperty("Alias").onInterface(Adapter);
	return alias.get<std::string>();
}

std::string BluetoothWrapper::address() const {
	auto hciProxy = sdbus::createProxy(bluezName, AdapterPath);

	auto address = hciProxy->getProperty("Address").onInterface(Adapter);
	return address.get<std::string>();
}

/*String Bluetooth::switch_device(bool enable) const {
	const char* destinationName = "org.bluez";
	const char* objectPath = "/org/bluez/hci0";
	auto hciProxy = sdbus::createProxy(destinationName, objectPath);

	hciProxy->setProperty("Powered").onInterface("org.bluez.Adapter1").toValue(true);
	// auto method = hciProxy->createMethodCall(interfaceName, "Address");
	// method << numbers << separator;
	// auto reply = hciProxy->callMethod(method);
	// std::string result;
	// reply >> result;
	// std::cout << result << std::endl;

	if (enable)
		return "on";
	return "off";
}*/