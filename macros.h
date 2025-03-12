#ifndef MACRO_H
#define MACRO_H

#include "stdlib.h"

#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)                                                              \
  if (!(n)) {                                                                  \
    printf("%s - Failed", #n);                                                 \
    printf("On %s ", __DATE__);                                                \
    printf("At %s ", __TIME__);                                                \
    printf("In File %s ", __FILE__);                                           \
    printf("At Line %d\n", __LINE__);                                          \
    exit(1);                                                                   \
  }
#endif

#define FILERANK2SQUARE(file, rank) ((21 + (file)) + ((rank)*10))

#define SQ64(sq120) (SQUARE120TOSQUARE64[(sq120)])
#define SQ120(sq64) (SQUARE64TOSQUARE120[(sq64)])

#define IsBQ(p) (PieceBishopQueen[(p)])
#define IsRQ(p) (PieceRookQueen[(p)])
#define IsKn(p) (PieceKnight[(p)])
#define IsKi(p) (PieceKing[(p)])

#define HASH_PCE(pce, sq) (pos->posKey ^= (PieceKey[(pce)][(sq)]))
#define HASH_CA (pos->posKey ^= (CastleKeys[(pos->castlePermission)]))
#define HASH_SIDE (pos->posKey ^= (SideKey))
#define HASH_EP (pos->posKey ^= (PieceKey[EMPTY][(pos->enPassant)]))

/* GAME MOVE EXPLANATION */

/*
0000 0000 0000 0000 0000 0111 1111 -> From which square came the Move (0x7F)
0000 0000 0000 0011 1111 0000 0000 -> To which square goes the move (>>7, 0x7F)
0000 0000 0011 1100 0000 0000 0000 -> Which (if any) piece we capture (>>14,
0xF) 0000 0000 0100 0000 0000 0000 0000 -> Is it En Passant Move (0x40000) 0000
0000 1000 0000 0000 0000 0000 -> Is it the first move a pawn did? (0x80000) 0000
1111 0000 0000 0000 0000 0000 -> if promoted to which piece? (>>20, 0xF) 0001
0000 0000 0000 0000 0000 0000 -> Did we castle? (0x1000000)
*/

#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)

#define MOVEFLAGENPASSANT 0x40000
#define MOVEFLAGPAWNSTART 0x80000
#define MOVEFLAGECASTLE 0x1000000

#define MOVEFLAGCAPTURE 0x7C000
#define MOVEFLAGPROMOTED 0xF00000

#endif
