@tool
extends TextureRect

@export var primaryColor: Color = Color(0.353, 0.412, 0.533)
@export var secondaryColor: Color = Color(0.623, 0.688, 0.828, 1.0)

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE
	material.set_shader_parameter("primary", primaryColor)
	material.set_shader_parameter("secondary", secondaryColor)
	
