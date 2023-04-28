class_name Airplane
extends RigidBody3D

@export
var synced_position := Vector3()
@export
var player_name := ""
@export
var player_id := -1

const min_flight_speed := 10
const max_flight_speed := 30
const turn_speed := 0.75
const pitch_speed := 0.5
const level_speed := 3.0
const throttle_delta := 200
const acceleration := 6.0

var forward_speed := 0
var target_speed := 0
var grounded := false

var turn_input := 0
var pitch_input := 0

func _ready():
	position = synced_position
	$Label3D.text = player_name
	#if player_id != -1:
	#	$MultiplayerSynchronizer.set_multiplayer_authority(player_id)

func _physics_process(delta: float):
	if multiplayer.multiplayer_peer == null or multiplayer.get_unique_id() == player_id:
		# The client which this player represent will update the controls state, and notify it to everyone.
		# inputs.update()
		pass

#	if multiplayer.multiplayer_peer == null or is_multiplayer_authority():
#		# The server updates the position that will be notified to the clients.
#		synced_position = position
#	else:
#		# The client simply updates the position to the last known one.
#		position = synced_position

	# Throttle input
	if Input.is_action_pressed("player_space"):
		target_speed = min(forward_speed + throttle_delta * delta, max_flight_speed)
	else:
		var limit = 0 if grounded else min_flight_speed
		target_speed = max(forward_speed - throttle_delta * delta, limit)
	# Turn (roll/yaw) input
	turn_input = 0
	if forward_speed > 0.5:
		turn_input -= Input.get_action_strength("player_right")
		turn_input += Input.get_action_strength("player_left")
	# Pitch (climb/dive) input
	pitch_input = 0
	if not grounded:
		pitch_input -= Input.get_action_strength("player_down")
	if forward_speed >= min_flight_speed:
		pitch_input += Input.get_action_strength("player_up")
	_move_player(delta)


func _move_player(_delta: float):
	apply_central_force(Vector3.FORWARD)
