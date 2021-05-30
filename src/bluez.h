/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** bluez.cpp
*/

#ifndef BLUEZ_WRAPPER_H
#define BLUEZ_WRAPPER_H

#include <sdbus-c++/sdbus-c++.h>

class BluetoothWrapper {
private:
	const char* destinationName = "org.bluez";
	const char* objectPath = "/org/bluez/hci0";

public:
	void set_powered(const bool enable) const {
		auto hciProxy = sdbus::createProxy(destinationName, objectPath);

		hciProxy->setProperty("Powered").onInterface("org.bluez.Adapter1").toValue(enable);
	};

	bool is_powered() const {
		auto hciProxy = sdbus::createProxy(destinationName, objectPath);

		auto powered = hciProxy->getProperty("Powered").onInterface("org.bluez.Adapter1");
		if (powered)
			return true;
		return false;
	};

};

#endif // BLUEZ_WRAPPER_H


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