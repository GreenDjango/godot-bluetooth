/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** main.cpp
*/

#include "bluetooth_wrapper.h"

#include <sdbus-c++/sdbus-c++.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

void onConcatenated(sdbus::Signal &signal) {
	std::string concatenatedString;
	signal >> concatenatedString;

	std::cout << "Received signal with concatenated string " << concatenatedString << std::endl;
}

void test_wrapper() {
	BluetoothWrapper BTwrapper{};

	//BTwrapper.set_powered(false);

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
}

int main(int argc, char *argv[]) {
	test_wrapper();

	// Create proxy object for the concatenator object on the server side. Since here
	// we are creating the proxy instance without passing connection to it, the proxy
	// will create its own connection automatically, and it will be system bus connection.
	// busctl get-property org.bluez /org/bluez/hci0 org.bluez.Adapter1 Powered
	const char *destinationName = "org.bluez";
	const char *objectPath = "/org/bluez/hci0";
	auto concatenatorProxy = sdbus::createProxy(destinationName, objectPath);

	// Let's subscribe for the signals
	const char *interfaceName = "org.bluez.Adapter1";
	std::cout << concatenatorProxy->getProperty("Address").onInterface(interfaceName).get<std::string>() << std::endl;

	return 0;

	concatenatorProxy->registerSignalHandler(interfaceName, "Address", &onConcatenated);
	concatenatorProxy->finishRegistration();

	// std::vector<int> numbers = {1, 2, 3};
	// std::string separator = ":";

	// Invoke concatenate on given interface of the object
	{
		auto method = concatenatorProxy->createMethodCall(interfaceName, "Address");
		// method << numbers << separator;
		auto reply = concatenatorProxy->callMethod(method);
		std::string result;
		reply >> result;
		std::cout << result << std::endl;
	}

	// Invoke concatenate again, this time with no numbers and we shall get an error
	/*{
		auto method = concatenatorProxy->createMethodCall(interfaceName, "concatenate");
		method << std::vector<int>() << separator;
		try
		{
			auto reply = concatenatorProxy->callMethod(method);
			assert(false);
		}
		catch(const sdbus::Error& e)
		{
			std::cerr << "Got concatenate error " << e.getName() << " with message " << e.getMessage() << std::endl;
		}
	}*/

	// Give sufficient time to receive 'concatenated' signal from the first concatenate invocation
	sleep(1);

	return 0;
}