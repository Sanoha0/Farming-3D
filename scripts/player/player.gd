extends CharacterBody3D

signal tool_changed(tool_name: String)
signal energy_changed(current: float, maximum: float)
signal inventory_changed

@export var walk_speed: float = 5.0
@export var sprint_speed: float = 8.0
@export var jump_velocity: float = 5.0
@export var mouse_sensitivity: float = 0.0022
@export var interaction_distance: float = 5.0
@export var max_energy: float = 100.0

var energy: float = 100.0
var first_person: bool = true
var active_tool: String = "hoe"
var inventory: Dictionary = {"seed_turnip": 12, "wood": 0, "stone": 0, "turnip": 0}
var gravity: float = float(ProjectSettings.get_setting("physics/3d/default_gravity"))
var pitch: float = 0.0

@onready var head: Node3D = $Head
@onready var first_camera: Camera3D = $Head/FirstPersonCamera
@onready var third_pivot: Node3D = $ThirdPersonPivot
@onready var third_camera: Camera3D = $ThirdPersonPivot/SpringArm3D/ThirdPersonCamera
@onready var body_mesh: MeshInstance3D = $Body
@onready var tool_mesh: MeshInstance3D = $Head/FirstPersonCamera/ToolAnchor/ToolMesh

func _ready() -> void:
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
	energy = max_energy
	_apply_camera_mode()
	_update_tool_visual()
	energy_changed.emit(energy, max_energy)
	tool_changed.emit(active_tool)

func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventMouseMotion and Input.mouse_mode == Input.MOUSE_MODE_CAPTURED:
		rotate_y(-event.relative.x * mouse_sensitivity)
		pitch = clampf(pitch - event.relative.y * mouse_sensitivity, deg_to_rad(-85.0), deg_to_rad(85.0))
		head.rotation.x = pitch
		third_pivot.rotation.x = pitch
	if event.is_action_pressed("ui_cancel"):
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE if Input.mouse_mode == Input.MOUSE_MODE_CAPTURED else Input.MOUSE_MODE_CAPTURED

func _physics_process(delta: float) -> void:
	if not is_on_floor():
		velocity.y -= gravity * delta
	if Input.is_action_just_pressed("jump") and is_on_floor():
		velocity.y = jump_velocity

	var input_vec: Vector2 = Input.get_vector("move_left", "move_right", "move_forward", "move_backward")
	var direction: Vector3 = (transform.basis * Vector3(input_vec.x, 0.0, input_vec.y)).normalized()
	var speed: float = sprint_speed if Input.is_action_pressed("sprint") else walk_speed
	if not direction.is_zero_approx():
		velocity.x = direction.x * speed
		velocity.z = direction.z * speed
	else:
		velocity.x = move_toward(velocity.x, 0.0, speed * 6.0 * delta)
		velocity.z = move_toward(velocity.z, 0.0, speed * 6.0 * delta)
	move_and_slide()

	if Input.is_action_just_pressed("toggle_camera"):
		first_person = not first_person
		_apply_camera_mode()
	if Input.is_action_just_pressed("tool_hoe"):
		_select_tool("hoe")
	if Input.is_action_just_pressed("tool_water"):
		_select_tool("water")
	if Input.is_action_just_pressed("tool_pickaxe"):
		_select_tool("pickaxe")
	if Input.is_action_just_pressed("tool_axe"):
		_select_tool("axe")
	if Input.is_action_just_pressed("use_tool"):
		_use_tool()
	if Input.is_action_just_pressed("interact"):
		_interact()
	if Input.is_action_just_pressed("plant"):
		_plant()

func _apply_camera_mode() -> void:
	first_camera.current = first_person
	third_camera.current = not first_person
	body_mesh.visible = not first_person
	tool_mesh.visible = first_person

func _select_tool(tool_name: String) -> void:
	active_tool = tool_name
	_update_tool_visual()
	tool_changed.emit(active_tool)

func _update_tool_visual() -> void:
	match active_tool:
		"hoe":
			tool_mesh.scale = Vector3(0.75, 0.75, 1.3)
		"water":
			tool_mesh.scale = Vector3(1.2, 0.8, 0.8)
		"pickaxe":
			tool_mesh.scale = Vector3(1.25, 0.7, 1.25)
		"axe":
			tool_mesh.scale = Vector3(0.9, 0.75, 1.25)

func _ray_target() -> Object:
	var camera: Camera3D = get_viewport().get_camera_3d()
	if camera == null:
		return null
	var origin: Vector3 = camera.global_position
	var ray_end: Vector3 = origin + -camera.global_transform.basis.z * interaction_distance
	var query: PhysicsRayQueryParameters3D = PhysicsRayQueryParameters3D.create(origin, ray_end)
	query.exclude = [get_rid()]
	var hit: Dictionary = get_world_3d().direct_space_state.intersect_ray(query)
	if hit.is_empty():
		return null
	return hit.get("collider") as Object

func _use_tool() -> void:
	var target: Object = _ray_target()
	if target == null or not target.has_method("apply_tool"):
		return
	var tool_costs: Dictionary = {"hoe": 2.0, "water": 1.0, "pickaxe": 3.0, "axe": 3.0}
	var cost: float = float(tool_costs.get(active_tool, 0.0))
	if energy < cost:
		return
	if target.call("apply_tool", active_tool, self):
		energy -= cost
		energy_changed.emit(energy, max_energy)

func _interact() -> void:
	var target: Object = _ray_target()
	if target != null and target.has_method("interact"):
		target.call("interact", self)

func _plant() -> void:
	var target: Object = _ray_target()
	if target != null and target.has_method("plant_seed"):
		target.call("plant_seed", self)

func add_item(item_id: String, amount: int = 1) -> void:
	inventory[item_id] = int(inventory.get(item_id, 0)) + amount
	inventory_changed.emit()

func remove_item(item_id: String, amount: int = 1) -> bool:
	if int(inventory.get(item_id, 0)) < amount:
		return false
	inventory[item_id] = int(inventory[item_id]) - amount
	inventory_changed.emit()
	return true

func restore_energy() -> void:
	energy = max_energy
	energy_changed.emit(energy, max_energy)
