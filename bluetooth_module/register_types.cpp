/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** register_types.cpp
*/

#include "register_types.h"
//#include "core/error_macros.h"
#include "src/networked_multiplayer_bt.h"

static bool btm_ok = false;

void register_btm_types() {
	if (enet_initialize() != 0) {
		//ERR_PRINT("ENet initialization failure");
	} else {
		btm_ok = true;
	}

	ClassDB::register_class<NetworkedMultiplayerENet>();
}

void unregister_btm_types() {
	if (btm_ok) {
		enet_deinitialize();
	}
}
