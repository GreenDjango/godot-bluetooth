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

#include <iostream>

const char* bluezName = "org.bluez";
const char* bluezPath = "/org/bluez";
const char* rootPath = "/";
const char* hciPath = "/org/bluez/hci0";
const char* AdapterInterface = "org.bluez.Adapter1";
const char* ObjectManagerInterface = "org.freedesktop.DBus.ObjectManager";

BluetoothWrapper::BluetoothWrapper() {
	rootProxy = std::move(sdbus::createProxy(bluezName, rootPath));
	hciProxy = std::move(sdbus::createProxy(bluezName, hciPath));
}

// Path /

typedef std::map<std::string, sdbus::Variant> VariantMap;
typedef std::map<std::string, VariantMap> VariantMapMap;
typedef std::map<sdbus::ObjectPath, VariantMapMap> VariantMapMapPath;

template<typename T> 
T BluetoothWrapper::call_root_method(const std::string& methodName,const std::string& interfaceName) const {
	T resultValue;
	rootProxy->callMethod(methodName).onInterface(interfaceName).withArguments().storeResultsTo(resultValue);
	return resultValue;
}

// Object org.bluez.Adapter1

std::map<std::string, std::string> BluetoothWrapper::list_devices() const {
	auto methodName = std::string("GetManagedObjects");

	auto data = call_root_method<VariantMapMapPath>(methodName, ObjectManagerInterface);
	auto devices_list = std::map<std::string, std::string>();

	for (VariantMapMapPath::iterator it = data.begin(); it != data.end(); ++it) {
		// std::cout << it->first << " => " << '\n';
		for (VariantMapMap::iterator yt = it->second.begin(); yt != it->second.end(); ++yt) {
			if (yt->second.count("Address") == 1 && yt->second.count("Name") == 1) {
				auto address = yt->second.at("Address").get<std::string>();
				auto name = yt->second.at("Name").get<std::string>();
				devices_list[address] = name;
				// for (VariantMap::iterator xt = yt->second.begin(); xt != yt->second.end(); ++xt) {
				// 	 if (xt->second.peekValueType() == "s")
				// 		std::cout << "  " << xt->first << " => " << xt->second.get<std::string>() << '\n';
				// }
			}
		}
	}

	return devices_list;
}

// Path /org/bluez/hci0

sdbus::Variant BluetoothWrapper::get_hci_value(const std::string& propertyName, std::string interfaceName) const {
	return hciProxy->getProperty(propertyName).onInterface(interfaceName);
}

void BluetoothWrapper::set_hci_value(const std::string& propertyName, std::string interfaceName, const sdbus::Variant& value) const {
	return hciProxy->setProperty(propertyName).onInterface(interfaceName).toValue(value);
}

// Object org.bluez.Adapter1

std::string BluetoothWrapper::address() const {
	auto address = get_hci_value("Address", AdapterInterface);
	return address.get<std::string>();
}

std::string BluetoothWrapper::address_type() const {
	auto address_type = get_hci_value("AddressType", AdapterInterface);
	return address_type.get<std::string>();
}

void BluetoothWrapper::set_alias(const std::string& alias) const {
	set_hci_value("Alias", AdapterInterface, alias);
}

std::string BluetoothWrapper::get_alias() const {
	auto alias = get_hci_value("Alias", AdapterInterface);
	return alias.get<std::string>();
}

/*
int BluetoothWrapper::device_class() const {
	auto device_class = get_hci_value("Alias", AdapterInterface);
	return device_class.get<int>();
}
*/

void BluetoothWrapper::set_discoverable(const bool enable) const {
	set_hci_value("Discoverable", AdapterInterface, enable);
}

bool BluetoothWrapper::is_discoverable() const {
	auto discoverable = get_hci_value("Discoverable", AdapterInterface);
	if (discoverable)
		return true;
	return false;
}

/*
void BluetoothWrapper::set_discoverable_timeout(const int timeout) const {
	set_hci_value("DiscoverableTimeout", AdapterInterface, timeout);
}

int BluetoothWrapper::get_discoverable_timeout() const {
	auto timeout = get_hci_value("DiscoverableTimeout", AdapterInterface);
	return timeout.get<int>();
}
*/

bool BluetoothWrapper::discovering() const {
	auto discovering = get_hci_value("Discovering", AdapterInterface);
	if (discovering)
		return true;
	return false;
}

std::string BluetoothWrapper::modalias() const {
	auto modalias = get_hci_value("Modalias", AdapterInterface);
	return modalias.get<std::string>();
}

std::string BluetoothWrapper::name() const {
	auto name = get_hci_value("Name", AdapterInterface);

	return name.get<std::string>();
}

void BluetoothWrapper::set_pairable(const bool enable) const {
	set_hci_value("Pairable", AdapterInterface, enable);
}

bool BluetoothWrapper::is_pairable() const {
	auto pairable = get_hci_value("Pairable", AdapterInterface);
	if (pairable)
		return true;
	return false;
}

/*
void BluetoothWrapper::set_pairable_timeout(const int timeout) const {
	set_hci_value("PairableTimeout", AdapterInterface, timeout);
}

int BluetoothWrapper::get_pairable_timeout() const {
	auto timeout = get_hci_value("PairableTimeout", AdapterInterface);
	return timeout.get<int>();
}
*/

void BluetoothWrapper::set_powered(const bool enable) const {
	set_hci_value("Powered", AdapterInterface, enable);
}

bool BluetoothWrapper::is_powered() const {
	auto powered = get_hci_value("Powered", AdapterInterface);
	if (powered)
		return true;
	return false;
}

// std::vector<std::string> BluetoothWrapper::roles() const;
// std::vector<std::string> BluetoothWrapper::uuids() const;


// org.bluez.LEAdvertisingManager1
// char active_instances() const;
// std::vector<std::string> supported_includes() const;
// char supported_instances() const;

/*String Bluetooth::switch_device(bool enable) const {
	// auto method = hciProxy->createMethodCall(interfaceName, "Address");
	// method << numbers << separator;
	// auto reply = hciProxy->callMethod(method);
	// std::string result;
	// reply >> result;
	// std::cout << result << std::endl;
}*/