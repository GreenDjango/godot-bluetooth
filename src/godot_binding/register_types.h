/*****************************************************************************\
**  register_types.h
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#ifndef BLUEPLUG_REGISTER_TYPES_H
#define BLUEPLUG_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_blueplug_module(ModuleInitializationLevel p_level);
void uninitialize_blueplug_module(ModuleInitializationLevel p_level);

#endif // BLUEPLUG_REGISTER_TYPES_H