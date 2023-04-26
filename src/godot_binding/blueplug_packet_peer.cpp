/*****************************************************************************\
**  blueplug_packet_peer.cpp
**
**  This file is part of the Godot Bluetooth Module project, and is made
**  available under the terms of the MIT License version.
**
**  Copyright (C) 2023 - Godot Bluetooth Module contributors
\*****************************************************************************/

#include "blueplug_packet_peer.h"

using namespace godot;

void BluePlugPacketPeer::_bind_methods() {
}

// int32_t BluePlugPacketPeer::_get_available_packet_count() const {
// 	return 0;
// }

// int32_t BluePlugPacketPeer::_get_max_packet_size() const {
// 	// https://stackoverflow.com/questions/38913743/maximum-packet-length-for-bluetooth-le
// 	return 20;
// }

Error BluePlugPacketPeer::_get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) try {
	// ERR_FAIL_COND_V(!peer_connection, ERR_UNCONFIGURED);
	// ERR_FAIL_COND_V(_get_connection_state() != STATE_NEW, FAILED);
	// peer_connection->setLocalDescription(rtc::Description::Type::Offer);
	return OK;
} catch (const std::exception &e) {
	ERR_PRINT(e.what());
	ERR_FAIL_V(FAILED);
}

Error BluePlugPacketPeer::_put_packet(const uint8_t *p_buffer, int32_t p_buffer_size) try {
	return OK;
} catch (const std::exception &e) {
	ERR_PRINT(e.what());
	ERR_FAIL_V(FAILED);
}
