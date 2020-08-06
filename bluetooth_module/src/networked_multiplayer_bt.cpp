/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** networked_multiplayer_bt.cpp
*/

#include "networked_multiplayer_bt.h"
#include "moc_networked_multiplayer_bt.cpp"
#include "core/os/os.h"

int enet_initialize(void) {
	return 0;
}

void enet_deinitialize(void) {
}

//TODO: remove
/*void NetworkedMultiplayerBt::startDeviceDiscovery()
{

    // Create a discovery agent and connect to its signals
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

    // Start a discovery
    discoveryAgent->start();

    //...
}
void NetworkedMultiplayerBt::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
}*/

void NetworkedMultiplayerBt::set_transfer_mode(TransferMode p_mode) {

	transfer_mode = p_mode;
}

NetworkedMultiplayerPeer::TransferMode NetworkedMultiplayerBt::get_transfer_mode() const {

	return transfer_mode;
}

void NetworkedMultiplayerBt::set_target_peer(int p_peer) {

	target_peer = p_peer;
}

int NetworkedMultiplayerBt::get_packet_peer() const {

	ERR_FAIL_COND_V_MSG(!active, 1, "The multiplayer instance isn't currently active.");
	ERR_FAIL_COND_V(incoming_packets.size() == 0, 1);

	return incoming_packets.front()->get().from;
}

Error NetworkedMultiplayerBt::create_server(int p_port, int p_max_clients, int p_in_bandwidth, int p_out_bandwidth) {

	ERR_FAIL_COND_V_MSG(active, ERR_ALREADY_IN_USE, "The multiplayer instance is already active.");
	ERR_FAIL_COND_V_MSG(p_port < 0 || p_port > 65535, ERR_INVALID_PARAMETER, "The port number must be set between 0 and 65535 (inclusive).");
	ERR_FAIL_COND_V_MSG(p_max_clients < 1 || p_max_clients > 4095, ERR_INVALID_PARAMETER, "The number of clients must be set between 1 and 4095 (inclusive).");
	ERR_FAIL_COND_V_MSG(p_in_bandwidth < 0, ERR_INVALID_PARAMETER, "The incoming bandwidth limit must be greater than or equal to 0 (0 disables the limit).");
	ERR_FAIL_COND_V_MSG(p_out_bandwidth < 0, ERR_INVALID_PARAMETER, "The outgoing bandwidth limit must be greater than or equal to 0 (0 disables the limit).");

	//TODO: Start SERVER

	active = true;
	server = true;
	refuse_connections = false;
	unique_id = 1;
	connection_status = CONNECTION_CONNECTED;
	return OK;
}

Error NetworkedMultiplayerBt::create_client(const String &p_address, int p_port, int p_in_bandwidth, int p_out_bandwidth, int p_client_port) {

	ERR_FAIL_COND_V_MSG(active, ERR_ALREADY_IN_USE, "The multiplayer instance is already active.");
	ERR_FAIL_COND_V_MSG(p_port < 0 || p_port > 65535, ERR_INVALID_PARAMETER, "The server port number must be set between 0 and 65535 (inclusive).");
	ERR_FAIL_COND_V_MSG(p_client_port < 0 || p_client_port > 65535, ERR_INVALID_PARAMETER, "The client port number must be set between 0 and 65535 (inclusive).");
	ERR_FAIL_COND_V_MSG(p_in_bandwidth < 0, ERR_INVALID_PARAMETER, "The incoming bandwidth limit must be greater than or equal to 0 (0 disables the limit).");
	ERR_FAIL_COND_V_MSG(p_out_bandwidth < 0, ERR_INVALID_PARAMETER, "The outgoing bandwidth limit must be greater than or equal to 0 (0 disables the limit).");

	//TODO: start CLIENT

	connection_status = CONNECTION_CONNECTING;
	active = true;
	server = false;
	refuse_connections = false;
	return OK;
}

void NetworkedMultiplayerBt::poll() {

	ERR_FAIL_COND_MSG(!active, "The multiplayer instance isn't currently active.");
	_pop_current_packet();
}

bool NetworkedMultiplayerBt::is_server() const {
	ERR_FAIL_COND_V_MSG(!active, false, "The multiplayer instance isn't currently active.");

	return server;
}

void NetworkedMultiplayerBt::close_connection(uint32_t wait_usec) {

	ERR_FAIL_COND_MSG(!active, "The multiplayer instance isn't currently active.");

	_pop_current_packet();

	bool peers_disconnected = false;
	for (Map<int, String *>::Element *E = peer_map.front(); E; E = E->next()) {
		if (E->get()) {
			//enet_peer_disconnect_now(E->get(), unique_id);
			//int *id = (int *)(E->get()->data);
			//memdelete(id);
			peers_disconnected = true;
		}
	}

	if (peers_disconnected) {
		//enet_host_flush(host);

		if (wait_usec > 0) {
			OS::get_singleton()->delay_usec(wait_usec); // Wait for disconnection packets to send
		}
	}

	//enet_host_destroy(host);
	active = false;
	incoming_packets.clear();
	peer_map.clear();
	unique_id = 1; // Server is 1
	connection_status = CONNECTION_DISCONNECTED;
}

void NetworkedMultiplayerBt::disconnect_peer(int p_peer, bool now) {

	ERR_FAIL_COND_MSG(!active, "The multiplayer instance isn't currently active.");
	ERR_FAIL_COND_MSG(!is_server(), "Can't disconnect a peer when not acting as a server.");
	ERR_FAIL_COND_MSG(!peer_map.has(p_peer), vformat("Peer ID %d not found in the list of peers.", p_peer));

	if (now) {
		//int *id = (int *)peer_map[p_peer]->data;
		//enet_peer_disconnect_now(peer_map[p_peer], 0);

		// enet_peer_disconnect_now doesn't generate ENET_EVENT_TYPE_DISCONNECT,
		// notify everyone else, send disconnect signal & remove from peer_map like in poll()
		if (server_relay) {
			for (Map<int, String *>::Element *E = peer_map.front(); E; E = E->next()) {

				if (E->key() == p_peer) {
					continue;
				}

				/*ENetPacket *packet = enet_packet_create(NULL, 8, ENET_PACKET_FLAG_RELIABLE);
				encode_uint32(SYSMSG_REMOVE_PEER, &packet->data[0]);
				encode_uint32(p_peer, &packet->data[4]);
				enet_peer_send(E->get(), SYSCH_CONFIG, packet);*/
			}
		}

		//if (id)
		//	memdelete(id);

		emit_signal("peer_disconnected", p_peer);
		peer_map.erase(p_peer);
	} else {
		//enet_peer_disconnect_later(peer_map[p_peer], 0);
	}
}

int NetworkedMultiplayerBt::get_available_packet_count() const {

	return incoming_packets.size();
}

Error NetworkedMultiplayerBt::get_packet(const uint8_t **r_buffer, int &r_buffer_size) {

	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, ERR_UNAVAILABLE, "No incoming packets available.");

	_pop_current_packet();

	current_packet = incoming_packets.front()->get();
	incoming_packets.pop_front();

	//*r_buffer = (const uint8_t *)(&current_packet.packet->data[8]);
	//r_buffer_size = current_packet.packet->dataLength - 8;

	return OK;
}

Error NetworkedMultiplayerBt::put_packet(const uint8_t *p_buffer, int p_buffer_size) {

	ERR_FAIL_COND_V_MSG(!active, ERR_UNCONFIGURED, "The multiplayer instance isn't currently active.");
	ERR_FAIL_COND_V_MSG(connection_status != CONNECTION_CONNECTED, ERR_UNCONFIGURED, "The multiplayer instance isn't currently connected to any server or client.");

	//int packet_flags = 0;
	//int channel = SYSCH_RELIABLE;

	/*switch (transfer_mode) {
		case TRANSFER_MODE_UNRELIABLE: {
			if (always_ordered)
				packet_flags = 0;
			else
				packet_flags = ENET_PACKET_FLAG_UNSEQUENCED;
			channel = SYSCH_UNRELIABLE;
		} break;
		case TRANSFER_MODE_UNRELIABLE_ORDERED: {
			packet_flags = 0;
			channel = SYSCH_UNRELIABLE;
		} break;
		case TRANSFER_MODE_RELIABLE: {
			packet_flags = ENET_PACKET_FLAG_RELIABLE;
			channel = SYSCH_RELIABLE;
		} break;
	}

	if (transfer_channel > SYSCH_CONFIG)
		channel = transfer_channel;

	Map<int, ENetPeer *>::Element *E = NULL;

	if (target_peer != 0) {

		E = peer_map.find(ABS(target_peer));
		ERR_FAIL_COND_V_MSG(!E, ERR_INVALID_PARAMETER, vformat("Invalid target peer: %d", target_peer));
	}

	ENetPacket *packet = enet_packet_create(NULL, p_buffer_size + 8, packet_flags);
	encode_uint32(unique_id, &packet->data[0]); // Source ID
	encode_uint32(target_peer, &packet->data[4]); // Dest ID
	copymem(&packet->data[8], p_buffer, p_buffer_size);

	if (server) {

		if (target_peer == 0) {
			enet_host_broadcast(host, channel, packet);
		} else if (target_peer < 0) {
			// Send to all but one
			// and make copies for sending

			int exclude = -target_peer;

			for (Map<int, String *>::Element *F = peer_map.front(); F; F = F->next()) {

				if (F->key() == exclude) // Exclude packet
					continue;

				ENetPacket *packet2 = enet_packet_create(packet->data, packet->dataLength, packet_flags);

				enet_peer_send(F->get(), channel, packet2);
			}

			enet_packet_destroy(packet); // Original packet no longer needed
		} else {
			enet_peer_send(E->get(), channel, packet);
		}
	} else {

		ERR_FAIL_COND_V(!peer_map.has(1), ERR_BUG);
		enet_peer_send(peer_map[1], channel, packet); // Send to server for broadcast
	}

	enet_host_flush(host);*/

	return OK;
}

int NetworkedMultiplayerBt::get_max_packet_size() const {

	return 1 << 24; // Anything is good
}

void NetworkedMultiplayerBt::_pop_current_packet() {

	if (current_packet.packet) {
		//TODO: enet_packet_destroy(current_packet.packet);
		current_packet.packet = NULL;
		current_packet.from = 0;
		current_packet.channel = -1;
	}
}

NetworkedMultiplayerPeer::ConnectionStatus NetworkedMultiplayerBt::get_connection_status() const {

	return connection_status;
}

uint32_t NetworkedMultiplayerBt::_gen_unique_id() const {

	uint32_t hash = 0;

	while (hash == 0 || hash == 1) {

		hash = hash_djb2_one_32(
				(uint32_t)OS::get_singleton()->get_ticks_usec());
		hash = hash_djb2_one_32(
				(uint32_t)OS::get_singleton()->get_unix_time(), hash);
		hash = hash_djb2_one_32(
				(uint32_t)OS::get_singleton()->get_user_data_dir().hash64(), hash);
		hash = hash_djb2_one_32(
				(uint32_t)((uint64_t)this), hash); // Rely on ASLR heap
		hash = hash_djb2_one_32(
				(uint32_t)((uint64_t)&hash), hash); // Rely on ASLR stack

		hash = hash & 0x7FFFFFFF; // Make it compatible with unsigned, since negative ID is used for exclusion
	}

	return hash;
}

int NetworkedMultiplayerBt::get_unique_id() const {

	ERR_FAIL_COND_V_MSG(!active, 0, "The multiplayer instance isn't currently active.");
	return unique_id;
}

void NetworkedMultiplayerBt::set_refuse_new_connections(bool p_enable) {

	refuse_connections = p_enable;
}

bool NetworkedMultiplayerBt::is_refusing_new_connections() const {

	return refuse_connections;
}

IP_Address NetworkedMultiplayerBt::get_peer_address(int p_peer_id) const {

	ERR_FAIL_COND_V_MSG(!peer_map.has(p_peer_id), IP_Address(), vformat("Peer ID %d not found in the list of peers.", p_peer_id));
	ERR_FAIL_COND_V_MSG(!is_server() && p_peer_id != 1, IP_Address(), "Can't get the address of peers other than the server (ID -1) when acting as a client.");
	ERR_FAIL_COND_V_MSG(peer_map[p_peer_id] == NULL, IP_Address(), vformat("Peer ID %d found in the list of peers, but is null.", p_peer_id));

	IP_Address out{};
	//out.set_ipv4((uint8_t *)&(peer_map[p_peer_id]->address.host));

	return out;
}

int NetworkedMultiplayerBt::get_peer_port(int p_peer_id) const {

	ERR_FAIL_COND_V_MSG(!peer_map.has(p_peer_id), 0, vformat("Peer ID %d not found in the list of peers.", p_peer_id));
	ERR_FAIL_COND_V_MSG(!is_server() && p_peer_id != 1, 0, "Can't get the address of peers other than the server (ID -1) when acting as a client.");
	ERR_FAIL_COND_V_MSG(peer_map[p_peer_id] == NULL, 0, vformat("Peer ID %d found in the list of peers, but is null.", p_peer_id));
	//return peer_map[p_peer_id]->address.port;
	return 0;
}

void NetworkedMultiplayerBt::set_server_relay_enabled(bool p_enabled) {
	ERR_FAIL_COND_MSG(active, "Server relaying can't be toggled while the multiplayer instance is active.");

	server_relay = p_enabled;
}

bool NetworkedMultiplayerBt::is_server_relay_enabled() const {
	return server_relay;
}

void NetworkedMultiplayerBt::_bind_methods() {

	ClassDB::bind_method(D_METHOD("create_server", "port", "max_clients", "in_bandwidth", "out_bandwidth"), &NetworkedMultiplayerBt::create_server, DEFVAL(32), DEFVAL(0), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("create_client", "address", "port", "in_bandwidth", "out_bandwidth", "client_port"), &NetworkedMultiplayerBt::create_client, DEFVAL(0), DEFVAL(0), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("close_connection", "wait_usec"), &NetworkedMultiplayerBt::close_connection, DEFVAL(100));
	ClassDB::bind_method(D_METHOD("disconnect_peer", "id", "now"), &NetworkedMultiplayerBt::disconnect_peer, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("get_peer_address", "id"), &NetworkedMultiplayerBt::get_peer_address);
	ClassDB::bind_method(D_METHOD("get_peer_port", "id"), &NetworkedMultiplayerBt::get_peer_port);

	ClassDB::bind_method(D_METHOD("set_server_relay_enabled", "enabled"), &NetworkedMultiplayerBt::set_server_relay_enabled);
	ClassDB::bind_method(D_METHOD("is_server_relay_enabled"), &NetworkedMultiplayerBt::is_server_relay_enabled);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "server_relay"), "set_server_relay_enabled", "is_server_relay_enabled");
}

NetworkedMultiplayerBt::NetworkedMultiplayerBt() {
	//_argv = ((char **)calloc(1, sizeof(char *)));
	//_argv[0] = ((char *)calloc(20, sizeof(char)));
	//strcpy(_argv[0], "BluetoothModule");
	//app = new QApplication(_argc, _argv);

	active = false;
	server = false;
	refuse_connections = false;
	server_relay = true;
	unique_id = 0;
	target_peer = 0;
	current_packet.packet = NULL;
	transfer_mode = TRANSFER_MODE_RELIABLE;
	connection_status = CONNECTION_DISCONNECTED;

	//app.exec();
}

NetworkedMultiplayerBt::~NetworkedMultiplayerBt() {

	if (active) {
		close_connection();
	}
}