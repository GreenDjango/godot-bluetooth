/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** networked_multiplayer_bt.h
*/

#ifndef NETWORKED_MULTIPLAYER_BT_H
#define NETWORKED_MULTIPLAYER_BT_H

#include "core/io/networked_multiplayer_peer.h"
#include "BluetoothModule.h"

int enet_initialize(void);
void enet_deinitialize(void);

/*#include "core/crypto/crypto.h"
#include "core/io/compression.h"

sudo setcap 'cap_net_raw,cap_net_admin+eip' ./bin/godot.x11.tools.64

#include <enet/enet.h>
*/
class NetworkedMultiplayerBt : public NetworkedMultiplayerPeer {
	GDCLASS(NetworkedMultiplayerBt, NetworkedMultiplayerPeer);

private:
	BluetoothModule module;
	bool active;
	bool server;

	int target_peer;
	TransferMode transfer_mode;
	bool refuse_connections;
	bool server_relay;
	ConnectionStatus connection_status;
	uint32_t unique_id;

	Map<int, String *> peer_map;

	struct Packet {
		String *packet;
		int from;
		int channel;
	};
	List<Packet> incoming_packets;
	Packet current_packet;
	uint32_t _gen_unique_id() const;
	void _pop_current_packet();

protected:
	static void _bind_methods();

public:
	//TODO: remove
	//void startDeviceDiscovery();
	//void deviceDiscovered();
	virtual void set_transfer_mode(TransferMode p_mode) override;
	virtual TransferMode get_transfer_mode() const override;
	virtual void set_target_peer(int p_peer) override;

	virtual int get_packet_peer() const override;

	virtual IP_Address get_peer_address(int p_peer_id) const;
	virtual int get_peer_port(int p_peer_id) const;

	Error create_server(int p_port, int p_max_clients = 32, int p_in_bandwidth = 0, int p_out_bandwidth = 0);
	Error create_client(const String &p_address, int p_port, int p_in_bandwidth = 0, int p_out_bandwidth = 0, int p_client_port = 0);

	void close_connection(uint32_t wait_usec = 100);

	void disconnect_peer(int p_peer, bool now = false);

	virtual void poll() override;

	virtual bool is_server() const override;

	virtual int get_available_packet_count() const override;
	virtual Error get_packet(const uint8_t **r_buffer, int &r_buffer_size) override; ///< buffer is GONE after next get_packet
	virtual Error put_packet(const uint8_t *p_buffer, int p_buffer_size) override;

	virtual int get_max_packet_size() const override;

	virtual ConnectionStatus get_connection_status() const override;

	virtual void set_refuse_new_connections(bool p_enable) override;
	virtual bool is_refusing_new_connections() const override;
	void set_server_relay_enabled(bool p_enabled);
	bool is_server_relay_enabled() const;

	virtual int get_unique_id() const override;

	NetworkedMultiplayerBt();
	~NetworkedMultiplayerBt();
};

#endif // NETWORKED_MULTIPLAYER_BT_H
