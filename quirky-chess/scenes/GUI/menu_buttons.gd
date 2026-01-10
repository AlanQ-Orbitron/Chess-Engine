extends Button

@export var sideMenu: Control
@onready var side_menu_container: Control = %SideMenu

func _ready() -> void:
	if sideMenu == null:
		printerr("Null Node")
		return

func _on_mouse_entered() -> void:
	side_menu_container.hideOtherMenus(sideMenu)
	if sideMenu == null:
		printerr("Null Node")
		return
	sideMenu.show()
