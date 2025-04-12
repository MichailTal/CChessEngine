#ifndef DEFS_H
#define DEFS_H

typedef unsigned long long U64;

#define NAME "CCHESS v1.0"
#define BRD_SQ_NUM 120
#define MAXGAMEHALFMOVES 2048
#define MAXPOSITIONMOVES 256
#define MAXDEPTH 64
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define CLRBIT(bb, sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb, sq) ((bb) |= SetMask[(sq)])

#define INFINITE 30000

#define ISMATE (INFINITE - MAXDEPTH)

#define MAX_HASH 1024

// Piece Definitions

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };

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

// Engine Modes
enum { UCIMODE, XBOARDMODE, CONSOLEMODE };

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

enum { HFNONE, HFALPHA, HFBETA, HFEXACT };

// Undo moves
typedef struct {
  int move;
  int castlePermission;
  int enPassant;
  int fiftyMoveRule;
  U64 posKey;
} S_UNDO;

// Principle Variation Structs
typedef struct {
  U64 posKey;
  int move;
  int score;
  int depth;
  int flags;
} S_HASHENTRY;

typedef struct {
  S_HASHENTRY *pTable;
  int numEntries;
  int newWrite;
  int overWrite;
  int hit;
  int cut;
} S_HASHTABLE;

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
  int bigPce[2];       // Basically is it a pawn or not
  int majPce[2];       // Rooks and Queen(s)
  int minPce[2];       // Bishops and Knights
  int material[2];

  int castlePermission;

  S_UNDO moveHistory[MAXGAMEHALFMOVES];

  S_HASHTABLE HashTable[1];
  int PvArray[MAXDEPTH];

  int pieceList[13][10]; // piece list

  // Only for non-captures
  int searchHistory[13][BRD_SQ_NUM];
  int searchKillers[2][MAXDEPTH]; // Two moves that were the most recent beta
                                  // cut offs

} board_representation;

typedef struct {
  int move;
  int score;
} move_representation;

typedef struct {
  move_representation moves[MAXPOSITIONMOVES];
  int count;
} move_list;

typedef struct {

  int starttime;
  int stoptime;
  int depth;
  int depthset;
  int timeset;
  int movestogo;
  int infinite;

  long nodes;

  int quit;
  int stopped;

  float failhigh;
  float failhighfirst;
  int nullCut;

  int GAME_MODE;
  int POST_THINKING;

} S_SEARCHINFO;

typedef struct {
  int UseBook;
} S_OPTIONS;

#endif
