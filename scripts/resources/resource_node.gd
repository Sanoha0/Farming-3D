extends StaticBody3D

@export_enum("pickaxe", "axe") var required_tool := "pickaxe"
@export var durability := 3
@export var reward_item := "stone"
@export var reward_amount := 2

var remaining := 0

func _ready() -> void:
	remaining = max(durability, 1)

func apply_tool(tool_name: String, player) -> bool:
	if tool_name != required_tool or remaining <= 0:
		return false
	remaining -= 1
	if remaining <= 0:
		player.add_item(reward_item, reward_amount)
		queue_free()
	return true
