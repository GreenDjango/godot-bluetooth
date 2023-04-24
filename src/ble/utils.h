/*****************************************************************************\
**  utils.h
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#ifndef BLUEPLUG_UTILS_H
#define BLUEPLUG_UTILS_H

#include <string>
#include <vector>

class Utils {
private:
	/**
	 * Company names
	 */
	static std::vector<std::string> *company_identifiers;

public:
	static const std::vector<std::string> get_adapters_addresses();
	static const std::string get_company_name(const int p_code);
};

#endif // BLUEPLUG_UTILS_H
