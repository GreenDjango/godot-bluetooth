/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** bluetooth_module.h
*/

#ifndef BLUETOOTH_MODULE_H
#define BLUETOOTH_MODULE_H

#include "scene/main/node.h"
#include "bluez.h"

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

	Bluetooth();
};

#endif // BLUETOOTH_MODULE_H