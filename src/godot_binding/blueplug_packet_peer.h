/*****************************************************************************\
**  blueplug_packet_peer.h
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#ifndef BLUEPLUG_PACKET_PEER_H
#define BLUEPLUG_PACKET_PEER_H

#include <godot_cpp/classes/multiplayer_peer.hpp>
#include <godot_cpp/classes/packet_peer.hpp>

namespace godot {

class BluePlugPacketPeer : public PacketPeer {
	GDCLASS(BluePlugPacketPeer, PacketPeer);

protected:
	static void _bind_methods();

// public:
// 	BluePlugPacketPeer();
// 	~BluePlugPacketPeer();
};

} //namespace godot

#endif // BLUEPLUG_PACKET_PEER_H