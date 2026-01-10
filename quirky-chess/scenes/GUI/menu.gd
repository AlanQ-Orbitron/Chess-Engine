extends Control

func _ready() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE
	
func hideUnlessSelf(node: Node) -> void:
	if node != self:
		hide()
