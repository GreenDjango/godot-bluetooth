/*****************************************************************************\
**  gdexample.h
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/sprite2d.hpp>

namespace godot {

class GDExample : public Sprite2D {
    GDCLASS(GDExample, Sprite2D)

private:
    float time_passed;

protected:
    static void _bind_methods();

public:
    GDExample();
    ~GDExample();

    void _process(float delta);
};

}

#endif