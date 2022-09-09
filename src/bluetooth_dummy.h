/*****************************************************************************\
**  bluetooth_dummy.h
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2022 - Godot Bluetooth Module contributors
\*****************************************************************************/

#if !defined(BLUETOOTH_WRAPPER_DUMMY_H)
#define BLUETOOTH_WRAPPER_DUMMY_H

#include <map>
#include <string>

struct DeviceInfo {
	std::string address;
	std::string address_type;
	std::string name;
	std::string alias;
	std::string icon;
	bool paired;
	bool trusted;
	bool blocked;
	bool legacy_pairing;
	bool connected;

	DeviceInfo() {
		address = "",
		address_type = "",
		alias = "",
		blocked = false,
		connected = false,
		icon = "",
		legacy_pairing = false,
		name = "",
		paired = false,
		trusted = false;
	}
};

class BluetoothWrapperDummy {
public:
	virtual ~BluetoothWrapperDummy();

	virtual std::map<std::string, DeviceInfo> list_devices() const;

	virtual void remove_device(const std::string &device_address) const;
	virtual void start_discovery() const;
	virtual void stop_discovery() const;

	virtual std::string address() const;
	virtual std::string address_type() const;
	virtual void set_alias(const std::string &alias) const;
	virtual std::string get_alias() const;
	// int device_class() const;
	virtual void set_discoverable(const bool enable) const;
	virtual bool is_discoverable() const;
	// void set_discoverable_timeout(const int timeout) const;
	// int get_discoverable_timeout() const;
	virtual bool discovering() const;
	virtual std::string modalias() const;
	virtual std::string name() const;
	virtual void set_pairable(const bool enable) const;
	virtual bool is_pairable() const;
	// void set_pairable_timeout(const int timeout) const;
	// int get_pairable_timeout() const;
	virtual void set_powered(const bool enable) const;
	virtual bool is_powered() const;
	// std::vector<std::string> roles() const;
	// std::vector<std::string> uuids() const;

	virtual bool is_dummy() const;
};

// typedef std::unique_ptr<Resource> ResourcePtr;

#endif // BLUETOOTH_WRAPPER_DUMMY_H