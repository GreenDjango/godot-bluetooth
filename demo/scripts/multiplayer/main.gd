extends Node

var blueplug = BluePlug.new()
var peer := ENetMultiplayerPeer.new()
var hostname := "host"
var players = {}
const player_scene := preload("res://scenes/multiplayer/airplane.tscn")
@onready var game := $Game

func _init():
	blueplug.init_adapter(blueplug.adapters()[0])
	blueplug.peripheral_found.connect(self._on_peripheral_found)
	if OS.has_environment("USER"):
		hostname = OS.get_environment("USER")

func _ready():
	game.multiplayer.peer_connected.connect(self._peer_connected)
	game.multiplayer.peer_disconnected.connect(self._peer_disconnected)
	game.multiplayer.server_disconnected.connect(self._close_network)
	game.multiplayer.connection_failed.connect(self._close_network)
	game.multiplayer.connected_to_server.connect(self._connected)

func _close_network():
	print("close_network")
	game.multiplayer.multiplayer_peer = null
	peer.close()

func _connected():
	print("connected")

func _peer_connected(id):
	print("peer_connected %d" % id)
	register_player.rpc_id(id, hostname)

func _peer_disconnected(id):
	print("Disconnected %d" % id)

func _on_start_server_pressed():
	$CanvasLayer/Control/ServerPanel.visible = true
	$CanvasLayer/Control/ServerPanel/RichTextLabel.text = ""
	game.multiplayer.multiplayer_peer = null
	peer.create_server(5080)
	game.multiplayer.multiplayer_peer = peer

func _on_start_client_pressed():
	$CanvasLayer/Control/ScanList.visible = true
	$CanvasLayer/Control/ScanList.clear()
	blueplug.start_scan()

func _on_peripheral_found(p_address: String):
	$CanvasLayer/Control/ScanList.add_item("%s (%s)" % [blueplug.peripheral_identifier(p_address), p_address])
	$CanvasLayer/Control/ScanList.sort_items_by_text()

func _on_scan_list_item_activated(_index: int):
	blueplug.stop_scan()
	game.multiplayer.multiplayer_peer = null
	peer.create_client("localhost", 5080)
	game.multiplayer.multiplayer_peer = peer

func _on_start_game_pressed():
	load_world.rpc()

	var spawn_points = {1: Vector2(randf_range(-16, 16), randf_range(-16, 16))}
	for p_id in players:
		spawn_points[p_id] = Vector2(randf_range(-16, 16), randf_range(-16, 16))

	for p_id in spawn_points:
		if p_id == 1:
			$Game/airplane.position = Vector3(spawn_points[p_id].x, 0, spawn_points[p_id].y)
			$Game/airplane.player_id = p_id
			$Game/airplane/Label3D.text = "You"
		else:
			var player = player_scene.instantiate()
			player.synced_position = Vector3(spawn_points[p_id].x, 0, spawn_points[p_id].y)
			player.player_id = p_id
			player.player_name = players[p_id]
			$Game/Players.add_child(player, true)
		# player.set_player_name(player_name if p_id == multiplayer.get_unique_id() else players[p_id])

@rpc("any_peer")
func register_player(new_player_name: String):
	print("new player: %s" % new_player_name)
	var id = multiplayer.get_remote_sender_id()
	players[id] = new_player_name
	$CanvasLayer/Control/ServerPanel/RichTextLabel.append_text("[%s] [b]%s[/b]\n" % [id, new_player_name])

@rpc("call_local")
func load_world():
	$CanvasLayer/Control/Menu.visible = false
	$CanvasLayer/Control/ScanList.visible = false
	$CanvasLayer/Control/ServerPanel.visible = false
	$Game/airplane/AnimationPlayer.stop()
	# get_tree().set_pause(false)
