extends AnimatableBody3D

@export var open_angle_degrees := -95.0
@export var open_time := 0.35
var is_open := false
var closed_rotation := 0.0

func _ready() -> void:
	closed_rotation = rotation.y

func interact(_player) -> void:
	is_open = not is_open
	var target := closed_rotation + deg_to_rad(open_angle_degrees if is_open else 0.0)
	var tween := create_tween().set_trans(Tween.TRANS_SINE).set_ease(Tween.EASE_IN_OUT)
	tween.tween_property(self, "rotation:y", target, open_time)
