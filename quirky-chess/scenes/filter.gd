extends ColorRect
@onready var viewport: Viewport = get_viewport()
var centered_mouse_position: Vector2

func _ready() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE
	
	material.set_shader_parameter("screen_size", viewport.get_visible_rect().size)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	centered_mouse_position = (
		viewport.get_mouse_position() / 
		viewport.get_visible_rect().size) * 2.0 - Vector2(1.0, 1.0
	)
	centered_mouse_position *= 0.01 / (1.0 + centered_mouse_position.length() * 0.8)
	material.set_shader_parameter("offset", centered_mouse_position)
