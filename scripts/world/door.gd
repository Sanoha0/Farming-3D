extends AnimatableBody3D

@export var open_angle_degrees: float = -95.0
@export var open_time: float = 0.35

var is_open: bool = false
var closed_rotation: float = 0.0

func _ready() -> void:
	closed_rotation = rotation.y

func interact(_player) -> void:
	is_open = not is_open
	var target_rotation: float = closed_rotation + deg_to_rad(open_angle_degrees if is_open else 0.0)
	var tween: Tween = create_tween().set_trans(Tween.TRANS_SINE).set_ease(Tween.EASE_IN_OUT)
	tween.tween_property(self, "rotation:y", target_rotation, open_time)
