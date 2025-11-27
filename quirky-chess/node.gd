extends Node

func _ready() -> void:
	var board: ChessBoard = ChessBoard.new()
	board.generate_board("rnbqkbnr/ppppppp1/7p/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
	print(board.get_valid_moves())
	
	
func toString(n: int) -> String:
	var bits: String = ""
	for i in range(64):
		bits = str(n & 1) + bits
		n >>= 1
	return bits
