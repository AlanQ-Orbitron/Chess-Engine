extends Control

@onready var h_box_container: GridContainer = $MarginContainer/HBoxContainer
@onready var pieces: PackedScene = preload("res://scenes/chess board/pieces.tscn") as PackedScene

signal choice(button: Button)

const AtlasToFen: Dictionary = {
	"p" : Vector2i(0, 0), "P" : Vector2i(1, 0),
	"r" : Vector2i(0, 1), "R" : Vector2i(1, 1),
	"n" : Vector2i(0, 2), "N" : Vector2i(1, 2),
	"b" : Vector2i(0, 3), "B" : Vector2i(1, 3),
	"q" : Vector2i(0, 4), "Q" : Vector2i(1, 4),
	"k" : Vector2i(0, 5), "K" : Vector2i(1, 5)
}

func get_choice(list: Array) -> Vector2:
	show()
	var piece_instance: Control
	var atlas: Vector2
	for item: String in list:
		piece_instance = pieces.instantiate()
		atlas = AtlasToFen.get(item)
		piece_instance.get_child(0).animation = "White" if atlas.x == 1 else "Black"
		piece_instance.get_child(0).frame = atlas.y
		h_box_container.add_child(piece_instance)
		piece_instance.pressed.connect(func()->void:choice.emit(piece_instance))
	var button: AnimatedSprite2D = (await choice).get_child(0)
	var piece: Vector2 = Vector2(1 if button.animation == "White" else 0, button.frame)
	for child in get_children():
		child.queue_free()
	return piece
