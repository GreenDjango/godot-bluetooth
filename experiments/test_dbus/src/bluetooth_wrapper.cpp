/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** bluez.cpp
*/

#include "bluetooth_wrapper.h"

#include <iostream>

const char *bluezName = "org.bluez";
const char *bluezPath = "/org/bluez";
const char *rootPath = "/";
const char *hciPath = "/org/bluez/hci0";
const char *AdapterInterface = "org.bluez.Adapter1";
const char *ObjectManagerInterface = "org.freedesktop.DBus.ObjectManager";

BluetoothWrapper::BluetoothWrapper() {
	rootProxy = std::move(sdbus::createProxy(bluezName, rootPath));
	hciProxy = std::move(sdbus::createProxy(bluezName, hciPath));
}

// Path /

template <typename T>
T BluetoothWrapper::call_root_method(const std::string &methodName, const std::string &interfaceName) const {
	T resultValue;
	rootProxy->callMethod(methodName).onInterface(interfaceName).withArguments().storeResultsTo(resultValue);
	return resultValue;
}

// Object org.bluez.Adapter1

typedef std::map<sdbus::ObjectPath, std::map<std::string, std::map<std::string, sdbus::Variant>>> ManagedObjects;

std::map<std::string, DeviceInfo> BluetoothWrapper::list_devices() const {
	auto data = call_root_method<ManagedObjects>("GetManagedObjects", ObjectManagerInterface);
	auto devices_list = std::map<std::string, DeviceInfo>();

	for (auto paths = data.begin(); paths != data.end(); ++paths) {
		for (auto interfaces = paths->second.begin(); interfaces != paths->second.end(); ++interfaces) {
			if (interfaces->first != "org.bluez.Device1")
				continue;
			auto properties = interfaces->second;
			DeviceInfo device_info = DeviceInfo();

			if (properties.count("Address") == 1)
				device_info.address = properties.at("Address").get<std::string>();
			if (properties.count("AddressType") == 1)
				device_info.address_type = properties.at("AddressType").get<std::string>();
			if (properties.count("Alias") == 1)
				device_info.alias = properties.at("Alias").get<std::string>();
			if (properties.count("Blocked") == 1)
				device_info.blocked = properties.at("Blocked").get<bool>();
			if (properties.count("Connected") == 1)
				device_info.connected = properties.at("Connected").get<bool>();
			if (properties.count("Icon") == 1)
				device_info.icon = properties.at("Icon").get<std::string>();
			if (properties.count("LegacyPairing") == 1)
				device_info.legacy_pairing = properties.at("LegacyPairing").get<bool>();
			if (properties.count("Name") == 1)
				device_info.name = properties.at("Name").get<std::string>();
			if (properties.count("Paired") == 1)
				device_info.paired = properties.at("Paired").get<bool>();
			if (properties.count("Trusted") == 1)
				device_info.trusted = properties.at("Trusted").get<bool>();

			devices_list[paths->first] = device_info;
		}
	}
	return devices_list;
}

// Path /org/bluez/hci0

sdbus::MethodInvoker &BluetoothWrapper::call_hci_method(const std::string &methodName, const std::string &interfaceName) const {
	return hciProxy->callMethod(methodName).onInterface(interfaceName);
}

sdbus::Variant BluetoothWrapper::get_hci_value(const std::string &propertyName, std::string interfaceName) const {
	return hciProxy->getProperty(propertyName).onInterface(interfaceName);
}

void BluetoothWrapper::set_hci_value(const std::string &propertyName, std::string interfaceName, const sdbus::Variant &value) const {
	return hciProxy->setProperty(propertyName).onInterface(interfaceName).toValue(value);
}

// Object org.bluez.Adapter1

void BluetoothWrapper::remove_device(const std::string &device_address) const {
	call_hci_method("RemoveDevice", AdapterInterface).withArguments(device_address).storeResultsTo();
}

void BluetoothWrapper::start_discovery() const {
	hciProxy->callMethod("StartDiscovery").onInterface(AdapterInterface).withArguments().storeResultsTo();
}

void BluetoothWrapper::stop_discovery() const {
	call_hci_method("StopDiscovery", AdapterInterface).withArguments().storeResultsTo();
}

std::string BluetoothWrapper::address() const {
	auto address = get_hci_value("Address", AdapterInterface);
	return address.get<std::string>();
}

std::string BluetoothWrapper::address_type() const {
	auto address_type = get_hci_value("AddressType", AdapterInterface);
	return address_type.get<std::string>();
}

void BluetoothWrapper::set_alias(const std::string &alias) const {
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
