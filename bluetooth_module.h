/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** bluetooth_module.h
*/

#ifndef BLUETOOTH_MODULE_H
#define BLUETOOTH_MODULE_H

#include "scene/main/node.h"
#include <bluetooth_wrapper.h>

class Bluetooth : public Node {
	GDCLASS(Bluetooth, Node);

private:
	BluetoothWrapper BTwrapper;
	bool requesting;

protected:
	static void _bind_methods();

public:
	void set_powered(const bool enable) const;
	bool is_powered() const;
	String get_device_name() const;
	String get_alias() const;
	String get_address() const;

	Bluetooth();
};

#endif // BLUETOOTH_MODULE_H
