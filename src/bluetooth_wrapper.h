/*
** GODOT PROJECT, 2020
** bluetooth wrapper
** File description:
** bluetooth_wrapper.cpp
*/

#ifndef BLUETOOTH_WRAPPER_H
#define BLUETOOTH_WRAPPER_H

#include <string>
#include <vector>
#include <sdbus-c++/sdbus-c++.h>

class BluetoothWrapper {

private:
	// Path /
	std::unique_ptr<sdbus::IProxy> rootProxy;
	sdbus::Variant call_root_method(const std::string& methodName,const std::string& interfaceName) const;
	// Path /org/bluez/hci0
	std::unique_ptr<sdbus::IProxy> hciProxy;
	sdbus::Variant get_hci_value(const std::string& propertyName, std::string interfaceName) const;
	void set_hci_value(const std::string& propertyName, std::string interfaceName, const sdbus::Variant& value) const;

public:
	BluetoothWrapper();

	// Object org.freedesktop.DBus.ObjectManager
	std::map<std::string, std::string> list_devices() const;

	// Object org.bluez.Adapter1
	std::string address() const;
	std::string address_type() const;
	void set_alias(const std::string& alias) const;
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

/*
 '/org/bluez/hci0/dev_E0_C3_77_8C_AB_53': {'org.bluez.Device1': {'Adapter': '/org/bluez/hci0',
                                                                 'Address': 'E0:C3:77:8C:AB:53',
                                                                 'AddressType': 'public',
                                                                 'Alias': 'Galaxy '
                                                                          'A22',
                                                                 'Blocked': False,
                                                                 'Class': 5898764,
                                                                 'Connected': False,
                                                                 'Icon': 'phone',
                                                                 'LegacyPairing': False,
                                                                 'Modalias': 'bluetooth:v0075p0100d0201',
                                                                 'Name': 'Galaxy '
                                                                         'A22',
                                                                 'Paired': True,
                                                                 'ServicesResolved': False,
                                                                 'Trusted': True,
                                                                 'UUIDs': ['00001105-0000-1000-8000-00805f9b34fb',
                                                                           '0000110a-0000-1000-8000-00805f9b34fb',
                                                                           '0000110c-0000-1000-8000-00805f9b34fb',
                                                                           '0000110e-0000-1000-8000-00805f9b34fb',
                                                                           '00001112-0000-1000-8000-00805f9b34fb',
                                                                           '00001115-0000-1000-8000-00805f9b34fb',
                                                                           '00001116-0000-1000-8000-00805f9b34fb',
                                                                           '0000111f-0000-1000-8000-00805f9b34fb',
                                                                           '0000112d-0000-1000-8000-00805f9b34fb',
                                                                           '0000112f-0000-1000-8000-00805f9b34fb',
                                                                           '00001132-0000-1000-8000-00805f9b34fb',
                                                                           '00001200-0000-1000-8000-00805f9b34fb',
                                                                           '00001800-0000-1000-8000-00805f9b34fb',
                                                                           '00001801-0000-1000-8000-00805f9b34fb',
                                                                           'a23d00bc-217c-123b-9c00-fc44577136ee']},
                                           'org.bluez.MediaControl1': {'Connected': False},
                                           'org.bluez.Network1': {'Connected': False},
                                           'org.freedesktop.DBus.Introspectable': {},
                                           'org.freedesktop.DBus.Properties': {}}}
*/

#endif // BLUETOOTH_WRAPPER_H