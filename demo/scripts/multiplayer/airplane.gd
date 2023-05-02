class_name Airplane
extends RigidBody3D

@export
var synced_position := Vector3()
@export
var player_name := ""
@export
var player_id := -1

const max_flight_speed := 0.6
const throttle_delta := 800000.0
var current_speed := .0
var turn_input := .0

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
	var level_speed := .0
	# Throttle input
	if Input.is_action_pressed("player_space"):
		level_speed = throttle_delta * delta
	# Turn (roll/yaw) input
	turn_input = 0
	turn_input -= Input.get_action_strength("player_right")
	turn_input += Input.get_action_strength("player_left")
	rotate_y(turn_input * delta)
	# Pitch (climb/dive) input
	var target_speed := .0
	if Input.is_action_pressed("player_down"):
		target_speed += max_flight_speed
	if Input.is_action_pressed("player_up"):
		target_speed -= max_flight_speed
	current_speed = lerp(current_speed, target_speed, delta if target_speed == 0 else delta/4.0)
	prints(current_speed, target_speed)
	translate_object_local(Vector3(0, 0, current_speed))

	apply_central_force(Vector3(0, level_speed, 0))
	
