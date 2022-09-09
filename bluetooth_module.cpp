/*****************************************************************************\
**  bluetooth_module.cpp
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2022 - Godot Bluetooth Module contributors
\*****************************************************************************/

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

Array Bluetooth::list_devices() const {
	auto devices_list = Array();
	auto raw_device_list = BTwrapper.list_devices();

	for (auto it = raw_device_list.begin(); it != raw_device_list.end(); ++it) {
		auto device_info = Dictionary();
		auto info = it->second;

		device_info["address"] = info.address.c_str();
		device_info["address_type"] = info.address_type.c_str();
		device_info["alias"] = info.alias.c_str();
		device_info["blocked"] = info.blocked;
		device_info["connected"] = info.connected;
		device_info["icon"] = info.icon.c_str();
		device_info["legacy_pairing"] = info.legacy_pairing;
		device_info["name"] = info.name.c_str();
		device_info["paired"] = info.paired;
		device_info["trusted"] = info.trusted;

		devices_list.push_back(device_info);
	}
	return devices_list;
}

void Bluetooth::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_device_name"), &Bluetooth::get_device_name);
	ClassDB::bind_method(D_METHOD("get_alias"), &Bluetooth::get_alias);
	ClassDB::bind_method(D_METHOD("get_address"), &Bluetooth::get_address);
	ClassDB::bind_method(D_METHOD("list_devices"), &Bluetooth::list_devices);

	ClassDB::bind_method(D_METHOD("set_powered", "enable"), &Bluetooth::set_powered);
	ClassDB::bind_method(D_METHOD("is_powered"), &Bluetooth::is_powered);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "powered"), "set_powered", "is_powered");
}