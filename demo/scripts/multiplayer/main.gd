extends Control

var peer := BluePlugMultiplayerPeer.new()
var hostname := "host"

func _init():
	if OS.has_environment("USERNAME"):
		hostname = OS.get_environment("USERNAME")

func _ready():
	multiplayer.peer_connected.connect(self._peer_connected)
	multiplayer.peer_disconnected.connect(self._peer_disconnected)
	multiplayer.server_disconnected.connect(self._close_network)
	multiplayer.connection_failed.connect(self._close_network)
	multiplayer.connected_to_server.connect(self._connected)

func _close_network():
	print("close_network")
	multiplayer.multiplayer_peer = null
	peer.close()

func _connected():
	print("connected")

func _peer_connected(id):
	print("peer_connected %d" % id)

func _peer_disconnected(id):
	print("Disconnected %d" % id)

func _on_start_server_pressed():
	multiplayer.multiplayer_peer = null
	peer.create_server()
	multiplayer.multiplayer_peer = peer


func _on_start_client_pressed():
	multiplayer.multiplayer_peer = null
	peer.create_client()
	multiplayer.multiplayer_peer = peer
