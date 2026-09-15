extends Node

func _ready() -> void:
	_add_key("move_forward", KEY_W)
	_add_key("move_backward", KEY_S)
	_add_key("move_left", KEY_A)
	_add_key("move_right", KEY_D)
	_add_key("jump", KEY_SPACE)
	_add_key("sprint", KEY_SHIFT)
	_add_key("interact", KEY_E)
	_add_key("plant", KEY_F)
	_add_key("toggle_camera", KEY_V)
	_add_key("tool_hoe", KEY_1)
	_add_key("tool_water", KEY_2)
	_add_key("tool_pickaxe", KEY_3)
	_add_key("tool_axe", KEY_4)
	_add_mouse("use_tool", MOUSE_BUTTON_LEFT)

func _ensure_action(action: StringName) -> void:
	if not InputMap.has_action(action):
		InputMap.add_action(action)

func _add_key(action: StringName, keycode: Key) -> void:
	_ensure_action(action)
	for existing in InputMap.action_get_events(action):
		if existing is InputEventKey and existing.physical_keycode == keycode:
			return
	var event := InputEventKey.new()
	event.physical_keycode = keycode
	InputMap.action_add_event(action, event)

func _add_mouse(action: StringName, button: MouseButton) -> void:
	_ensure_action(action)
	for existing in InputMap.action_get_events(action):
		if existing is InputEventMouseButton and existing.button_index == button:
			return
	var event := InputEventMouseButton.new()
	event.button_index = button
	InputMap.action_add_event(action, event)
