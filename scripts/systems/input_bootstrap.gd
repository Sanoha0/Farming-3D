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

func _add_key(action: StringName, keycode: int) -> void:
	_ensure_action(action)
	for existing in InputMap.action_get_events(action):
		if existing is InputEventKey and existing.physical_keycode == keycode:
			return
	var key_event: InputEventKey = InputEventKey.new()
	key_event.physical_keycode = keycode
	InputMap.action_add_event(action, key_event)

func _add_mouse(action: StringName, button: int) -> void:
	_ensure_action(action)
	for existing in InputMap.action_get_events(action):
		if existing is InputEventMouseButton and existing.button_index == button:
			return
	var mouse_event: InputEventMouseButton = InputEventMouseButton.new()
	mouse_event.button_index = button
	InputMap.action_add_event(action, mouse_event)
