/*****************************************************************************\
**  core.h
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#ifndef BLUEPLUG_CORE_H
#define BLUEPLUG_CORE_H

#include <simpleble/Adapter.h>

class Core {
private:
	SimpleBLE::Adapter *current_adapter;

public:
	void init(const std::string address);
};

#endif // BLUEPLUG_CORE_H
