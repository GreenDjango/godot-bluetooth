/*****************************************************************************\
**  utils.cpp
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#include "utils.h"
#include "company_identfiers.h"

#include <simpleble/Adapter.h>

std::vector<std::string> *Utils::company_identifiers = &_company_identifiers;

const std::vector<std::string> Utils::get_adapters_addresses() {
	auto adapters = SimpleBLE::Adapter::get_adapters();
	std::vector<std::string> addresses;

	for (auto &adapter : adapters) {
		addresses.push_back(adapter.address());
	}
	return addresses;
}

const std::string Utils::get_company_name(const int p_code) {
	try {
		const auto &l_string = company_identifiers->at(p_code);
		return l_string;
	} catch (...) {
		return "UNKNOWN";
	}
}