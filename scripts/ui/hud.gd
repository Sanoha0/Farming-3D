extends CanvasLayer

@onready var time_label: Label = $TopLeft/Time
@onready var status_label: Label = $TopLeft/Status
@onready var inventory_label: Label = $TopLeft/Inventory

func _process(_delta: float) -> void:
	var world: Object = get_parent()
	if world != null and world.has_method("get_clock_text") and world.has_method("get_calendar_text"):
		var clock_text: String = str(world.call("get_clock_text"))
		var calendar_text: String = str(world.call("get_calendar_text"))
		time_label.text = "%s  |  %s" % [clock_text, calendar_text]

	var players: Array[Node] = get_tree().get_nodes_in_group("player")
	if players.is_empty():
		return

	var player = players[0]
	status_label.text = "Tool: %s   Energy: %d/%d   [V] Camera" % [str(player.active_tool).capitalize(), int(player.energy), int(player.max_energy)]

	var parts: Array[String] = []
	for key in player.inventory.keys():
		parts.append("%s x%d" % [str(key).replace("_", " ").capitalize(), int(player.inventory[key])])
	inventory_label.text = "Inventory: " + "   ".join(parts)
