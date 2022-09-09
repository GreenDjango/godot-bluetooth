/*****************************************************************************\
**  bluetooth_module.h
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2022 - Godot Bluetooth Module contributors
\*****************************************************************************/

#ifndef BLUETOOTH_MODULE_H
#define BLUETOOTH_MODULE_H

#include "core/reference.h"
#include <bluetooth_wrapper.h>

class Bluetooth : public Reference {
	GDCLASS(Bluetooth, Reference);

private:
	BluetoothWrapper BTwrapper;

protected:
	static void _bind_methods();

public:
	void set_powered(const bool enable) const;
	bool is_powered() const;
	String get_device_name() const;
	String get_alias() const;
	String get_address() const;
	Array list_devices() const;
};

#endif // BLUETOOTH_MODULE_H
