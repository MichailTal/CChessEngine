#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

const int CastlePerm[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 13, 15, 15, 15, 12, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 7,  15, 15, 15, 3,  15, 15, 11, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15};

static void ClearPiece(const int square, board_representation *pos) {

    ASSERT(SqOnBoard(square));
    ASSERT(CheckBoard(pos));
    
    int piece = pos->pieces[square];
    
    ASSERT(PieceValid(piece));
    
    int colour = PieceColour[piece];
    int index = 0;
    int t_pceNum = -1;
    
    ASSERT(SideValid(colour));
    
    HASH_PCE(piece,square);
    
    pos->pieces[square] = EMPTY;
    pos->material[colour] -= PieceValue[piece];
    
    if(PieceBig[piece]) {
            pos->bigPce[colour]--;
        if(PieceMajor[piece]) {
            pos->majPce[colour]--;
        } else {
            pos->minPce[colour]--;
        }
    } else {
        CLRBIT(pos->pawns[colour],SQ64(square));
        CLRBIT(pos->pawns[BOTH],SQ64(square));
    }
    
    for(index = 0; index < pos->pieceNumber[piece]; ++index) {
        if(pos->pieceList[piece][index] == square) {
            t_pceNum = index;
            break;
        }
    }
    
    ASSERT(t_pceNum != -1);
    ASSERT(t_pceNum>=0&&t_pceNum<10);
    
    pos->pieceNumber[piece]--;		
    
    pos->pieceList[piece][t_pceNum] = pos->pieceList[piece][pos->pieceNumber[piece]];
    
}

static void AddPiece(const int sq, board_representation *pos, const int pce) {

    ASSERT(PieceValid(pce));
    ASSERT(SqOnBoard(sq));
	
	int col = PieceColour[pce];
	ASSERT(SideValid(col));

    HASH_PCE(pce,sq);
	
	pos->pieces[sq] = pce;

    if(PieceBig[pce]) {
			pos->bigPce[col]++;
		if(PieceMajor[pce]) {
			pos->majPce[col]++;
		} else {
			pos->minPce[col]++;
		}
	} else {
		SETBIT(pos->pawns[col],SQ64(sq));
		SETBIT(pos->pawns[BOTH],SQ64(sq));
	}
	
	pos->material[col] += PieceValue[pce];
	pos->pieceList[pce][pos->pieceNumber[pce]++] = sq;
	
}

static void MovePiece(const int from, const int to, board_representation *pos) {

  ASSERT(SqOnBoard(from));
  ASSERT(SqOnBoard(to));

  int index = 0;
  int piece = pos->pieces[from];
  int colour = PieceColour[piece];

#ifdef DEBUG
  int t_PieceNum = FALSE;
#endif

  HASH_PCE(piece, from);
  pos->pieces[from] = EMPTY;

  HASH_PCE(piece, to);
  pos->pieces[to] = piece;

  // e.g. it is a pawn
  if (!PieceBig[piece]) {
    CLRBIT(pos->pawns[colour], SQ64(from));
    CLRBIT(pos->pawns[BOTH], SQ64(from));
    SETBIT(pos->pawns[colour], SQ64(to));
    SETBIT(pos->pawns[BOTH], SQ64(to));
  }

  for (index = 0; index < pos->pieceNumber[piece]; ++index) {
    if (pos->pieceList[piece][index] == from) {
      pos->pieceList[piece][index] = to;
#ifdef DEBUG
      t_PieceNum = TRUE;
#endif
      break;
    }
  }
  ASSERT(t_PieceNum);
}

int MakeMove(board_representation *pos, int move) {

	ASSERT(CheckBoard(pos));
	
	int from = FROMSQ(move);
    int to = TOSQ(move);
    int side = pos->side;
	
	ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(SideValid(side));
    ASSERT(PieceValid(pos->pieces[from]));
	
	pos->moveHistory[pos->histPly].posKey = pos->posKey;
	
	if(move & MOVEFLAGENPASSANT) {
        if(side == WHITE) {
            ClearPiece(to-10,pos);
        } else {
            ClearPiece(to+10,pos);
        }
    } else if (move & MOVEFLAGECASTLE) {
        switch(to) {
            case C1:
                MovePiece(A1, D1, pos);
			break;
            case C8:
                MovePiece(A8, D8, pos);
			break;
            case G1:
                MovePiece(H1, F1, pos);
			break;
            case G8:
                MovePiece(H8, F8, pos);
			break;
            default: ASSERT(FALSE); break;
        }
    }	
	
	if(pos->enPassant != NO_SQ) HASH_EP;
    HASH_CA;
	
	pos->moveHistory[pos->histPly].move = move;
    pos->moveHistory[pos->histPly].fiftyMoveRule = pos->fiftyMoveRule;
    pos->moveHistory[pos->histPly].enPassant = pos->enPassant;
    pos->moveHistory[pos->histPly].castlePermission = pos->castlePermission;

    pos->castlePermission &= CastlePerm[from];
    pos->castlePermission &= CastlePerm[to];
    pos->enPassant = NO_SQ;

	HASH_CA;
	
	int captured = CAPTURED(move);
    pos->fiftyMoveRule++;
	
	if(captured != EMPTY) {
        ASSERT(PieceValid(captured));
        ClearPiece(to, pos);
        pos->fiftyMoveRule = 0;
    }
	
	pos->histPly++;
	pos->ply++;
	
	if(PiecePawn[pos->pieces[from]]) {
        pos->fiftyMoveRule = 0;
        if(move & MOVEFLAGENPASSANT) {
            if(side==WHITE) {
                pos->enPassant=from+10;
                ASSERT(RanksBrd[pos->enPassant]==RANK_3);
            } else {
                pos->enPassant=from-10;
                ASSERT(RanksBrd[pos->enPassant]==RANK_6);
            }
            HASH_EP;
        }
    }
	
	MovePiece(from, to, pos);
	
	int prPce = PROMOTED(move);
    if(prPce != EMPTY)   {
        ASSERT(PieceValid(prPce) && !PiecePawn[prPce]);
        ClearPiece(to, pos);
        AddPiece(to, pos, prPce);
    }
	
	if(PieceKing[pos->pieces[to]]) {
        pos->kingSquare[pos->side] = to;
    }
	
	pos->side ^= 1;
    HASH_SIDE;

    ASSERT(CheckBoard(pos));
	
		
	if(SqAttacked(pos->kingSquare[side],pos->side,pos))  {
        TakeMove(pos);
        return FALSE;
    }
	
	return TRUE;
	
}

void TakeMove(board_representation *pos) {
	
	ASSERT(CheckBoard(pos));
	
	pos->histPly--;
    pos->ply--;
	
    int move = pos->moveHistory[pos->histPly].move;
    int from = FROMSQ(move);
    int to = TOSQ(move);	
	
	ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
	
	if(pos->enPassant != NO_SQ) HASH_EP;
    HASH_CA;

    pos->castlePermission = pos->moveHistory[pos->histPly].castlePermission;
    pos->fiftyMoveRule = pos->moveHistory[pos->histPly].fiftyMoveRule;
    pos->enPassant = pos->moveHistory[pos->histPly].enPassant;

    if(pos->enPassant != NO_SQ) HASH_EP;
    HASH_CA;

    pos->side ^= 1;
    HASH_SIDE;
	
	if(MOVEFLAGENPASSANT & move) {
        if(pos->side == WHITE) {
            AddPiece(to-10, pos, bP);
        } else {
            AddPiece(to+10, pos, wP);
        }
    } else if(MOVEFLAGECASTLE & move) {
        switch(to) {
            case C1: MovePiece(D1, A1, pos); break;
            case C8: MovePiece(D8, A8, pos); break;
            case G1: MovePiece(F1, H1, pos); break;
            case G8: MovePiece(F8, H8, pos); break;
            default: ASSERT(FALSE); break;
        }
    }
	
	MovePiece(to, from, pos);
	
	if(PieceKing[pos->pieces[from]]) {
        pos->kingSquare[pos->side] = from;
    }
	
	int captured = CAPTURED(move);
    if(captured != EMPTY) {
        ASSERT(PieceValid(captured));
        AddPiece(to, pos, captured);
    }
	
	if(PROMOTED(move) != EMPTY)   {
        ASSERT(PieceValid(PROMOTED(move)) && !PiecePawn[PROMOTED(move)]);
        ClearPiece(from, pos);
        AddPiece(from, pos, (PieceColour[PROMOTED(move)] == WHITE ? wP : bP));
    }
	
    ASSERT(CheckBoard(pos));

}
