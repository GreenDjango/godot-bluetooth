/*****************************************************************************\
**  blueplug_multiplayer_peer.h
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#ifndef BLUEPLUG_MULTIPLAYER_PEER_H
#define BLUEPLUG_MULTIPLAYER_PEER_H

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>
#include <godot_cpp/templates/hash_map.hpp>

#include "blueplug_packet_peer.h"

namespace godot {

class BluePlugMultiplayerPeer : public MultiplayerPeerExtension {
	GDCLASS(BluePlugMultiplayerPeer, MultiplayerPeerExtension);

private:
    bool is_server = false;
	ConnectionStatus connection_status = CONNECTION_DISCONNECTED;
	int unique_id = 0;

	int target_peer = 0;
	HashMap<int, Ref<BluePlugPacketPeer>> peers_map;

	struct Packet {
		int source_peer = 0;
		uint8_t *data = nullptr;
		uint32_t size = 0;
	};
	Packet current_packet;
	List<Packet> incoming_packets;

	Ref<BluePlugPacketPeer> get_peer(int p_peer) const;
	void poll_server();
	void poll_client();

protected:
	static void _bind_methods();

public:
	Error _get_packet(const uint8_t **r_buffer, int *r_buffer_size) override;
	Error _put_packet(const uint8_t *p_buffer, int p_buffer_size) override;
	int _get_available_packet_count() const override;
	int _get_max_packet_size() const override;

	// PackedByteArray _get_packet_script() override;
	// Error _put_packet_script(const PackedByteArray &p_buffer) override;

	// void _set_transfer_channel(int p_channel) override;
	// int _get_transfer_channel() const override;

	// void _set_transfer_mode(MultiplayerPeer::TransferMode p_mode) override;
	// MultiplayerPeer::TransferMode _get_transfer_mode() const override;

	// void _set_refuse_new_connections(bool p_enable) override;
	// bool _is_refusing_new_connections() const override;

	void _set_target_peer(int p_peer) override;
	
	int _get_packet_peer() const override;
	bool _is_server() const override;
	void _poll() override;
	void _close() override;
	void _disconnect_peer(int p_peer, bool p_force) override;
	int _get_unique_id() const override;
	ConnectionStatus _get_connection_status() const override;

	Error create_server();
	Error create_client();
};

} //namespace godot

#endif // BLUEPLUG_MULTIPLAYER_PEER_H