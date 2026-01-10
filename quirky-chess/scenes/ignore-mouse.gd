extends Control

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE

func hideOtherMenus(node: Node) -> void:
	if get_children().size() <= 0:
		return
	for child in get_children():
		child.hideUnlessSelf(node)
