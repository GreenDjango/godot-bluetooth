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

void Bluetooth::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_powered", "enable"), &Bluetooth::set_powered);
	ClassDB::bind_method(D_METHOD("is_powered"), &Bluetooth::is_powered);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "powered"), "set_powered", "is_powered");
}

Bluetooth::Bluetooth() {
	requesting = false;
}