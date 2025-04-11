#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "../include/polykeys.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct {
  U64 key;
  unsigned short move;
  unsigned short weight;
  unsigned int learn;
} S_POLY_BOOK_ENTRY;

long NumEntries = 0;

S_POLY_BOOK_ENTRY *entries;

const int PolyKindOfPiece[13] = {-1, 1, 3, 5, 7, 9, 11, 0, 2, 4, 6, 8, 10};

void InitPolyBook(void) {
  char filePath[256];

  EngineOptions->UseBook = FALSE;

  printf("Enter the path to the book file: ");
  if (fgets(filePath, sizeof(filePath), stdin) == NULL) {
    fprintf(stderr, "Error reading file path\n");
    return;
  }

  // Remove newline character from the input
  size_t len = strlen(filePath);
  if (len > 0 && filePath[len - 1] == '\n') {
    filePath[len - 1] = '\0';
  }

  FILE *pFile = fopen(filePath, "rb");
  if (pFile == NULL) {
    fprintf(stderr, "Error opening file: %s\n", filePath);
    return;
  }

  fseek(pFile, 0, SEEK_END);
  long position = ftell(pFile);
  if (position < sizeof(S_POLY_BOOK_ENTRY)) {
    fprintf(stderr, "No entries found\n");
    fclose(pFile);
    return;
  }

  NumEntries = position / sizeof(S_POLY_BOOK_ENTRY);
  printf("%ld entries found in file\n", NumEntries);

  entries = (S_POLY_BOOK_ENTRY *)malloc(NumEntries * sizeof(S_POLY_BOOK_ENTRY));
  if (entries == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    fclose(pFile);
    return;
  }

  rewind(pFile);
  size_t returnValue =
      fread(entries, sizeof(S_POLY_BOOK_ENTRY), NumEntries, pFile);
  if (returnValue != NumEntries) {
    fprintf(stderr, "Error reading entries from file\n");
  } else {
    printf("fread() %ld entries in file\n", returnValue);
  }

  EngineOptions->UseBook = TRUE;

  fclose(pFile);
}

void CleanPolyBook(void) { free(entries); }

int HasPawnForCapture(board_representation *pos) {
  int squareWithPawn = 0;
  int targetPiece = (pos->side == WHITE) ? wP : bP;

  if (pos->enPassant != NO_SQ) {
    if (pos->side == WHITE) {
      squareWithPawn = pos->enPassant - 10;
    } else {
      squareWithPawn = pos->enPassant + 10;
    }

    if (pos->pieces[squareWithPawn + 1] == targetPiece) {
      return TRUE;
    } else if (pos->pieces[squareWithPawn - 1] == targetPiece) {
      return TRUE;
    }
  }

  return FALSE;
}

U64 PolyKeyFromBoard(board_representation *pos) {

  int square = 0;
  int rank = 0;
  int file = 0;
  U64 finalKey = 0;
  int piece = EMPTY;
  int polyPiece = 0;
  int offset = 0;

  for (square = 0; square < BRD_SQ_NUM; ++square) {
    piece = pos->pieces[square];

    if (piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
      ASSERT(piece >= wP && piece <= bK);
      polyPiece = PolyKindOfPiece[piece];
      rank = RanksBrd[square];
      file = FilesBrd[square];
      finalKey ^= Random64Poly[(64 * polyPiece) + (8 * rank) + file];
    }
  }

  // Castling

  offset = 768;

  if (pos->castlePermission & whiteKingCastle)
    finalKey ^= Random64Poly[offset + 0];
  if (pos->castlePermission & whiteQueenCastle)
    finalKey ^= Random64Poly[offset + 1];
  if (pos->castlePermission & blackKingCastle)
    finalKey ^= Random64Poly[offset + 2];
  if (pos->castlePermission & blackQueenCastle)
    finalKey ^= Random64Poly[offset + 3];

  // En Passant

  offset = 772;

  if (HasPawnForCapture(pos) == TRUE) {
    file = FilesBrd[pos->enPassant];
    finalKey ^= Random64Poly[offset + file];
  }

  // Side to Move

  if (pos->side == WHITE) {
    finalKey ^= Random64Poly[780];
  }

  return finalKey;
}

unsigned short endian_swap_u16(unsigned short x) {
  x = (x >> 8) | (x << 8);
  return x;
}

unsigned int endian_swap_u32(unsigned int x) {
  x = (x >> 24) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) | (x << 24);
  return x;
}

U64 endian_swap_u64(U64 x) {
  x = (x >> 56) | ((x << 40) & 0x00FF000000000000) |
      ((x << 24) & 0x0000FF0000000000) | ((x << 8) & 0x000000FF00000000) |
      ((x >> 8) & 0x00000000FF000000) | ((x >> 24) & 0x0000000000FF0000) |
      ((x >> 40) & 0x000000000000FF00) | (x << 56);
  return x;
}

int ConvertPolyMoveToInternalMove(unsigned short polyMove,
                                  board_representation *pos) {

  int fromfile = (polyMove >> 6) & 7;
  int fromrank = (polyMove >> 9) & 7;
  int tofile = (polyMove >> 0) & 7;
  int torank = (polyMove >> 3) & 7;
  int promotionpiece = (polyMove >> 12) & 7;

  char moveString[6];

  if (promotionpiece == 0) {
    sprintf(moveString, "%c%c%c%c", FileCharacter[fromfile],
            RankCharacter[fromrank], FileCharacter[tofile],
            RankCharacter[torank]);
  } else {
    char promChar = 'q';
    switch (promotionpiece) {
    case 1:
      promChar = 'n';
      break;
    case 2:
      promChar = 'b';
      break;
    case 3:
      promChar = 'r';
      break;
    }
    sprintf(moveString, "%c%c%c%c%c", FileCharacter[fromfile],
            RankCharacter[fromrank], FileCharacter[tofile],
            RankCharacter[torank], promChar);
  }
  return ParseMove(moveString, pos);
}

int GetBookMoves(board_representation *pos) {
  int index = 0;
  S_POLY_BOOK_ENTRY *entry;
  unsigned short move;
  const int MAXBOOKMOVES = 32;
  int bookMoves[MAXBOOKMOVES];
  int tempMove = NOMOVE;
  int counter = 0;

  U64 polyKey = PolyKeyFromBoard(pos);

  for (entry = entries; entry < entries + NumEntries; entry++) {
    if (polyKey == endian_swap_u64(entry->key)) {
      move = endian_swap_u16(entry->move);
      tempMove = ConvertPolyMoveToInternalMove(move, pos);

      if (tempMove != NOMOVE) {
        bookMoves[counter++] = tempMove;
        if (counter > MAXBOOKMOVES)
          break;
      }
    }
    index++;
  }

  if (counter != 0) {
    int randMove = rand() % counter;
    return bookMoves[randMove];
  } else {
    return NOMOVE;
  }
}
