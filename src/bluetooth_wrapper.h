/*
** GODOT PROJECT, 2020
** bluetooth wrapper
** File description:
** bluetooth_wrapper.cpp
*/

#ifndef BLUETOOTH_WRAPPER_H
#define BLUETOOTH_WRAPPER_H

#include <string>

class BluetoothWrapper {

public:
	void set_powered(const bool enable) const;
	bool is_powered() const;
	std::string name() const;
	std::string alias() const;
	std::string address() const;
};

#endif // BLUETOOTH_WRAPPER_H