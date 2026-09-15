extends CanvasLayer

@onready var time_label: Label = $TopLeft/Time
@onready var status_label: Label = $TopLeft/Status
@onready var inventory_label: Label = $TopLeft/Inventory

func _process(_delta: float) -> void:
	var world := get_parent()
	if world != null and world.has_method("get_clock_text"):
		time_label.text = "%s  |  %s" % [world.get_clock_text(), world.get_calendar_text()]
	var players := get_tree().get_nodes_in_group("player")
	if players.is_empty():
		return
	var player = players[0]
	status_label.text = "Tool: %s   Energy: %d/%d   [V] Camera" % [player.active_tool.capitalize(), int(player.energy), int(player.max_energy)]
	var parts: Array[String] = []
	for key in player.inventory.keys():
		parts.append("%s x%d" % [str(key).replace("_", " ").capitalize(), int(player.inventory[key])])
	inventory_label.text = "Inventory: " + "   ".join(parts)
