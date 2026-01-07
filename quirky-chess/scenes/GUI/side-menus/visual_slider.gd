@tool
extends HBoxContainer

@export var text: String
@export var texture: Texture
@export_range(0, 100) var minValue: float = 0
@export_range(0, 100) var maxValue: float = 100

@onready var textBox: Label = $Text
@onready var imageBox: TextureRect = $Image
@onready var h_slider: HSlider = $HSlider

func _ready() -> void:
	textBox.text = text
	imageBox.texture = texture
	h_slider.min_value = minValue
	h_slider.max_value = maxValue

func _on_h_slider_value_changed(value: float) -> void:
	
