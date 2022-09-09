/*****************************************************************************\
**  bluetooth_wrapper.h
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2022 - Godot Bluetooth Module contributors
\*****************************************************************************/

#if !defined(BLUETOOTH_WRAPPER_H)
#define BLUETOOTH_WRAPPER_H

#if defined(GODOT_BLUETOOTH_P_X11)

#include "bluetooth_bluez.h"
using BluetoothWrapper = BluetoothWrapperBluez;

#else

#include "bluetooth_dummy.h"
using BluetoothWrapper = BluetoothWrapperDummy;

#endif

#endif // BLUETOOTH_WRAPPER_H