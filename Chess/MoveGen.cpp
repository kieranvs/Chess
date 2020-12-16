#include "MoveGen.h"

int moveUp(int i, int amount)
{
	return i + (amount * 10);
}

int moveLeft(int i, int amount)
{
	return i - 1;
}

int moveRight(int i, int amount)
{
	return i + 1;
}

const int OffsetMoveByPiece[7][8] =
{
	{}, // Invalid
	{}, // Pawn
	{ -10,  -1,  1, 10, 0,  0,  0,  0 }, // Rook
	{ -21, -19,-12, -8, 8, 12, 19, 21 }, // Knight
	{ -11,  -9,  9, 11, 0,  0,  0,  0 }, // Bishop
	{ -11, -10, -9, -1, 1,  9, 10, 11 }, // Queen
	{ -11, -10, -9, -1, 1,  9, 10, 11 }  // King
};

const bool OffsetMoveSlides[7] = { false, false, true, false, true, true, false };

bool is_in_check(const Board& board, Player player);

void move_gen(const Board& board, Player playerToMove, std::vector<Board>& output)
{
	Player otherPlayer = playerToMove == Player::White ? Player::Black : Player::White;

	auto make_move = [&board, &output, &playerToMove](int from, int to)
	{
		Board newboard = board;
		newboard.sq[from] = Piece::None;
		newboard.sq[to] = board.sq[from];
		newboard.en_passant_target = 0;
		if (!is_in_check(newboard, playerToMove))
			output.push_back(newboard);
	};

	auto make_move_special_2p = [&board, &output, &playerToMove](int from, int to, int en_passant_target)
	{
		Board newboard = board;
		newboard.sq[from] = Piece::None;
		newboard.sq[to] = board.sq[from];
		newboard.en_passant_target = en_passant_target;
		if (!is_in_check(newboard, playerToMove))
			output.push_back(newboard);
	};

	auto make_move_special_ep = [&board, &output, &playerToMove](int from, int to, int capture)
	{
		Board newboard = board;
		newboard.sq[from] = Piece::None;
		newboard.sq[to] = board.sq[from];
		newboard.sq[capture] = Piece::None;
		newboard.en_passant_target = 0;
		if (!is_in_check(newboard, playerToMove))
			output.push_back(newboard);
	};

	int pawnDir = playerToMove == Player::White ? 1 : -1;
	int pawnDoubleMoveRank = playerToMove == Player::White ? 2 : 7;
	for (int i = 0; i < 120; i++)
	{
		if (board.isPlayer(i, playerToMove))
		{
			if (board.isPawn(i))
			{
				// Move forward
				int forwardSq = moveUp(i, pawnDir);
				if (board.isFree(forwardSq))
					make_move(i, forwardSq);

				// Move forward two
				if (board.rankOf(i) == pawnDoubleMoveRank)
				{
					int forwardSq2 = moveUp(i, pawnDir * 2);
					if (board.isFree(forwardSq) && board.isFree(forwardSq2))
						make_move_special_2p(i, forwardSq2, forwardSq);
				}

				// Attack
				int leftAttackSq = moveLeft(moveUp(i, pawnDir), 1);
				if (board.isPlayer(leftAttackSq, otherPlayer))
					make_move(i, leftAttackSq);

				int rightAttackSq = moveRight(moveUp(i, pawnDir), 1);
				if (board.isPlayer(rightAttackSq, otherPlayer))
					make_move(i, rightAttackSq);

				// En passant attack
				if (leftAttackSq == board.en_passant_target || rightAttackSq == board.en_passant_target)
				{
					int ep_capture_sq = moveUp(board.en_passant_target, -pawnDir);
					if (board.isPlayer(ep_capture_sq, otherPlayer) && board.isPawn(ep_capture_sq))
						make_move_special_ep(i, board.en_passant_target, ep_capture_sq);
				}
			}
			else // Not pawns
			{
				auto piece_type = board.sq[i] & PieceTypeMask;
				for (int offsetTableIndex = 0; offsetTableIndex < 8; offsetTableIndex++)
				{
					int curSq = i;
					while (true)
					{
						curSq += OffsetMoveByPiece[piece_type][offsetTableIndex];
						if (board.isFree(curSq))
							make_move(i, curSq);
						else
						{
							if (board.isPlayer(curSq, otherPlayer))
								make_move(i, curSq);
							break;
						}
						if (!OffsetMoveSlides[piece_type])
							break;
					}
				}
			}
		}
	}
}

bool is_in_check(const Board& board, Player player)
{
	auto checkKing = player == Player::White ? Piece::WhiteKing : Piece::BlackKing;
	auto otherPlayer = player == Player::White ? Player::Black : Player::White;

	int pawnDir = otherPlayer == Player::White ? 1 : -1;
	for (int i = 0; i < 120; i++)
	{
		if (board.isPlayer(i, otherPlayer))
		{
			if (board.isPawn(i))
			{
				// Attack
				int leftAttackSq = moveLeft(moveUp(i, pawnDir), 1);
				if (board.sq[leftAttackSq] == checkKing)
					return true;

				int rightAttackSq = moveRight(moveUp(i, pawnDir), 1);
				if (board.sq[rightAttackSq] == checkKing)
					return true;
			}
			else // Not pawns
			{
				auto piece_type = board.sq[i] & PieceTypeMask;
				for (int offsetTableIndex = 0; offsetTableIndex < 8; offsetTableIndex++)
				{
					int curSq = i;
					while (true)
					{
						curSq += OffsetMoveByPiece[piece_type][offsetTableIndex];
						if (!board.isFree(curSq))
						{
							if (board.sq[curSq] == checkKing)
								return true;
							break;
						}
						if (!OffsetMoveSlides[piece_type])
							break;
					}
				}
			}
		}
	}

	return false;
}