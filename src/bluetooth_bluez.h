/*****************************************************************************\
**  bluetooth_bluez.h
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2022 - Godot Bluetooth Module contributors
\*****************************************************************************/

#if !defined(BLUETOOTH_WRAPPER_BLUEZ_H)
#define BLUETOOTH_WRAPPER_BLUEZ_H

#include "bluetooth_dummy.h"
#include <QtBluetooth/QBluetoothLocalDevice>

class BluetoothWrapperBluez : public BluetoothWrapperDummy {
public:
	BluetoothWrapperBluez();
	~BluetoothWrapperBluez();

	void set_powered(const bool enable) const override;
	bool is_powered() const override;

	bool is_dummy() const override;

private:
	QBluetoothLocalDevice *localDevice;
};

#endif // BLUETOOTH_WRAPPER_BLUEZ_H
