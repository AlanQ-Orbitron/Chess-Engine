extends Control

@export var controller: Button
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE
	Global.closeAllSideMenu.connect(closeMenu)
	controller.open.connect(openMenu)
	
func openMenu() -> void:
	visible = true

func closeMenu() -> void:
	visible = false
