extends Node3D

signal new_day(day: int, season: String, year: int)

@export var real_seconds_per_game_day := 900.0
@export var starting_hour := 6.0
@export var days_per_season := 28

var minutes_into_day := 360.0
var day := 1
var year := 1
var season_index := 0
var seasons := ["Spring", "Summer", "Fall", "Winter"]

@onready var sun: DirectionalLight3D = $Lighting/Sun

func _ready() -> void:
	minutes_into_day = starting_hour * 60.0
	_update_sun()

func _process(delta: float) -> void:
	if real_seconds_per_game_day <= 0.0:
		return
	minutes_into_day += delta * (1440.0 / real_seconds_per_game_day)
	while minutes_into_day >= 1440.0:
		minutes_into_day -= 1440.0
		advance_day()
	_update_sun()

func advance_day() -> void:
	day += 1
	if day > days_per_season:
		day = 1
		season_index += 1
		if season_index >= seasons.size():
			season_index = 0
			year += 1
	for plot in get_tree().get_nodes_in_group("farm_plots"):
		if plot.has_method("advance_day"):
			plot.advance_day()
	new_day.emit(day, seasons[season_index], year)

func sleep_to_next_day(player = null) -> void:
	advance_day()
	minutes_into_day = starting_hour * 60.0
	if player != null and player.has_method("restore_energy"):
		player.restore_energy()

func get_clock_text() -> String:
	var total := int(minutes_into_day) % 1440
	return "%02d:%02d" % [total / 60, total % 60]

func get_calendar_text() -> String:
	return "%s %d, Year %d" % [seasons[season_index], day, year]

func _update_sun() -> void:
	if sun == null:
		return
	var hour := minutes_into_day / 60.0
	var angle := lerp(-90.0, 270.0, hour / 24.0)
	sun.rotation_degrees.x = angle
