/*****************************************************************************\
**  bluetooth_bluez.cpp
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2022 - Godot Bluetooth Module contributors
\*****************************************************************************/

#include "bluetooth_bluez.h"
#include <QProcess>

BluetoothWrapperBluez::BluetoothWrapperBluez() :
		localDevice(new QBluetoothLocalDevice()) {}

BluetoothWrapperBluez::~BluetoothWrapperBluez() {
	delete localDevice;
}

void BluetoothWrapperBluez::set_powered(const bool enable) const {
	if (enable) {
		localDevice->powerOn();
		if (enable && localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff) {
			QProcess::execute("rfkill", { "unblock", "bluetooth" });
		}
	} else {
		localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
	}
}
bool BluetoothWrapperBluez::is_powered() const {
	return localDevice->hostMode() != QBluetoothLocalDevice::HostPoweredOff;
}

bool BluetoothWrapperBluez::is_dummy() const {
	return false;
}