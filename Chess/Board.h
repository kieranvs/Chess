#pragma once

#include <stdint.h>

constexpr uint8_t PieceTypePawn = 0x1;
constexpr uint8_t PieceTypeRook = 0x2;
constexpr uint8_t PieceTypeKnight = 0x3;
constexpr uint8_t PieceTypeBishop = 0x4;
constexpr uint8_t PieceTypeQueen = 0x5;
constexpr uint8_t PieceTypeKing = 0x6;

constexpr uint8_t PieceTypeMask = 0x7;

constexpr uint8_t PieceTypeWhite = 0x8;
constexpr uint8_t PieceTypeBlack = 0x10;

constexpr uint8_t PieceTypeInvalid = 0x80;

enum class Player : uint8_t
{
	White = PieceTypeWhite,
	Black = PieceTypeBlack
};

struct Piece
{
	static constexpr uint8_t WhitePawn = PieceTypeWhite | PieceTypePawn;
	static constexpr uint8_t WhiteRook = PieceTypeWhite | PieceTypeRook;
	static constexpr uint8_t WhiteKnight = PieceTypeWhite | PieceTypeKnight;
	static constexpr uint8_t WhiteBishop = PieceTypeWhite | PieceTypeBishop;
	static constexpr uint8_t WhiteQueen = PieceTypeWhite | PieceTypeQueen;
	static constexpr uint8_t WhiteKing = PieceTypeWhite | PieceTypeKing;
	static constexpr uint8_t BlackPawn = PieceTypeBlack | PieceTypePawn;
	static constexpr uint8_t BlackRook = PieceTypeBlack | PieceTypeRook;
	static constexpr uint8_t BlackKnight = PieceTypeBlack | PieceTypeKnight;
	static constexpr uint8_t BlackBishop = PieceTypeBlack | PieceTypeBishop;
	static constexpr uint8_t BlackQueen = PieceTypeBlack | PieceTypeQueen;
	static constexpr uint8_t BlackKing = PieceTypeBlack | PieceTypeKing;
	static constexpr uint8_t None = 0x00;
	static constexpr uint8_t OffBoard = PieceTypeInvalid;
};

constexpr int mailbox120[120] =
{
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
     -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
     -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
     -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
     -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
     -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
     -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
     -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

constexpr int mailbox64[64] =
{
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
};

#define SqN(name, i) static constexpr int name = mailbox64[i];

struct SquareName
{
	SqN(A1,  0); SqN(A2,  8); SqN(A3, 16); SqN(A4, 24); SqN(A5, 32); SqN(A6, 40); SqN(A7, 48); SqN(A8, 56);
	SqN(B1,  1); SqN(B2,  9); SqN(B3, 17); SqN(B4, 25); SqN(B5, 33); SqN(B6, 41); SqN(B7, 49); SqN(B8, 57);
	SqN(C1,  2); SqN(C2, 10); SqN(C3, 18); SqN(C4, 26); SqN(C5, 34); SqN(C6, 42); SqN(C7, 50); SqN(C8, 58);
	SqN(D1,  3); SqN(D2, 11); SqN(D3, 19); SqN(D4, 27); SqN(D5, 35); SqN(D6, 43); SqN(D7, 51); SqN(D8, 59);
	SqN(E1,  4); SqN(E2, 12); SqN(E3, 20); SqN(E4, 28); SqN(E5, 36); SqN(E6, 44); SqN(E7, 52); SqN(E8, 60);
	SqN(F1,  5); SqN(F2, 13); SqN(F3, 21); SqN(F4, 29); SqN(F5, 37); SqN(F6, 45); SqN(F7, 53); SqN(F8, 61);
	SqN(G1,  6); SqN(G2, 14); SqN(G3, 22); SqN(G4, 30); SqN(G5, 38); SqN(G6, 46); SqN(G7, 54); SqN(G8, 62);
	SqN(H1,  7); SqN(H2, 15); SqN(H3, 23); SqN(H4, 31); SqN(H5, 39); SqN(H6, 47); SqN(H7, 55); SqN(H8, 63);
};

#undef SqN

constexpr uint8_t CastleRightsWhiteKingside = 0x1;
constexpr uint8_t CastleRightsWhiteQueenside = 0x2;
constexpr uint8_t CastleRightsWhite = 0x3;
constexpr uint8_t CastleRightsBlackKingside = 0x4;
constexpr uint8_t CastleRightsBlackQueenside = 0x8;
constexpr uint8_t CastleRightsBlack = 0xC;
constexpr uint8_t CastleRightsAll = 0xF;
constexpr uint8_t CastleRightsNone = 0x0;

struct Board
{
	// va -  -  Bl Wh [Piece]
	// .  .  .  .  .  .  .  .
	uint8_t sq[120]; //12 rows x 10 cols
	int en_passant_target;
	// - - - - BQ BK WQ WK
	uint8_t castle_rights;

	bool isValid(int i) const { return sq[i] != Piece::OffBoard; }
	bool isFree(int i) const { return sq[i] == Piece::None; }
	bool isPlayer(int i, Player p) const { return (sq[i] & (uint8_t)p) != 0; }
	bool isPawn(int i) const { return (sq[i] & PieceTypeMask) == PieceTypePawn; }
	bool isKing(int i) const { return (sq[i] & PieceTypeMask) == PieceTypeKing; }
	int rankOf(int i) const { return (i / 10) - 2 + 1; }
};

static_assert(sizeof(Board) == 128);
 