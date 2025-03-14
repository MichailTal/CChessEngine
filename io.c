#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

char *PrintSquare(const int square) {
  static char SqStr[3];

  int file = FilesBrd[square];
  int rank = RanksBrd[square];

  sprintf(SqStr, "%c%c", ('a' + file), ('1' + file));

  return SqStr;
}

char *PrintMove(const int move) {
  static char MvStr[6];

  int filefrom = FilesBrd[FROMSQ(move)];
  int rankfrom = RanksBrd[FROMSQ(move)];
  int fileto = FilesBrd[TOSQ(move)];
  int rankto = RanksBrd[TOSQ(move)];

  int promoted = PROMOTED(move);
  int captured = CAPTURED(move);

  if (promoted) {
    char promotechar = 'q';
    if (IsKn(promoted)) {
      promotechar = 'n';
    } else if (IsRQ(promoted) && !IsBQ(promoted)) {
      promotechar = 'r';
    } else if (!IsRQ(promoted) && IsBQ(promoted)) {
      promotechar = 'b';
    }
    sprintf(MvStr, "%c%c%c%c%c", ('a' + filefrom), ('1' + rankfrom),
            ('a' + fileto), ('1' + rankto), promotechar);
  } else {
    sprintf(MvStr, "%c%c%c%c", ('a' + filefrom), ('1' + rankfrom),
            ('a' + fileto), ('1' + rankto));
  }

  if (captured) {
    sprintf(MvStr, "%c%cx%c%c", ('a' + filefrom), ('1' + rankfrom),
            ('a' + fileto), ('1' + rankto));
  }

  return MvStr;
}

void PrintMoveList(const move_list *list) {
  int index = 0;
  int score = 0;
  int move = 0;
  printf("MoveList:\n");

  for (index = 0; index < list->count; ++index) {

    move = list->moves[index].move;
    score = list->moves[index].score;

    printf("Move:%d > %s (score:%d)\n", index + 1, PrintMove(move), score);
  }
  printf("MoveList Total %d Moves:\n\n", list->count);
}

int ParseMove(char *ptrChar, board_representation *pos) {

  if (ptrChar[1] > '8' || ptrChar[1] < '1') return NOMOVE;
  if (ptrChar[3] > '8' || ptrChar[3] < '1') return NOMOVE;
  if (ptrChar[0] > 'h' || ptrChar[0] < 'a') return NOMOVE;
  if (ptrChar[2] > 'h' || ptrChar[2] < 'a') return NOMOVE;

  int from = FILERANK2SQUARE(ptrChar[0] - 'a', ptrChar[1] - '1');
  int to = FILERANK2SQUARE(ptrChar[2] - 'a', ptrChar[3] - '1');

  printf("ptrChar: %s from: %d, to: %d, \n", ptrChar, from, to);

  ASSERT(SqOnBoard(from) && SqOnBoard(to));

  move_list list[1];
  GenerateAllMoves(pos, list);

  int MoveNum = 0;
  int Move = 0;
  int PromPce = EMPTY;
  
  for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
		Move = list->moves[MoveNum].move;
		if(FROMSQ(Move)==from && TOSQ(Move)==to) {
			PromPce = PROMOTED(Move);
      // handling the promotion cases
			if(PromPce!=EMPTY) {
				if(IsRQ(PromPce) && !IsBQ(PromPce) && ptrChar[4]=='r') {
					return Move;
				} else if(!IsRQ(PromPce) && IsBQ(PromPce) && ptrChar[4]=='b') {
					return Move;
				} else if(IsRQ(PromPce) && IsBQ(PromPce) && ptrChar[4]=='q') {
					return Move;
				} else if(IsKn(PromPce) && ptrChar[4]=='n') {
					return Move;
				}
				continue;
			}
			return Move;
		}
  }
    return NOMOVE;
}
