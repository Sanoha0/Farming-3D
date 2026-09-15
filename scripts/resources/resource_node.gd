extends StaticBody3D

@export_enum("pickaxe", "axe") var required_tool: String = "pickaxe"
@export var durability: int = 3
@export var reward_item: String = "stone"
@export var reward_amount: int = 2

var remaining: int = 0

func _ready() -> void:
	remaining = durability if durability > 0 else 1

func apply_tool(tool_name: String, player) -> bool:
	if tool_name != required_tool or remaining <= 0:
		return false
	remaining -= 1
	if remaining <= 0:
		player.add_item(reward_item, reward_amount)
		queue_free()
	return true
