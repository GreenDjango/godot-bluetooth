/*****************************************************************************\
**  blueplug_multiplayer_peer.cpp
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#include "godot_binding/blueplug_multiplayer_peer.h"

#include <simpleble/Adapter.h>

using namespace godot;

Ref<BluePlugPacketPeer> BluePlugMultiplayerPeer::get_peer(int p_peer) const {
	ERR_FAIL_COND_V(!peers_map.has(p_peer), Ref<BluePlugPacketPeer>());
	return peers_map[p_peer];
}

void BluePlugMultiplayerPeer::poll_server() {
}

void BluePlugMultiplayerPeer::poll_client() {
}

void BluePlugMultiplayerPeer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create_client"), &BluePlugMultiplayerPeer::create_client);
	ClassDB::bind_method(D_METHOD("create_server"), &BluePlugMultiplayerPeer::create_server);
}

Error BluePlugMultiplayerPeer::_get_packet(const uint8_t **r_buffer, int *r_buffer_size) {
	ERR_FAIL_COND_V(_get_connection_status() != CONNECTION_CONNECTED, ERR_UNCONFIGURED);

	*r_buffer_size = 0;

	if (current_packet.data != nullptr) {
		memfree(current_packet.data);
		current_packet.data = nullptr;
	}

	ERR_FAIL_COND_V(incoming_packets.size() == 0, ERR_UNAVAILABLE);

	current_packet = incoming_packets.front()->get();
	incoming_packets.pop_front();

	*r_buffer = current_packet.data;
	*r_buffer_size = current_packet.size;

	return OK;
}

Error BluePlugMultiplayerPeer::_put_packet(const uint8_t *p_buffer, int p_buffer_size) {
	ERR_FAIL_COND_V(_get_connection_status() != CONNECTION_CONNECTED, ERR_UNCONFIGURED);

	if (_is_server()) {
		if (target_peer > 0) {
			ERR_FAIL_COND_V_MSG(!peers_map.has(target_peer), ERR_INVALID_PARAMETER, "Peer not found: " + itos(target_peer));
			get_peer(target_peer)->_put_packet(p_buffer, p_buffer_size);
		} else {
			for (auto &E : peers_map) {
				if (target_peer && -target_peer == E.key) {
					continue; // Excluded.
				}
				E.value->_put_packet(p_buffer, p_buffer_size);
			}
		}
		return OK;
	} else {
		return get_peer(1)->_put_packet(p_buffer, p_buffer_size);
	}
}

int BluePlugMultiplayerPeer::_get_available_packet_count() const {
	return incoming_packets.size();
}

int BluePlugMultiplayerPeer::_get_max_packet_size() const {
	return 1 << 24; // Anything is good
}

void BluePlugMultiplayerPeer::_set_target_peer(int p_peer) {
	target_peer = p_peer;
}

int BluePlugMultiplayerPeer::_get_packet_peer() const {
	ERR_FAIL_COND_V(incoming_packets.size() == 0, 1);

	return incoming_packets.front()->get().source_peer;
}

bool BluePlugMultiplayerPeer::_is_server() const {
	return is_server;
}

void BluePlugMultiplayerPeer::_poll() {
	if (_get_connection_status() == CONNECTION_DISCONNECTED) {
		return;
	}
	if (_is_server()) {
		poll_server();
	} else {
		poll_client();
	}
}

void BluePlugMultiplayerPeer::_close() {
	connection_status = CONNECTION_DISCONNECTED;
	unique_id = 0;
	peers_map.clear();
	// pending_peers.clear();
	if (current_packet.data != nullptr) {
		memfree(current_packet.data);
		current_packet.data = nullptr;
	}
	for (Packet &E : incoming_packets) {
		memfree(E.data);
		E.data = nullptr;
	}
	incoming_packets.clear();
}

void BluePlugMultiplayerPeer::_disconnect_peer(int p_peer_id, bool p_force) {
	ERR_FAIL_COND(!peers_map.has(p_peer_id));
	// peers_map[p_peer_id]->close();
	if (p_force) {
		peers_map.erase(p_peer_id);
		if (!_is_server()) {
			_close();
		}
	}
}

int BluePlugMultiplayerPeer::_get_unique_id() const {
	return unique_id;
}

MultiplayerPeer::ConnectionStatus BluePlugMultiplayerPeer::_get_connection_status() const {
	return connection_status;
}

Error BluePlugMultiplayerPeer::create_server() {
	ERR_FAIL_COND_V(get_connection_status() != CONNECTION_DISCONNECTED, ERR_ALREADY_IN_USE);
	_close();
	is_server = true;

	ERR_FAIL_COND_V(SimpleBLE::Adapter::bluetooth_enabled() == false, ERR_CANT_RESOLVE);
	auto adapters = SimpleBLE::Adapter::get_adapters();
	ERR_FAIL_COND_V(adapters.empty() == true, ERR_CANT_ACQUIRE_RESOURCE);

	// adapters.front()

	// tcp_server.instantiate();
	// Error err = tcp_server->listen(p_port, p_bind_ip);
	// if (err != OK) {
	// 	tcp_server.unref();
	// 	return err;
	// }

	unique_id = 1;
	connection_status = CONNECTION_CONNECTED;
	return OK;
}

Error BluePlugMultiplayerPeer::create_client() {
	ERR_FAIL_COND_V(get_connection_status() != CONNECTION_DISCONNECTED, ERR_ALREADY_IN_USE);
	_close();
	is_server = false;
	// Ref<WebSocketPeer> peer = _create_peer();
	// Error err = peer->connect_to_url(p_url, p_options);
	// if (err != OK) {
	// 	return err;
	// }
	// PendingPeer pending;
	// pending.time = OS::get_singleton()->get_ticks_msec();
	// pending_peers[1] = pending;
	// peers_map[1] = peer;
	connection_status = CONNECTION_CONNECTING;
	return OK;
}
