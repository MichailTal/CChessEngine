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
