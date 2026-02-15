extends TileMapLayer

@onready var promotion_menu: Control = %"Promotion Menu"
@onready var chess_engine: Node2D = $".."


const AtlasToFen: Dictionary = {
	Vector2i(0, 0) : "p", Vector2i(1, 0) : "P",
	Vector2i(0, 1) : "r", Vector2i(1, 1) : "R",
	Vector2i(0, 2) : "n", Vector2i(1, 2) : "N",
	Vector2i(0, 3) : "b", Vector2i(1, 3) : "B",
	Vector2i(0, 4) : "q", Vector2i(1, 4) : "Q",
	Vector2i(0, 5) : "k", Vector2i(1, 5) : "K"
}

var board: ChessBoard

func localized(pos: Vector2) -> Vector2i:
	return local_to_map(to_local(pos))

func invertY(coord: Vector2i) -> Vector2i:
	return Vector2i(coord.x, -coord.y)

func boardToFen() -> String:
	var fen: String = ""
	var count: int = 0
	for y in range(8, 0, -1):
		for x in range(1, 9):
			if (get_cell_atlas_coords(Vector2i(x, -y)) == Vector2i(-1, -1)): 
				count += 1
			else:
				if (count): 
					fen += char(count + ord('0'))
					count = 0
				fen += AtlasToFen.get(get_cell_atlas_coords(Vector2i(x, -y)))
		if (count): 
			fen += char(count + ord('0'))
			count = 0
		fen += "/" if (y > 1) else ""
	return fen

func getPiece(pos: Vector2) -> Variant: # Dictionary
	var piecePosition: Vector2i = localized(pos)
	var pieceType: Vector2i = get_cell_atlas_coords(piecePosition)
	
	if pieceType == Vector2i(-1, -1):
		return null
	
	return {
		"pieceType": pieceType,
		"position": invertY(piecePosition)
	}

func setPiece(piece: Dictionary) -> void:
	set_cell(invertY(piece.get("position")), 2, piece.get("pieceType"))

func setHighlights(piece: Dictionary, moves: Dictionary) -> void:
	clear()
	var piecePos: String = UCISink(piece.get("position"))
	for move: String in moves:
		if move[0] == piecePos[0] && move[1] == piecePos[1]:
			set_cell(filerankSink(move.substr(2,2)), 1, Vector2i(0, 0))

func moveTo(piece: Dictionary, pos: Vector2, legalMoves: Dictionary) -> bool:
	var coords: Vector2i = invertY(localized(pos))
	var move: String = UCISink(piece.get("position")) + UCISink(coords)
	var pieces_type: String
	var promotion_string: String
	if move in legalMoves:
		pieces_type = AtlasToFen.get(piece.get("pieceType"))
		if legalMoves.has(move):
			if legalMoves.get(move) is Array:
				promotion_menu.global_position = to_global(map_to_local(filerankSink(move.substr(2, 2)))) - Vector2(0, 36)
				var promotion_type: Vector2i = await promotion_menu.get_choice(legalMoves.get(move))
				promotion_string = AtlasToFen.get(promotion_type)
				piece.set("pieceType", promotion_type)
			elif legalMoves.get(move) == "E":
				clearCell(Vector2i(coords.x, coords.y + (1 if (pieces_type != pieces_type.to_upper()) else -1)))
		var sanityCheck: bool = board.move_to(move + promotion_string)
		if !sanityCheck:
			printerr("Move To Error")
		clearCell(piece.get("position"))
		piece.set("position", coords)
		setPiece(piece)
		return true
	return false


func clearCell(coords: Vector2i) -> void:
	set_cell(invertY(coords), -1)

func UCISink(coords: Vector2i) -> String:
	var rank: String = char(coords.x + ord('a') - 1)
	var file: String = char(coords.y + ord('0'))
	return rank + file

func filerankSink(UCI: String) -> Vector2i:
	var rank: int = ord(UCI[0]) - ord('a') + 1
	var file: int = int(UCI[1]) * -1
	return Vector2i(rank, file)
