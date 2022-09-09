/*****************************************************************************\
**  register_types.cpp
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2022 - Godot Bluetooth Module contributors
\*****************************************************************************/

#include "register_types.h"
#include "core/class_db.h"

#include "bluetooth_module.h"

void register_bluetooth_types() {
	ClassDB::register_class<Bluetooth>();
}

void unregister_bluetooth_types() {
	// Nothing to do here in this example.
}