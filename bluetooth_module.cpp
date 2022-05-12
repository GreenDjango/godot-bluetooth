/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** bluetooth_module.cpp
*/

#include "bluetooth_module.h"

void Bluetooth::set_powered(const bool enable) const {
	BTwrapper.set_powered(enable);
}

bool Bluetooth::is_powered() const {
	return BTwrapper.is_powered();
}

String Bluetooth::get_device_name() const {
	return BTwrapper.name().c_str();
}

String Bluetooth::get_alias() const {
	return BTwrapper.get_alias().c_str();
}

String Bluetooth::get_address() const {
	return BTwrapper.address().c_str();
}

HashMap<String, String> Bluetooth::list_devices() const {
	auto devices_list = HashMap<String, String>();
	auto raw_device_list = BTwrapper.list_devices();

	for (auto it = raw_device_list.begin(); it != raw_device_list.end(); ++it) {
		devices_list[it->first.c_str()] = it->second.c_str();
	}
	return devices_list;
}

void Bluetooth::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_device_name"), &Bluetooth::get_device_name);
	ClassDB::bind_method(D_METHOD("get_alias"), &Bluetooth::get_alias);
	ClassDB::bind_method(D_METHOD("get_address"), &Bluetooth::get_address);

	ClassDB::bind_method(D_METHOD("set_powered", "enable"), &Bluetooth::set_powered);
	ClassDB::bind_method(D_METHOD("is_powered"), &Bluetooth::is_powered);

	ClassDB::bind_method(D_METHOD("list_devices"), &Bluetooth::list_devices);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "powered"), "set_powered", "is_powered");
}

Bluetooth::Bluetooth() {
	requesting = false;
}