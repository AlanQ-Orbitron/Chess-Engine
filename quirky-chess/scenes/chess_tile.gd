extends TileMapLayer

var pieceDate: Dictionary
var board: ChessBoard = ChessBoard.new()

func getPiece(coord: Vector2) -> Variant: # Dictionary
	var cellPosition: Vector2 = local_to_map(to_local(coord))
	if get_cell_source_id(cellPosition) == -1:
		return null
	else:
		pieceDate = {
			"position": cellPosition,
			"atlas": get_cell_atlas_coords(cellPosition)
		}
	
	return pieceDate

func isLegalMove(sources: Vector2, destination: Vector2) -> bool:
	var sourcesUCI: String = char(ord('a') + int(sources.x)) + str(abs(int(sources.y)))
	var destinationUCI: String = char(ord('a') + int(destination.x)) + str(abs(int(destination.y)))
	var fullUCI: String = sourcesUCI + destinationUCI
	return fullUCI in board.get_valid_moves()

func UCIToVector(UCI: String) -> Array[Vector2i]:
	var source: Vector2i = Vector2i(ord(UCI[0]) - ord('a'), UCI[1].to_int())
	var destination: Vector2i = Vector2i(ord(UCI[2]) - ord('a'), UCI[3].to_int())
	return [source, destination]

func setHighlights(sources: Vector2, moves: Array) -> void:
	for i in range(8):
		for j in range(8):
			set_cell(Vector2(i, j * -1), -1)
	
	var sourcesUCI: String = char(ord('a') + int(sources.x)) + str(abs(int(sources.y)))
	var list: Array = []
	for move: String in moves:
		if sourcesUCI.left(2) == move.left(2):
			list.append(move)
	for move: String in list:
		set_cell(Vector2i(ord(move[2]) - ord('a'), move[3].to_int() * -1), 1, Vector2i.ZERO)
	pass

func moveTo(piece: Dictionary, coord: Vector2) -> bool:
	if isLegalMove(piece.get("position"), coord):
		set_cell(piece.get("position"), -1)
		set_cell(coord, 2, piece.get("atlas"))
		return true
	return false
