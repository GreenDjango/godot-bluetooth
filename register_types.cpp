/*
** GODOT PROJECT, 2021
** bluetooth module
** File description:
** register_types.cpp
*/

#include "register_types.h"
#include "core/class_db.h"
#include "src/bluetooth_module.h"

void register_bluetooth_types() {
	ClassDB::register_class<Bluetooth>();
}

void unregister_bluetooth_types() {
	// Nothing to do here in this example.
}