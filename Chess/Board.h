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
	SqN(A1,  0); SqN(A2,  1); SqN(A3,  2); SqN(A4,  3); SqN(A5,  4); SqN(A6,  5); SqN(A7,  6); SqN(A8,  7);
	SqN(B1,  8); SqN(B2,  9); SqN(B3, 10); SqN(B4, 11); SqN(B5, 12); SqN(B6, 13); SqN(B7, 14); SqN(B8, 15);
	SqN(C1, 16); SqN(C2, 17); SqN(C3, 18); SqN(C4, 19); SqN(C5, 20); SqN(C6, 21); SqN(C7, 22); SqN(C8, 23);
	SqN(D1, 24); SqN(D2, 25); SqN(D3, 26); SqN(D4, 27); SqN(D5, 28); SqN(D6, 29); SqN(D7, 30); SqN(D8, 31);
	SqN(E1, 32); SqN(E2, 33); SqN(E3, 34); SqN(E4, 35); SqN(E5, 36); SqN(E6, 37); SqN(E7, 38); SqN(E8, 39);
	SqN(F1, 40); SqN(F2, 41); SqN(F3, 42); SqN(F4, 43); SqN(F5, 44); SqN(F6, 45); SqN(F7, 46); SqN(F8, 47);
	SqN(G1, 48); SqN(G2, 49); SqN(G3, 50); SqN(G4, 51); SqN(G5, 52); SqN(G6, 53); SqN(G7, 54); SqN(G8, 55);
	SqN(H1, 56); SqN(H2, 57); SqN(H3, 58); SqN(H4, 59); SqN(H5, 60); SqN(H6, 61); SqN(H7, 62); SqN(H8, 63);
};

#undef SqN

struct Board
{
	// va -  -  Bl Wh [Piece]
	// .  .  .  .  .  .  .  .
	uint8_t sq[120]; //12 rows x 10 cols
	int en_passant_target;

	bool isValid(int i) const { return sq[i] != Piece::OffBoard; }
	bool isFree(int i) const { return sq[i] == Piece::None; }
	bool isPlayer(int i, Player p) const { return (sq[i] & (uint8_t)p) != 0; }
	bool isPawn(int i) const { return (sq[i] & PieceTypeMask) == PieceTypePawn; }
	int rankOf(int i) const { return (i / 10) - 2 + 1; }
};
 