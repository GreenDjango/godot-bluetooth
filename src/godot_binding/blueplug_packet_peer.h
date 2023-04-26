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

#include <godot_cpp/classes/packet_peer_extension.hpp>

namespace godot {

class BluePlugPacketPeer : public PacketPeerExtension {
	GDCLASS(BluePlugPacketPeer, PacketPeerExtension);

protected:
	static void _bind_methods();

public:
	// int32_t _get_available_packet_count() const override;
	// int32_t _get_max_packet_size() const override;
	Error _get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) override;
	Error _put_packet(const uint8_t *p_buffer, int32_t p_buffer_size) override;
};

} //namespace godot

#endif // BLUEPLUG_PACKET_PEER_H