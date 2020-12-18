#include "Eval.h"

const double piece_material_value[] =
{
	0.0, // Invalid
	100.0, // Pawn
	500.0, // Rook
	300.0, // Knight
	300.0, // Bishop
	900.0, // Queen
	10000.0 // King
};

const int center_squares[] = { 54, 55, 64, 65};

double evaluate_position(const Board& board)
{
	double material_score = 0.0;
	for (int i = 0; i < 64; i++)
	{
		auto piece = board.sq[mailbox64[i]];
		auto pieceVal = piece_material_value[piece & PieceTypeMask];
		if (board.isPlayer(mailbox64[i], Player::White))
			material_score += pieceVal;
		else if (board.isPlayer(mailbox64[i], Player::Black))
			material_score -= pieceVal;
	}

	double center_control_score = 0.0;
	for (int i = 0; i < 4; i++)
	{
		if (board.isPlayer(center_squares[i], Player::White))
			center_control_score += 105;
		else if (board.isPlayer(center_squares[i], Player::Black))
			center_control_score -= 105;
	}

	return material_score + center_control_score;
}