#ifndef DEFS_H
#define DEFS_H

typedef unsigned long long U64;

#define NAME "CCHESS v1.0"
#define BRD_SQ_NUM 120
#define MAXGAMEHALFMOVES 2048
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define CLRBIT(bb, sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb, sq) ((bb) |= SetMask[(sq)])

// Piece Definitions

enum { EMPTY, wP, wQ, wK, wR, wB, wN, bP, bQ, bK, bR, bB, bN };

// File Definition

enum {
  FILE_A,
  FILE_B,
  FILE_C,
  FILE_D,
  FILE_E,
  FILE_F,
  FILE_G,
  FILE_H,
  FILE_NONE
};

// Rank Definition
enum {
  RANK_1,
  RANK_2,
  RANK_3,
  RANK_4,
  RANK_5,
  RANK_6,
  RANK_7,
  RANK_8,
  RANK_NONE
};

// Colours
enum { WHITE, BLACK, BOTH };

// Square Definitions
enum {
  A1 = 21,
  B1,
  C1,
  D1,
  E1,
  F1,
  G1,
  H1,
  A2 = 31,
  B2,
  C2,
  D2,
  E2,
  F2,
  G2,
  H2,
  A3 = 41,
  B3,
  C3,
  D3,
  E3,
  F3,
  G3,
  H3,
  A4 = 51,
  B4,
  C4,
  D4,
  E4,
  F4,
  G4,
  H4,
  A5 = 61,
  B5,
  C5,
  D5,
  E5,
  F5,
  G5,
  H5,
  A6 = 71,
  B6,
  C6,
  D6,
  E6,
  F6,
  G6,
  H6,
  A7 = 81,
  B7,
  C7,
  D7,
  E7,
  F7,
  G7,
  H7,
  A8 = 91,
  B8,
  C8,
  D8,
  E8,
  F8,
  G8,
  H8,
  NO_SQ,
  OFFBOARD
};

// Python Defintion of Bool
enum { FALSE, TRUE };

enum {
  whiteKingCastle = 1,
  whiteQueenCastle = 2,
  blackKingCastle = 4,
  blackQueenCastle = 8
};

// Undo moves
typedef struct {
  int move;
  int castlePermission;
  int enPassant;
  int fiftyMoveRule;
  U64 posKey;
} S_UNDO;

typedef struct {
  int pieces[BRD_SQ_NUM];
  U64 pawns[3];      // stores the pawn positions on the respective rank
  int kingSquare[2]; // positions of the two kings, this is safe as kings are
                     // always present

  int side;          // which side to move
  int enPassant;     // fields on which En Passant is possible
  int fiftyMoveRule; // keeps track of the fifty move rule
  int ply;           // number of 'half moves'
  int histPly;       // number of half moves made for determining a repetition

  U64 posKey; // unique key for each position

  int pieceNumber[13]; // Number of pieces on the board
  int nonPawnPieces[2];
  int majorPieces[2]; // Rooks and Queen(s)
  int minorPieces[2]; // Bishops and Knights
  int material[2];

  int castlePermission;

  S_UNDO moveHistory[MAXGAMEHALFMOVES];

  int pieceList[13][10]; // piece list

} board_representation;
#endif
