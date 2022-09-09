/*****************************************************************************\
**  bluetooth_dummy.cpp
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2022 - Godot Bluetooth Module contributors
\*****************************************************************************/

#include "bluetooth_dummy.h"

BluetoothWrapperDummy::~BluetoothWrapperDummy() {}

std::map<std::string, DeviceInfo> BluetoothWrapperDummy::list_devices() const {
	return std::map<std::string, DeviceInfo>();
}

void BluetoothWrapperDummy::remove_device(const std::string &device_address) const {
	(void)device_address;
}
void BluetoothWrapperDummy::start_discovery() const {}
void BluetoothWrapperDummy::stop_discovery() const {}

std::string BluetoothWrapperDummy::address() const {
	return "";
}
std::string BluetoothWrapperDummy::address_type() const {
	return "";
}
void BluetoothWrapperDummy::set_alias(const std::string &alias) const {
	(void)alias;
}
std::string BluetoothWrapperDummy::get_alias() const {
	return "";
}
void BluetoothWrapperDummy::set_discoverable(const bool enable) const {
	(void)enable;
}
bool BluetoothWrapperDummy::is_discoverable() const {
	return false;
}
bool BluetoothWrapperDummy::discovering() const {
	return false;
}
std::string BluetoothWrapperDummy::modalias() const {
	return "";
}
std::string BluetoothWrapperDummy::name() const {
	return "";
}
void BluetoothWrapperDummy::set_pairable(const bool enable) const {
	(void)enable;
}
bool BluetoothWrapperDummy::is_pairable() const {
	return false;
}
void BluetoothWrapperDummy::set_powered(const bool enable) const {
	(void)enable;
}
bool BluetoothWrapperDummy::is_powered() const {
	return false;
}

bool BluetoothWrapperDummy::is_dummy() const {
	return true;
}
