extends VSlider

signal open
@export var mixerBus: String

func _ready() -> void:
	var busIndex: int = AudioServer.get_bus_index(mixerBus)
	if (busIndex == -1):
		return
	value = AudioServer.get_bus_volume_linear(busIndex)

func _on_pressed() -> void:
	open.emit()

func _on_value_changed(_value: float) -> void:
	var busIndex: int = AudioServer.get_bus_index(mixerBus)
	if (busIndex == -1):
		return
	AudioServer.set_bus_volume_db(busIndex, linear_to_db(value))
