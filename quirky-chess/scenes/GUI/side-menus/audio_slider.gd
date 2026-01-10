@tool
extends HBoxContainer

@export var text: String
@export var texture: Texture
@export_range(0, 100) var minValue: float = 0
@export_range(0, 100) var maxValue: float = 100
@export var AudioBus: StringName

@onready var textBox: Label = $Text
@onready var imageBox: TextureRect = $Image
@onready var h_slider: HSlider = $HSlider

func _ready() -> void:
	textBox.text = text
	imageBox.texture = texture
	h_slider.min_value = minValue
	h_slider.max_value = maxValue

func _on_h_slider_value_changed(value: float) -> void:
	var busIndex: int = AudioServer.get_bus_index(AudioBus)
	if busIndex == -1:
		printerr("Missing Bus")
		return
	var linear_value: float = clamp(value / 100.0, 0.0, 1.0)
	AudioServer.set_bus_volume_db(busIndex, linear_to_db(linear_value))
	
