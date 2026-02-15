extends Button

@onready var menu: Control = $Menu
@onready var selectorIcon: TextureRect = $Icon

var onMenu: bool = false

func _ready() -> void:
	Global.closeAllPopups.connect(closeMenu)
	menu.visible = false
	menu.mouse_filter = Control.MOUSE_FILTER_STOP

func _on_mouse_entered() -> void:
	Global.closeAllPopups.emit()
	menu.visible = true
	selectorIcon.visible = true

func _on_mouse_exited() -> void:
	selectorIcon.visible = false
	
func _on_margin_container_mouse_entered() -> void:
	onMenu = true
	
func _on_margin_container_mouse_exited() -> void:
	onMenu = false

func _unhandled_input(event: InputEvent) -> void:
	if !(onMenu) && event.is_action_pressed("MB1"):
		closeMenu()

func closeMenu() -> void:
	menu.visible = false
