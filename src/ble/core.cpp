/*****************************************************************************\
**  core.cpp
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#include "core.h"

#include <simpleble/Adapter.h>

void Core::init(const std::string address) {
	for (auto &adapter : SimpleBLE::Adapter::get_adapters()) {
		if (adapter.address() == address) {
			current_adapter = &adapter;
			break;
		}
	}
	if (!current_adapter) {
		throw "Adapter not found";
	}
/*
	// Set scan found callback
	current_adapter->set_callback_on_scan_found(
			[&](SimpleBLE::Peripheral p_peripheral) {
				try {
					String l_address = String(p_peripheral.address().c_str());
					BLEPeripheral *l_peripheral = internal_get_peripheral(l_address);

					if (!l_peripheral) {
						create_peripheral(p_peripheral, l_address);

					} else {
						l_peripheral->update(p_peripheral);
						emit_peripheral_updated(l_address);
					}
				} catch (std::exception &l_exception) {
					ERR_PRINT("Peripheral error (" + String(l_exception.what()) + ")");
				}
			});

	// Set scan updated callback
	current_adapter->set_callback_on_scan_updated(
			[&](SimpleBLE::Peripheral p_peripheral) {
				try {
					String l_address = String(p_peripheral.address().c_str());
					BLEPeripheral *l_peripheral = internal_get_peripheral(l_address);

					if (!l_peripheral) {
						create_peripheral(p_peripheral, l_address);

					} else {
						l_peripheral->update(p_peripheral);
						emit_peripheral_updated(l_address);
					}
				} catch (std::exception &l_exception) {
					ERR_PRINT("Peripheral error (" + String(l_exception.what()) + ")");
				}
			});

	// Set scan start callback
	current_adapter->set_callback_on_scan_start(
			[&]() {
				emit_scan_started();
			});

	// Set scan stop callback
	current_adapter->set_callback_on_scan_stop(
			[&]() {
				emit_scan_stopped();
			});
			*/
}
