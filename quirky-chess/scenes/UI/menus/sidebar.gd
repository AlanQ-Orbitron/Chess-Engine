extends PanelContainer

@export var lerpSpeed: float
@onready var margin_container: MarginContainer = $MarginContainer
@onready var menu: Panel = $MarginContainer/HBoxContainer/Menu
@onready var button: Button = $MarginContainer/HBoxContainer/VBoxContainer/Panel/Button

func _ready() -> void:
	var tween: Tween = create_tween()
	tween.tween_property(margin_container, "position", Vector2(-menu.size.x, 0), 0)
	Global.closeAllPopups.emit()

func _on_button_toggled(toggled_on: bool) -> void:
	var tween: Tween = create_tween()
	if toggled_on:
		tween.tween_property(margin_container, "position", Vector2(0, 0), 0.2)
	else:
		tween.tween_property(margin_container, "position", Vector2(-menu.size.x, 0), 0.5)
		Global.closeAllPopups.emit()
