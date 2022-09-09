/*
** GODOT PROJECT, 2020
** bluetooth wrapper
** File description:
** bluetooth_wrapper.cpp
*/

#ifndef BLUETOOTH_WRAPPER_H
#define BLUETOOTH_WRAPPER_H

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <vector>

struct DeviceInfo {
	std::string address;
	std::string address_type;
	std::string name;
	std::string alias;
	std::string icon;
	bool paired;
	bool trusted;
	bool blocked;
	bool legacy_pairing;
	bool connected;

	DeviceInfo() {
		address = "",
		address_type = "",
		alias = "",
		blocked = false,
		connected = false,
		icon = "",
		legacy_pairing = false,
		name = "",
		paired = false,
		trusted = false;
	}
};

class BluetoothWrapper {
private:
	// Path /
	std::unique_ptr<sdbus::IProxy> rootProxy;
	template <typename T>
	T call_root_method(const std::string &methodName, const std::string &interfaceName) const;

	// Path /org/bluez/hci0
	std::unique_ptr<sdbus::IProxy> hciProxy;
	sdbus::MethodInvoker &call_hci_method(const std::string &methodName, const std::string &interfaceName) const;
	sdbus::Variant get_hci_value(const std::string &propertyName, std::string interfaceName) const;
	void set_hci_value(const std::string &propertyName, std::string interfaceName, const sdbus::Variant &value) const;

public:
	BluetoothWrapper();

	// Object org.freedesktop.DBus.ObjectManager
	std::map<std::string, DeviceInfo> list_devices() const;

	// Object org.bluez.Adapter1
	void remove_device(const std::string &device_address) const;
	void start_discovery() const;
	void stop_discovery() const;

	std::string address() const;
	std::string address_type() const;
	void set_alias(const std::string &alias) const;
	std::string get_alias() const;
	// int device_class() const;
	void set_discoverable(const bool enable) const;
	bool is_discoverable() const;
	// void set_discoverable_timeout(const int timeout) const;
	// int get_discoverable_timeout() const;
	bool discovering() const;
	std::string modalias() const;
	std::string name() const;
	void set_pairable(const bool enable) const;
	bool is_pairable() const;
	// void set_pairable_timeout(const int timeout) const;
	// int get_pairable_timeout() const;
	void set_powered(const bool enable) const;
	bool is_powered() const;
	// std::vector<std::string> roles() const;
	// std::vector<std::string> uuids() const;

	// org.bluez.LEAdvertisingManager1
	// char active_instances() const;
	// std::vector<std::string> supported_includes() const;
	// char supported_instances() const;
};

// typedef std::unique_ptr<Resource> ResourcePtr;

#endif // BLUETOOTH_WRAPPER_H