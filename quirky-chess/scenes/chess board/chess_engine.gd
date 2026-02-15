extends Node2D

@onready var highlights: TileMapLayer = $Highlights
@onready var chess_pieces: TileMapLayer = $ChessPieces
@onready var pieces: Control = $Pieces
@onready var chess_sounds: AudioStreamPlayer = $ChessSounds
@onready var promotion_menu: Control = $"Promotion Menu"

var board: ChessBoard = ChessBoard.new()
var selectedPiece: Variant # Dictionary
var isSelectedNull: bool

enum piece {
	Pawn,
	Rook,
	Knight,
	Biship,
	Queen,
	King
}

enum color {
	Black,
	White
}

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("MB1"):
		selectedPiece = chess_pieces.getPiece(get_global_mouse_position())
		if selectedPiece == null:
			pieces.hide()
			isSelectedNull = true
		else:
			pieces.show()
			isSelectedNull = false
			highlights.setHighlights(selectedPiece, board.get_valid_moves())
			chess_pieces.set_cell(selectedPiece.get("position"), -1)
			match selectedPiece.get("pieceType").x:
				0:
					pieces.get_child(0).animation = "Black"
				1:
					pieces.get_child(0).animation = "White"
			pieces.get_child(0).frame = selectedPiece.get("pieceType").y
	
	if event.is_action_released("MB1"):
		pieces.hide()
		if !isSelectedNull:
			highlights.clear()
			if !await chess_pieces.moveTo(selectedPiece, get_global_mouse_position(), board.get_valid_moves()):
				chess_pieces.setPiece(selectedPiece)
			else:
				chess_sounds.play()


func _ready() -> void:
	chess_pieces.board = board
	board.generate_board(chess_pieces.boardToFen() + " w KQkq - 0 1")
	#menu.connect("updateSettings", Callable(self, "setSettings"))

func setSettings() -> void:
	print(Global.settings)

func toString(n: int) -> String:
	var bits: String = ""
	for i in range(64):
		bits = str(n & 1) + bits
		n >>= 1
	return bits
