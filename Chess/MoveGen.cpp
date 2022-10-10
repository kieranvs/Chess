#include "MoveGen.h"
#include "Utils.h"

#include <algorithm>

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

bool is_king_in_check_here(const Board& board, Player player, int kingPos);

void move_gen(const Board& board, std::vector<MoveGenResult>& output)
{
	Player playerToMove = board.playerToMove();
	Player otherPlayer = board.oppositePlayer();

	auto update_castle_rights = [](Board& new_board, int from, int to)
	{
		if (from == SquareName::A1 || to == SquareName::A1)
			new_board.castle_rights = new_board.castle_rights & ~CastleRightsWhiteQueenside;
		if (from == SquareName::H1 || to == SquareName::H1)
			new_board.castle_rights = new_board.castle_rights & ~CastleRightsWhiteKingside;
		if (from == SquareName::A8 || to == SquareName::A8)
			new_board.castle_rights = new_board.castle_rights & ~CastleRightsBlackQueenside;
		if (from == SquareName::H8 || to == SquareName::H8)
			new_board.castle_rights = new_board.castle_rights & ~CastleRightsBlackKingside;
		if (from == SquareName::E1)
			new_board.castle_rights = new_board.castle_rights & ~CastleRightsWhite;
		if (from == SquareName::E8)
			new_board.castle_rights = new_board.castle_rights & ~CastleRightsBlack;
	};

	auto make_move = [&board, &output, &playerToMove, &update_castle_rights](int from, int to)
	{
		MoveGenResult node;
		node.board = board;
		node.board.sq[from] = Piece::None;
		node.board.sq[to] = board.sq[from];
		node.board.en_passant_target = 0;
		node.board.setPlayer(board.oppositePlayer());
		update_castle_rights(node.board, from, to);
		node.move_type = board.sq[to] == Piece::None ? MoveType::Move : MoveType::Capture;
		node.sq_from = from;
		node.sq_to = to;
		if (!is_in_check(node.board, playerToMove))
			output.push_back(node);
	};

	auto make_move_special_promotion = [&board, &output, &playerToMove, &update_castle_rights](int from, int to)
	{
		MoveGenResult node;
		node.board = board;
		node.board.sq[from] = Piece::None;
		node.board.en_passant_target = 0;
		node.board.setPlayer(board.oppositePlayer());
		update_castle_rights(node.board, from, to);
		node.move_type = board.sq[to] == Piece::None ? MoveType::Promotion : MoveType::CapturePromotion;
		node.sq_from = from;
		node.sq_to = to;

		node.board.sq[to] = playerToMove == Player::White ? Piece::WhiteQueen : Piece::BlackQueen;
		if (!is_in_check(node.board, playerToMove))
			output.push_back(node);

		node.board.sq[to] = playerToMove == Player::White ? Piece::WhiteRook : Piece::BlackRook;
		if (!is_in_check(node.board, playerToMove))
			output.push_back(node);

		node.board.sq[to] = playerToMove == Player::White ? Piece::WhiteBishop : Piece::BlackBishop;
		if (!is_in_check(node.board, playerToMove))
			output.push_back(node);

		node.board.sq[to] = playerToMove == Player::White ? Piece::WhiteKnight : Piece::BlackKnight;
		if (!is_in_check(node.board, playerToMove))
			output.push_back(node);
	};

	auto make_move_special_2p = [&board, &output, &playerToMove](int from, int to, int en_passant_target)
	{
		MoveGenResult node;
		node.board = board;
		node.board.sq[from] = Piece::None;
		node.board.sq[to] = board.sq[from];
		node.board.en_passant_target = en_passant_target;
		node.board.setPlayer(board.oppositePlayer());
		node.move_type = MoveType::Move;
		node.sq_from = from;
		node.sq_to = to;
		if (!is_in_check(node.board, playerToMove))
			output.push_back(node);
	};

	auto make_move_special_ep = [&board, &output, &playerToMove](int from, int to, int capture)
	{
		MoveGenResult node;
		node.board = board;
		node.board.sq[from] = Piece::None;
		node.board.sq[to] = board.sq[from];
		node.board.sq[capture] = Piece::None;
		node.board.en_passant_target = 0;
		node.board.setPlayer(board.oppositePlayer());
		node.move_type = MoveType::CaptureEnPassant;
		node.sq_from = from;
		node.sq_to = to;
		if (!is_in_check(node.board, playerToMove))
			output.push_back(node);
	};

	auto make_move_special_castle = [&board, &output, &playerToMove, &update_castle_rights](int from_king, int from_rook, int to_king, int to_rook, int king_step_dir)
	{
		MoveGenResult node;
		node.board = board;
		node.board.sq[from_king] = Piece::None;
		node.board.sq[from_rook] = Piece::None;

		// before replacing the pieces, verify that the king wouldn't go through check
		int curSq = from_king;
		while (curSq != to_king)
		{
			if (is_king_in_check_here(node.board, playerToMove, curSq))
				return;
			curSq += king_step_dir;
		}

		node.board.sq[to_king] = board.sq[from_king];
		node.board.sq[to_rook] = board.sq[from_rook];
		node.board.en_passant_target = 0;
		node.board.setPlayer(board.oppositePlayer());
		update_castle_rights(node.board, from_king, to_king);
		node.move_type = MoveType::Castle;
		node.sq_from = from_king;
		node.sq_to = to_king;
		if (!is_in_check(node.board, playerToMove))
			output.push_back(node);
	};

	int pawnDir = playerToMove == Player::White ? 1 : -1;
	int pawnDoubleMoveRank = playerToMove == Player::White ? 2 : 7;
	int pawnPromotionRank = playerToMove == Player::White ? 7 : 2;
	uint8_t CastleRightsPlayer = playerToMove == Player::White ? CastleRightsWhite : CastleRightsBlack;
	for (int i = 0; i < 120; i++)
	{
		if (board.isPlayer(i, playerToMove))
		{
			if (board.isPawn(i))
			{
				int pawnRank = board.rankOf(i);
				int leftAttackSq = moveLeft(moveUp(i, pawnDir), 1);
				int rightAttackSq = moveRight(moveUp(i, pawnDir), 1);
				int forwardSq = moveUp(i, pawnDir);

				if (pawnRank == pawnPromotionRank)
				{
					// Move forward
					if (board.isFree(forwardSq))
						make_move_special_promotion(i, forwardSq);

					// Attack
					if (board.isPlayer(leftAttackSq, otherPlayer))
						make_move_special_promotion(i, leftAttackSq);

					if (board.isPlayer(rightAttackSq, otherPlayer))
						make_move_special_promotion(i, rightAttackSq);
				}
				else
				{
					// Move forward
					if (board.isFree(forwardSq))
						make_move(i, forwardSq);

					// Attack
					if (board.isPlayer(leftAttackSq, otherPlayer))
						make_move(i, leftAttackSq);

					if (board.isPlayer(rightAttackSq, otherPlayer))
						make_move(i, rightAttackSq);
				}

				// Move forward two
				if (pawnRank == pawnDoubleMoveRank)
				{
					int forwardSq2 = moveUp(i, pawnDir * 2);
					if (board.isFree(forwardSq) && board.isFree(forwardSq2))
						make_move_special_2p(i, forwardSq2, forwardSq);
				}

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

			if (board.isKing(i) && (board.castle_rights & CastleRightsPlayer))
			{
				if (playerToMove == Player::White)
				{
					if (board.castle_rights & CastleRightsWhiteKingside)
					{
						if (board.isFree(SquareName::F1) && board.isFree(SquareName::G1))
							make_move_special_castle(SquareName::E1, SquareName::H1, SquareName::G1, SquareName::F1, 1);
					}
					if (board.castle_rights & CastleRightsWhiteQueenside)
					{
						if (board.isFree(SquareName::B1) && board.isFree(SquareName::C1) && board.isFree(SquareName::D1))
							make_move_special_castle(SquareName::E1, SquareName::A1, SquareName::C1, SquareName::D1, -1);
					}
				}
				else
				{
					if (board.castle_rights & CastleRightsBlackKingside)
					{
						if (board.isFree(SquareName::F8) && board.isFree(SquareName::G8))
							make_move_special_castle(SquareName::E8, SquareName::H8, SquareName::G8, SquareName::F8, 1);
					}
					if (board.castle_rights & CastleRightsBlackQueenside)
					{
						if (board.isFree(SquareName::B8) && board.isFree(SquareName::C8) && board.isFree(SquareName::D8))
							make_move_special_castle(SquareName::E8, SquareName::A8, SquareName::C8, SquareName::D8, -1);
					}
				}
			}
		}
	}

	constexpr bool sort_moves = true;
	if (sort_moves)
	{
		std::sort(output.begin(), output.end(),
			[](const MoveGenResult& m1, const MoveGenResult& m2)
			{
				return (int)m1.move_type > (int)m2.move_type;
			});
	}
}

const int KnightOffsets[] =
{
	-21, -19,-12, -8, 8, 12, 19, 21
};

const int KingCheckOffsetsSlide[] =
{
	-11, -10, -9, -1, 1, 9, 10, 11
};

const bool KingCheckOffsetsSlideIsDiagonal[] =
{
	true, false, true, false, false, true, false, true
};

bool is_in_check(const Board& board, Player player)
{
	auto checkKing = player == Player::White ? Piece::WhiteKing : Piece::BlackKing;

	for (int kingPos = 0; kingPos < 120; kingPos++)
	{
		if (board.sq[kingPos] == checkKing)
		{
			return is_king_in_check_here(board, player, kingPos);
		}
	}

	return false;
}

bool is_king_in_check_here(const Board& board, Player player, int kingPos)
{
	auto otherPlayer = player == Player::White ? Player::Black : Player::White;
	auto enemyPawn = otherPlayer == Player::White ? Piece::WhitePawn : Piece::BlackPawn;
	auto enemyKnight = otherPlayer == Player::White ? Piece::WhiteKnight : Piece::BlackKnight;
	auto enemyQueen = otherPlayer == Player::White ? Piece::WhiteQueen : Piece::BlackQueen;
	auto enemyKing = otherPlayer == Player::White ? Piece::WhiteKing : Piece::BlackKing;

	int pawnDir = otherPlayer == Player::White ? 1 : -1;

	//Pawns
	int leftAttackSq = moveLeft(moveUp(kingPos, -pawnDir), 1);
	if (board.sq[leftAttackSq] == enemyPawn)
		return true;

	int rightAttackSq = moveRight(moveUp(kingPos, -pawnDir), 1);
	if (board.sq[rightAttackSq] == enemyPawn)
		return true;

	//Knights
	for (int koi = 0; koi < 8; koi++)
	{
		if (board.sq[kingPos + KnightOffsets[koi]] == enemyKnight)
			return true;
	}

	//King
	for (int kco = 0; kco < 8; kco++)
	{
		if (board.sq[kingPos + KingCheckOffsetsSlide[kco]] == enemyKing)
			return true;
	}

	//Others
	for (int kco = 0; kco < 8; kco++)
	{
		auto enemyExtra = otherPlayer == Player::White ? (KingCheckOffsetsSlideIsDiagonal[kco] ? Piece::WhiteBishop : Piece::WhiteRook)
													   : (KingCheckOffsetsSlideIsDiagonal[kco] ? Piece::BlackBishop : Piece::BlackRook);
		int curSq = kingPos;
		while (true)
		{
			curSq += KingCheckOffsetsSlide[kco];
			auto curSqP = board.sq[curSq];
			if (curSqP == enemyQueen || curSqP == enemyExtra)
				return true;
			if (curSqP != Piece::None)
				break;
		}
	}

	return false;
}