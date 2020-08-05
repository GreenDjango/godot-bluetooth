/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** networked_multiplayer_bt.h
*/

#ifndef NETWORKED_MULTIPLAYER_BT_H
#define NETWORKED_MULTIPLAYER_BT_H

void register_btm_types();
void unregister_btm_types();

/*#include "core/crypto/crypto.h"
#include "core/io/compression.h"
#include "core/io/networked_multiplayer_peer.h"

#include <enet/enet.h>

class NetworkedMultiplayerENet : public NetworkedMultiplayerPeer {
	GDCLASS(NetworkedMultiplayerENet, NetworkedMultiplayerPeer);

public:
	enum CompressionMode {
		COMPRESS_NONE,
		COMPRESS_RANGE_CODER,
		COMPRESS_FASTLZ,
		COMPRESS_ZLIB,
		COMPRESS_ZSTD
	};

private:
	enum {
		SYSMSG_ADD_PEER,
		SYSMSG_REMOVE_PEER
	};

	enum {
		SYSCH_CONFIG,
		SYSCH_RELIABLE,
		SYSCH_UNRELIABLE,
		SYSCH_MAX
	};

	bool active;
	bool server;

	uint32_t unique_id;

	int target_peer;
	TransferMode transfer_mode;
	int transfer_channel;
	int channel_count;
	bool always_ordered;

	ENetEvent event;
	ENetPeer *peer;
	ENetHost *host;

	bool refuse_connections;
	bool server_relay;

	ConnectionStatus connection_status;

	Map<int, ENetPeer *> peer_map;

	struct Packet {
		ENetPacket *packet;
		int from;
		int channel;
	};

	CompressionMode compression_mode;

	List<Packet> incoming_packets;

	Packet current_packet;

	uint32_t _gen_unique_id() const;
	void _pop_current_packet();

	Vector<uint8_t> src_compressor_mem;
	Vector<uint8_t> dst_compressor_mem;

	ENetCompressor enet_compressor;
	static size_t enet_compress(void *context, const ENetBuffer *inBuffers, size_t inBufferCount, size_t inLimit, enet_uint8 *outData, size_t outLimit);
	static size_t enet_decompress(void *context, const enet_uint8 *inData, size_t inLimit, enet_uint8 *outData, size_t outLimit);
	static void enet_compressor_destroy(void *context);
	void _setup_compressor();

	IP_Address bind_ip;

	bool dtls_enabled;
	Ref<CryptoKey> dtls_key;
	Ref<X509Certificate> dtls_cert;
	bool dtls_verify;

protected:
	static void _bind_methods();

public:
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

	virtual int get_unique_id() const override;

	void set_compression_mode(CompressionMode p_mode);
	CompressionMode get_compression_mode() const;

	int get_packet_channel() const;
	int get_last_packet_channel() const;
	void set_transfer_channel(int p_channel);
	int get_transfer_channel() const;
	void set_channel_count(int p_channel);
	int get_channel_count() const;
	void set_always_ordered(bool p_ordered);
	bool is_always_ordered() const;
	void set_server_relay_enabled(bool p_enabled);
	bool is_server_relay_enabled() const;

	NetworkedMultiplayerENet();
	~NetworkedMultiplayerENet();

	void set_bind_ip(const IP_Address &p_ip);
	void set_dtls_enabled(bool p_enabled);
	bool is_dtls_enabled() const;
	void set_dtls_verify_enabled(bool p_enabled);
	bool is_dtls_verify_enabled() const;
	void set_dtls_key(Ref<CryptoKey> p_key);
	void set_dtls_certificate(Ref<X509Certificate> p_cert);
};

VARIANT_ENUM_CAST(NetworkedMultiplayerENet::CompressionMode);
*/
#endif // NETWORKED_MULTIPLAYER_BT_H
