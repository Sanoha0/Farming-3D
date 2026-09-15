extends StaticBody3D

@export var crop_id: String = "turnip"
@export var seed_item_id: String = "seed_turnip"
@export var days_to_mature: int = 4

var tilled: bool = false
var watered: bool = false
var planted: bool = false
var mature: bool = false
var growth_days: int = 0

@onready var untilled_visual: MeshInstance3D = $UntilledVisual
@onready var tilled_visual: MeshInstance3D = $TilledVisual
@onready var watered_visual: MeshInstance3D = $WateredVisual
@onready var crop_visual: MeshInstance3D = $CropVisual

func _ready() -> void:
	_update_visuals()

func apply_tool(tool_name: String, _player) -> bool:
	if tool_name == "hoe" and not planted and not tilled:
		tilled = true
		_update_visuals()
		return true
	if tool_name == "water" and tilled and not watered:
		watered = true
		_update_visuals()
		return true
	return false

func plant_seed(player) -> bool:
	if not tilled or planted:
		return false
	if not player.remove_item(seed_item_id, 1):
		return false
	planted = true
	mature = false
	growth_days = 0
	_update_visuals()
	return true

func interact(player) -> void:
	if mature:
		player.add_item(crop_id, 1)
		planted = false
		mature = false
		growth_days = 0
		watered = false
		_update_visuals()

func advance_day() -> void:
	if planted and watered:
		growth_days += 1
		if growth_days >= days_to_mature:
			mature = true
	watered = false
	_update_visuals()

func _update_visuals() -> void:
	untilled_visual.visible = not tilled
	tilled_visual.visible = tilled and not watered
	watered_visual.visible = tilled and watered
	crop_visual.visible = planted
	if planted:
		var safe_days: int = days_to_mature if days_to_mature > 0 else 1
		var progress: float = clampf(float(growth_days + 1) / float(safe_days), 0.2, 1.0)
		crop_visual.scale = Vector3.ONE * progress
