extends Button

signal open

func _on_pressed() -> void:
	open.emit()
