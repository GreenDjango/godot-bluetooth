/*****************************************************************************\
**  blueplug.cpp
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#include "blueplug.h"
#include "ble/utils.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

BluePlug::BluePlug() :
		m_adapter(nullptr) {
	try {
		m_adapters = SimpleBLE::Adapter::get_adapters();
		// Check if empty
		if (m_adapters.empty()) {
			ERR_PRINT("No bluetooth adapter found");
		}
	} catch (std::exception &l_exception) {
		ERR_PRINT("Error while collecting bluetooth adapters (" + String(l_exception.what()) + ")");
	}
}

BluePlug::~BluePlug() {
	for (const auto &l_peripheral : m_peripherals) {
		delete (l_peripheral.second);
	}
}

//###############################################################
//	Godot methods
//###############################################################

void BluePlug::_bind_methods() {
	/**
	 * General
	 */

	// Bind general getters
	ClassDB::bind_method(D_METHOD("adapters"), &BluePlug::get_adapters);
	ClassDB::bind_method(D_METHOD("peripherals"), &BluePlug::get_peripherals);
	ClassDB::bind_method(D_METHOD("connected_peripherals"), &BluePlug::get_connected_peripherals);
	ClassDB::bind_method(D_METHOD("company_name"), &BluePlug::get_company_name);
	ClassDB::bind_method(D_METHOD("status_string"), &BluePlug::get_status_string);
	ClassDB::bind_method(D_METHOD("status_level_string"), &BluePlug::get_status_level_string);

	// Error status signal
	ADD_SIGNAL(MethodInfo("adapter_status_updated", PropertyInfo(Variant::INT, "code"), PropertyInfo(Variant::STRING, "what"), PropertyInfo(Variant::STRING, "level")));
	ADD_SIGNAL(MethodInfo("peripheral_status_updated", PropertyInfo(Variant::STRING, "address"), PropertyInfo(Variant::INT, "code"), PropertyInfo(Variant::STRING, "what"), PropertyInfo(Variant::STRING, "level")));

	/**
	 * Adapter
	 */

	// Bind adapter actions
	ClassDB::bind_method(D_METHOD("init_adapter"), &BluePlug::init_adapter);
	ClassDB::bind_method(D_METHOD("start_scan"), &BluePlug::scan_start);
	ClassDB::bind_method(D_METHOD("stop_scan"), &BluePlug::scan_stop);

	// Bind adapter getters
	ClassDB::bind_method(D_METHOD("identifier"), &BluePlug::get_identifier);
	ClassDB::bind_method(D_METHOD("address"), &BluePlug::get_address);
	ClassDB::bind_method(D_METHOD("scan_is_active"), &BluePlug::get_scan_is_active);

	// Adapter signals
	ADD_SIGNAL(MethodInfo("scan_started"));
	ADD_SIGNAL(MethodInfo("scan_stopped"));
	ADD_SIGNAL(MethodInfo("peripheral_found", PropertyInfo(Variant::STRING, "address")));
	ADD_SIGNAL(MethodInfo("peripheral_updated", PropertyInfo(Variant::STRING, "address")));
	ADD_SIGNAL(MethodInfo("peripheral_connected", PropertyInfo(Variant::STRING, "address")));
	ADD_SIGNAL(MethodInfo("peripheral_disconnected", PropertyInfo(Variant::STRING, "address")));

	/**
	 * Peripheral
	 */

	// Bind peripheral getters
	ClassDB::bind_method(D_METHOD("delete_peripheral"), &BluePlug::delete_peripheral);
	ClassDB::bind_method(D_METHOD("is_peripheral_exists"), &BluePlug::get_is_peripheral_exists);
	ClassDB::bind_method(D_METHOD("peripheral_identifier"), &BluePlug::get_peripheral_identifier);
	ClassDB::bind_method(D_METHOD("peripheral_rssi"), &BluePlug::get_peripheral_rssi);
	ClassDB::bind_method(D_METHOD("is_peripheral_connected"), &BluePlug::get_is_peripheral_connected);
	ClassDB::bind_method(D_METHOD("is_peripheral_connectable"), &BluePlug::get_is_peripheral_connectable);
	ClassDB::bind_method(D_METHOD("peripheral_services_count"), &BluePlug::get_peripheral_services_count);
	ClassDB::bind_method(D_METHOD("peripheral_services"), &BluePlug::get_peripheral_services);
	ClassDB::bind_method(D_METHOD("peripheral_manufacturer_data"), &BluePlug::get_peripheral_manufacturer_data);
	ClassDB::bind_method(D_METHOD("is_peripheral_paired"), &BluePlug::get_is_peripheral_paired);
	ClassDB::bind_method(D_METHOD("peripheral_has_no_identifier"), &BluePlug::get_peripheral_has_no_identifier);

	// Bind peripheral actions
	ClassDB::bind_method(D_METHOD("connect_peripheral"), &BluePlug::connect_peripheral);
	ClassDB::bind_method(D_METHOD("disconnect_peripheral"), &BluePlug::disconnect_peripheral);
	ClassDB::bind_method(D_METHOD("unpair_peripheral"), &BluePlug::unpair_peripheral);

	// Bind peripheral BLE interactions
	ClassDB::bind_method(D_METHOD("read"), &BluePlug::read);
	ClassDB::bind_method(D_METHOD("read_descriptor"), &BluePlug::read_descriptor);
	ClassDB::bind_method(D_METHOD("write_request"), &BluePlug::write_request);
	ClassDB::bind_method(D_METHOD("write_command"), &BluePlug::write_command);
	ClassDB::bind_method(D_METHOD("write_descriptor"), &BluePlug::write_descriptor);
	ClassDB::bind_method(D_METHOD("notify"), &BluePlug::notify);
	ClassDB::bind_method(D_METHOD("indicate"), &BluePlug::indicate);
	ClassDB::bind_method(D_METHOD("unsubscribe"), &BluePlug::unsubscribe);

	// Peripheral signals
	ADD_SIGNAL(MethodInfo("peripheral_notified", PropertyInfo(Variant::STRING, "address"), PropertyInfo(Variant::PACKED_BYTE_ARRAY, "payload")));
	ADD_SIGNAL(MethodInfo("peripheral_indicated", PropertyInfo(Variant::STRING, "address"), PropertyInfo(Variant::PACKED_BYTE_ARRAY, "payload")));
}

//###############################################################
//	Internals
//###############################################################

BLEPeripheral *BluePlug::get_peripheral(const String &p_address) {
	if (!m_adapter) {
		emit_adapter_status(BLEUtils::Status::NOT_INITIALIZED);
		return nullptr;
	}
	auto l_peripheral = m_peripherals.find(p_address);
	if (l_peripheral == m_peripherals.end()) {
		emit_peripheral_status(p_address, BLEUtils::Status::NOT_FOUND);
		return nullptr;
	}
	return l_peripheral->second;
}

BLEPeripheral *BluePlug::internal_get_peripheral(const String &p_address) {
	auto l_peripheral = m_peripherals.find(p_address);
	if (l_peripheral == m_peripherals.end()) {
		return nullptr;
	}
	return l_peripheral->second;
}

void BluePlug::create_peripheral(SimpleBLE::Peripheral &p_peripheral, const String &p_address) {
	// Save new peripheral
	BLEPeripheral *l_peripheral = new BLEPeripheral(p_peripheral, this);
	m_peripherals.insert(std::make_pair(p_address, l_peripheral));

	// Set peripheral connected callback
	p_peripheral.set_callback_on_connected(
			[&, p_address, l_peripheral]() {
				// Not found
				if (!l_peripheral) {
					// Should not be possible
					emit_peripheral_status(p_address, BLEUtils::Status::NOT_FOUND);

					// Not connected
				} else if (!l_peripheral->get_is_connected()) {
					l_peripheral->set_is_connected(true);
					l_peripheral->update();
					emit_peripheral_connected(p_address);

					// Connected
				} else {
					// Should not be possible
					emit_peripheral_status(p_address, BLEUtils::Status::ALREADY_CONNECTED);
				}
			});

	// Set peripheral disconnected callback
	p_peripheral.set_callback_on_disconnected(
			[&, p_address, l_peripheral]() {
				// Not Found
				if (!l_peripheral) {
					// Should not be possible
					emit_peripheral_status(p_address, BLEUtils::Status::NOT_FOUND);

					// Connected
				} else if (l_peripheral->get_is_connected()) {
					l_peripheral->set_is_connected(false);
					emit_peripheral_disconnected(p_address);

					// Not connected
				} else {
					// Should not be possible but seems to be sent twice on Linux
					emit_peripheral_status(p_address, BLEUtils::Status::ALREADY_DISCONNECTED);
				}
			});
	try {
		// Emit found
		emit_peripheral_found(p_address);
	} catch (std::exception &l_exception) {
		emit_peripheral_status(p_address, l_exception);
	}
}

//###############################################################
//	Emit handlers
//###############################################################

void BluePlug::emit_adapter_status(const BLEUtils::Status &p_status, const String &p_what, const BLEUtils::StatusLevel &p_level) {
	emit_signal("adapter_status_updated", p_status, p_what, p_level);
}

void BluePlug::emit_adapter_status(std::exception &p_exception, const BLEUtils::StatusLevel &p_level) {
	emit_signal("adapter_status_updated", BLEUtils::get_status(p_exception), String(p_exception.what()), p_level);
}

void BluePlug::emit_adapter_status(const BLEUtils::Status &p_status, const BLEUtils::StatusLevel &p_level) {
	emit_signal("adapter_status_updated", p_status, BLEUtils::get_status_string(p_status), p_level);
}

void BluePlug::emit_scan_stopped() {
	emit_signal("scan_stopped");
}

void BluePlug::emit_scan_started() {
	emit_signal("scan_started");
}

void BluePlug::emit_peripheral_status(const String &p_address, const BLEUtils::Status &p_status, const String &p_what, const BLEUtils::StatusLevel &p_level) {
	emit_signal("peripheral_status_updated", p_address, p_status, p_what, p_level);
}

void BluePlug::emit_peripheral_status(const String &p_address, std::exception &p_exception, const BLEUtils::StatusLevel &p_level) {
	emit_signal("peripheral_status_updated", p_address, BLEUtils::get_status(p_exception), String(p_exception.what()), p_level);
}

void BluePlug::emit_peripheral_status(const String &p_address, const BLEUtils::Status &p_status, const BLEUtils::StatusLevel &p_level) {
	emit_signal("peripheral_status_updated", p_address, p_status, get_status_string(p_status), p_level);
}

void BluePlug::emit_peripheral_notify(const String &p_address, const PackedByteArray &p_payload) {
	emit_signal("peripheral_notified", p_address, p_payload);
}

void BluePlug::emit_peripheral_indicate(const String &p_address, const PackedByteArray &p_payload) {
	emit_signal("peripheral_indicated", p_address, p_payload);
}

void BluePlug::emit_peripheral_found(const String &p_address) {
	emit_signal("peripheral_found", p_address);
}

void BluePlug::emit_peripheral_disconnected(const String &p_address) {
	emit_signal("peripheral_disconnected", p_address);
}

void BluePlug::emit_peripheral_connected(const String &p_address) {
	emit_signal("peripheral_connected", p_address);
}

void BluePlug::emit_peripheral_updated(const String &p_address) {
	emit_signal("peripheral_updated", p_address);
}

//###############################################################
//	Adapter actions
//###############################################################

bool BluePlug::init_adapter(const String p_address) {
	// Only one adapter, reset the previous one
	if (m_adapter) {
		try {
			m_adapter->set_callback_on_scan_found(nullptr);
			m_adapter->set_callback_on_scan_updated(nullptr);
			m_adapter->set_callback_on_scan_start(nullptr);
			m_adapter->set_callback_on_scan_stop(nullptr);
		} catch (std::exception &l_exception) {
			emit_adapter_status(l_exception, BLEUtils::StatusLevel::WARNING);
		}
		m_adapter = nullptr;
	}

	// Get adapters
	for (auto &l_adapter : m_adapters) {
		try {
			// if match or if p_address is empty
			if (p_address == "" || l_adapter.address() == p_address.utf8().get_data()) {
				m_adapter = &l_adapter;
				break;
			}
		} catch (std::exception &l_exception) {
			emit_adapter_status(l_exception, BLEUtils::StatusLevel::WARNING);
		}
	}

	if (!m_adapter) {
		emit_adapter_status(BLEUtils::Status::NOT_FOUND);
		return false;
	}

	try {
		// Set scan found callback
		m_adapter->set_callback_on_scan_found(
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
		m_adapter->set_callback_on_scan_updated(
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
		m_adapter->set_callback_on_scan_start(
				[&]() {
					emit_scan_started();
				});

		// Set scan stop callback
		m_adapter->set_callback_on_scan_stop(
				[&]() {
					emit_scan_stopped();
				});
	} catch (std::exception &l_exception) {
		emit_adapter_status(l_exception);
		return false;
	}

	// OK
	return true;
}

bool BluePlug::scan_start() {
	try {
		if (!m_adapter) {
			emit_adapter_status(BLEUtils::Status::NOT_INITIALIZED);
			return false;
		}
		m_adapter->scan_start();
		return true;
	} catch (std::exception &l_exception) {
		emit_adapter_status(l_exception);
		return false;
	}
}

bool BluePlug::scan_stop() {
	try {
		if (!m_adapter) {
			emit_adapter_status(BLEUtils::Status::NOT_INITIALIZED);
			return false;
		}
		m_adapter->scan_stop();
		return true;
	} catch (std::exception &l_exception) {
		emit_adapter_status(l_exception);
		return false;
	}
}

bool BluePlug::scan_for(const int p_timeout_ms) {
	try {
		if (!m_adapter) {
			emit_adapter_status(BLEUtils::Status::NOT_INITIALIZED);
			return false;
		}
		m_adapter->scan_for(p_timeout_ms);
		return true;
	} catch (std::exception &l_exception) {
		emit_adapter_status(l_exception);
		return false;
	}
}

void BluePlug::delete_peripheral(const String p_address) {
	BLEPeripheral *l_peripheral = internal_get_peripheral(p_address);
	if (l_peripheral) {
		delete l_peripheral;
		m_peripherals.erase(p_address);
	}
}

//###############################################################
//	Adapter getters
//###############################################################

Variant BluePlug::get_identifier() {
	try {
		if (!m_adapter) {
			emit_adapter_status(BLEUtils::Status::NOT_INITIALIZED);
			return Variant();
		}
		return String(m_adapter->identifier().c_str());
	} catch (std::exception &l_exception) {
		emit_adapter_status(l_exception);
		return Variant();
	}
}

Variant BluePlug::get_address() {
	try {
		if (!m_adapter) {
			emit_adapter_status(BLEUtils::Status::NOT_INITIALIZED);
			return Variant();
		}
		return String(m_adapter->address().c_str());
	} catch (std::exception &l_exception) {
		emit_adapter_status(l_exception);
		return Variant();
	}
}

Variant BluePlug::get_scan_is_active() {
	try {
		if (!m_adapter) {
			emit_adapter_status(BLEUtils::Status::NOT_INITIALIZED);
			return false;
		}
		return m_adapter->scan_is_active();
	} catch (std::exception &l_exception) {
		emit_adapter_status(l_exception);
		return Variant();
	}
}

//###############################################################
//	Peripheral actions
//###############################################################

Variant BluePlug::connect_peripheral(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->connect_peripheral();
	}
	return Variant();
}

Variant BluePlug::disconnect_peripheral(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->disconnect_peripheral();
	}
	return Variant();
}

Variant BluePlug::unpair_peripheral(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->unpair_peripheral();
	}
	return Variant();
}

//###############################################################
//	Peripheral getters
//###############################################################

Variant BluePlug::get_is_peripheral_exists(const String p_address) {
	return get_peripheral(p_address) != nullptr;
}

Variant BluePlug::get_peripheral_identifier(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->get_identifier();
	}
	return Variant();
}

Variant BluePlug::get_peripheral_rssi(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->get_rssi();
	}
	return Variant();
}

Variant BluePlug::get_is_peripheral_connected(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->get_is_connected();
	}
	return Variant();
}

Variant BluePlug::get_peripheral_services_count(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->get_services_count();
	}
	return Variant();
}

Variant BluePlug::get_peripheral_services(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->get_services();
	}
	return Variant();
}

Variant BluePlug::get_peripheral_manufacturer_data(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->get_manufacturer_data();
	}
	return Variant();
}

Variant BluePlug::get_is_peripheral_paired(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->get_is_paired();
	}
	return Variant();
}

Variant BluePlug::get_peripheral_has_no_identifier(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->get_has_no_identifier();
	}
	return Variant();
}

Variant BluePlug::get_is_peripheral_connectable(const String p_address) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->get_is_connectable();
	}
	return Variant();
}

//###############################################################
//	Peripheral BLE interactions
//###############################################################

Variant BluePlug::read(const String p_address, const String p_service, const String p_characteristic) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->read(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
	}
	return Variant();
}

Variant BluePlug::read_descriptor(const String p_address, const String p_service, const String p_characteristic, const String p_descriptor) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->read_descriptor(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_descriptor.utf8().get_data());
	}
	return Variant();
}

Variant BluePlug::write_request(const String p_address, const String p_service, const String p_characteristic, const String p_data) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->write_request(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_data.utf8().get_data());
	}
	return Variant();
}

Variant BluePlug::write_command(const String p_address, const String p_service, const String p_characteristic, const String p_data) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->write_command(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_data.utf8().get_data());
	}
	return Variant();
}

Variant BluePlug::write_descriptor(const String p_address, const String p_service, const String p_characteristic, const String p_descriptor, const String p_data) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->write_descriptor(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_descriptor.utf8().get_data(), p_data.utf8().get_data());
	}
	return Variant();
}

Variant BluePlug::notify(const String p_address, const String p_service, const String p_characteristic) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->notify(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
	}
	return Variant();
}

Variant BluePlug::indicate(const String p_address, const String p_service, const String p_characteristic) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->indicate(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
	}
	return Variant();
}

Variant BluePlug::unsubscribe(const String p_address, const String p_service, const String p_characteristic) {
	BLEPeripheral *l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		return l_peripheral->unsubscribe(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
	}
	return Variant();
}

//###############################################################
//	General getters
//###############################################################

Array BluePlug::get_adapters() {
	// Return values
	Array l_return;

	for (auto l_adapter = m_adapters.begin(); l_adapter != m_adapters.end();) {
		Dictionary l_data;

		// Check results
		try {
			l_return.push_back(String(l_adapter->address().c_str()));
			++l_adapter;
		} catch (std::exception &l_exception) {
			ERR_PRINT("Error while collecting bluetooth adapters (" + String(l_exception.what()) + ")");
		}
	}

	// Return UUID array
	return l_return;
}

Dictionary BluePlug::get_peripherals() {
	// Return values
	Dictionary l_return;

	// Get and returns scan results
	for (auto l_peripheral : m_peripherals) {
		l_return[l_peripheral.first] = l_peripheral.second->get_address();
	}

	// Return value array
	return l_return;
}

Dictionary BluePlug::get_connected_peripherals() {
	// Return values
	Dictionary l_return;

	// Get and returns scan results
	for (auto l_peripheral : m_peripherals) {
		if (l_peripheral.second->get_is_connected()) {
			l_return[l_peripheral.first] = l_peripheral.second->get_address();
		}
	}

	// Return value array
	return l_return;
}

String BluePlug::get_company_name(const int p_company_code) {
	return String(Utils::get_company_name(p_company_code).c_str());
}

String BluePlug::get_status_string(const int p_code) {
	return BLEUtils::get_status_string(static_cast<BLEUtils::Status>(p_code));
}

String BluePlug::get_status_level_string(const int p_code) {
	return BLEUtils::get_status_level_string(static_cast<BLEUtils::StatusLevel>(p_code));
}
