#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "string.h"

int ThreeFoldRep(const board_representation *pos) {
  int i = 0;
  int r = 0;

  for (i = 0; i < pos->histPly; ++i) {
    if (pos->moveHistory[i].posKey == pos->posKey) {
      r++;
    }
  }
  return r;
}

int DrawMaterial(const board_representation *pos) {
  if (pos->pieceNumber[wP] || pos->pieceNumber[bP])
    return FALSE; // no draw if pawns are still there
  if (pos->pieceNumber[wQ] || pos->pieceNumber[bQ] || pos->pieceNumber[wR] ||
      pos->pieceNumber[bR])
    return FALSE; // no draw if one or both sides has Rock and Queen
  if (pos->pieceNumber[wB] > 1 || pos->pieceNumber[bB] > 1) {
    return FALSE;
  }
  if (pos->pieceNumber[wN] > 1 || pos->pieceNumber[bN] > 1) {
    return FALSE;
  }
  if (pos->pieceNumber[wN] && pos->pieceNumber[wB]) {
    return FALSE;
  }
  if (pos->pieceNumber[bN] && pos->pieceNumber[bB]) {
    return FALSE;
  }

  return TRUE;
}

int Checkresult(board_representation *pos) {

  if (pos->fiftyMoveRule > 100) {
    printf("1/2-1/2 {fity move rule claimed by engine}\n");
    return TRUE;
  }

  if (ThreeFoldRep(pos) >= 2) {
    printf("1/2-1/2 {threefold claimed by engine}\n");
  }

  if (DrawMaterial(pos) == TRUE) {
    printf("1/2-1/2 {insufficient material claimed by engine}\n");
  }

  move_list list[1];
  GenerateAllMoves(pos, list);

  int MoveNum = 0;
  int found = 0;

  for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {

    if (!MakeMove(pos, list->moves[MoveNum].move)) {
      continue;
    }
    found++;
    TakeMove(pos);
    break;
  }

  if (found != 0)
    return FALSE;

  int InCheck = SqAttacked(pos->kingSquare[pos->side], pos->side ^ 1, pos);

  if (InCheck == TRUE) {
    if (pos->side == WHITE) {
      printf("0-1 {black mates according to engine}\n");
      return TRUE;
    } else {
      printf("1-0 {white mates according to engine}\n");
      return TRUE;
    }
  } else {
    printf("1/2-1/2 {stalemate claimed by engine}\n");
    return TRUE;
  }

  return FALSE;
}

void PrintOptions(void) {
  printf("feature ping=1 setboard=1 colors=0 usermove=1\n");
  printf("feature done=1\n");
}

void XBoard_Loop(board_representation *pos, S_SEARCHINFO *info) {

  info->GAME_MODE = XBOARDMODE;
  info->POST_THINKING = TRUE;
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  PrintOptions();

  int depth = -1, movestogo[2] = {30, 30}, movetime = -1;
  int time = -1, inc = 0;
  int engineSide = BOTH;
  int timeLeft;
  int sec;
  int mps;
  int move = NOMOVE;
  char inBuf[80], command[80];
  int MB;

  engineSide = BLACK;
  ParseFen(START_FEN, pos);
  depth = -1;
  time = -1;

  while (TRUE) {

    fflush(stdout);

    if (pos->side == engineSide && Checkresult(pos) == FALSE) {
      info->starttime = GetTimeMs();
      info->depth = depth;

      if (time != -1) {
        info->timeset = TRUE;
        time /= movestogo[pos->side];
        time -= 50;
        info->stoptime = info->starttime + time + inc;
      }

      if (depth == -1 || depth > MAXDEPTH) {
        info->depth = MAXDEPTH;
      }

      printf(
          "time:%d start:%d stop:%d depth:%d timeset:%d movestogo:%d mps:%d\n",
          time, info->starttime, info->stoptime, info->depth, info->timeset,
          movestogo[pos->side], mps);
      SearchPosition(pos, info);

      if (mps != 0) {
        movestogo[pos->side ^ 1]--;
        if (movestogo[pos->side ^ 1] < 1) {
          movestogo[pos->side ^ 1] = mps;
        }
      }
    }

    fflush(stdout);

    memset(&inBuf[0], 0, sizeof(inBuf));
    fflush(stdout);
    if (!fgets(inBuf, 80, stdin))
      continue;

    sscanf(inBuf, "%s", command);

    printf("command seen:%s\n", inBuf);

    if (!strcmp(command, "quit")) {
      info->quit = TRUE;
      break;
    }

    if (!strcmp(command, "force")) {
      engineSide = BOTH;
      continue;
    }

    if (!strcmp(command, "protover")) {
      PrintOptions();
      continue;
    }

    if (!strcmp(command, "sd")) {
      sscanf(inBuf, "sd %d", &depth);
      printf("DEBUG depth:%d\n", depth);
      continue;
    }

    if (!strcmp(command, "st")) {
      sscanf(inBuf, "st %d", &movetime);
      printf("DEBUG movetime:%d\n", movetime);
      continue;
    }

    if (!strcmp(command, "time")) {
      sscanf(inBuf, "time %d", &time);
      time *= 10;
      printf("DEBUG time:%d\n", time);
      continue;
    }

    if (!strcmp(command, "level")) {
      sec = 0;
      movetime = -1;
      if (sscanf(inBuf, "level %d %d %d", &mps, &timeLeft, &inc) != 3) {
        sscanf(inBuf, "level %d %d:%d %d", &mps, &timeLeft, &sec, &inc);
        printf("DEBUG level with :\n");
      } else {
        printf("DEBUG level without :\n");
      }
      timeLeft *= 60000;
      timeLeft += sec * 1000;
      movestogo[0] = movestogo[1] = 30;
      if (mps != 0) {
        movestogo[0] = movestogo[1] = mps;
      }
      time = -1;
      printf("DEBUG level timeLeft:%d movesToGo:%d inc:%d mps%d\n", timeLeft,
             movestogo[0], inc, mps);
      continue;
    }

    if (!strcmp(command, "ping")) {
      printf("pong%s\n", inBuf + 4);
      continue;
    }

    if (!strcmp(command, "new")) {
      ClearHashTable(pos->HashTable);
      engineSide = BLACK;
      ParseFen(START_FEN, pos);
      depth = -1;
      time = -1;
      continue;
    }

    if (!strcmp(command, "setboard")) {
      engineSide = BOTH;
      ParseFen(inBuf + 9, pos);
      continue;
    }

    if (!strcmp(command, "go")) {
      engineSide = pos->side;
      continue;
    }

    if (!strcmp(command, "usermove")) {
      movestogo[pos->side]--;
      move = ParseMove(inBuf + 9, pos);
      if (move == NOMOVE)
        continue;
      MakeMove(pos, move);
      pos->ply = 0;
    }

    if (!strcmp(command, "memory")) {
      sscanf(inBuf, "memory %d", &MB);
      if (MB < 4)
        MB = 4;
      if (MB > 2048)
        MB = 2048;
      printf("Set Hash to %d MB\n", MB);
      InitHashTable(pos->HashTable, MB);
      continue;
    }

    if (!strcmp(command, "depth")) { // For Lichess
      sscanf(inBuf, "depth %d", &depth);
      printf("DEBUG depth set via depth cmd: %d\n", depth);
      continue;
    }
  }
}

void Console_Loop(board_representation *pos, S_SEARCHINFO *info) {

  printf("Welcome to the Console Mode!\n");
  printf("Type help for commands\n\n");

  info->GAME_MODE = CONSOLEMODE;
  info->POST_THINKING = TRUE;
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);

  int depth = MAXDEPTH, movetime = 3000;
  int engineSide = BOTH;
  int move = NOMOVE;
  char inBuf[80], command[80];

  engineSide = BLACK;
  ParseFen(START_FEN, pos);

  while (TRUE) {

    fflush(stdout);

    if (pos->side == engineSide && Checkresult(pos) == FALSE) {
      info->starttime = GetTimeMs();
      info->depth = depth;

      if (movetime != 0) {
        info->timeset = TRUE;
        info->stoptime = info->starttime + movetime;
      }

      SearchPosition(pos, info);
    }

    printf("\nEngine > ");

    fflush(stdout);

    memset(&inBuf[0], 0, sizeof(inBuf));
    fflush(stdout);
    if (!fgets(inBuf, 80, stdin))
      continue;

    sscanf(inBuf, "%s", command);

    if (!strcmp(command, "help")) {
      printf("Commands:\n");
      printf("quit - quit game\n");
      printf("force - computer will not think\n");
      printf("print - show board\n");
      printf("post - show thinking\n");
      printf("nopost - do not show thinking\n");
      printf("new - start new game\n");
      printf("go - set computer thinking\n");
      printf("depth x - set depth to x\n");
      printf("time x - set thinking time to x seconds (depth still applies if "
             "set)\n");
      printf("view - show current depth and movetime settings\n");
      printf("setboard x - set position to fen x\n");
      printf("bookoff - disables the book usage");
      printf("bookon - enables the book usage");
      printf("** note ** - to reset time and depth, set to 0\n");
      printf("enter moves using b7b8q notation\n\n\n");
      continue;
    }

    if (!strcmp(command, "eval")) {
      PrintBoard(pos);
      printf("Eval:%d", EvalPosition(pos));
      continue;
    }

    if (!strcmp(command, "setboard")) {
      engineSide = BOTH;
      ParseFen(inBuf + 9, pos);
      continue;
    }

    if (!strcmp(command, "quit")) {
      info->quit = TRUE;
      break;
    }

    if (!strcmp(command, "post")) {
      info->POST_THINKING = TRUE;
      continue;
    }

    if (!strcmp(command, "print")) {
      PrintBoard(pos);
      continue;
    }

    if (!strcmp(command, "nopost")) {
      info->POST_THINKING = FALSE;
      continue;
    }

    if (!strcmp(command, "force")) {
      engineSide = BOTH;
      continue;
    }

    if (!strcmp(command, "view")) {
      if (depth == MAXDEPTH)
        printf("depth not set ");
      else
        printf("depth %d", depth);

      if (movetime != 0)
        printf(" movetime %ds\n", movetime / 1000);
      else
        printf(" movetime not set\n");

      if (EngineOptions->UseBook == FALSE) {
        printf("Book disabled");
      } else {
        printf("Book enabled");
      }

      continue;
    }

    if (!strcmp(command, "depth")) {
      sscanf(inBuf, "depth %d", &depth);
      if (depth == 0)
        depth = MAXDEPTH;
      continue;
    }

    if (!strcmp(command, "time")) {
      sscanf(inBuf, "time %d", &movetime);
      movetime *= 1000;
      continue;
    }

    if (!strcmp(command, "new")) {
      ClearHashTable(pos->HashTable);
      engineSide = BLACK;
      ParseFen(START_FEN, pos);
      continue;
    }

    if (!strcmp(command, "go")) {
      engineSide = pos->side;
      continue;
    }

    if (!strcmp(command, "takeback")) {
      TakeMove(pos);
      TakeMove(pos);
      continue;
    }

    if (!strcmp(command, "bookoff")) {
      printf("Book disabled\n");
      EngineOptions->UseBook = FALSE;
      continue;
    }

    if (!strcmp(command, "bookon")) {
      printf("Book enabled\n");
      EngineOptions->UseBook = TRUE;
      continue;
    }

    if (!strcmp(command, "mirror")) {
      PrintBoard(pos);
      printf("Evaluation: %d\n", EvalPosition(pos));
      MirrorBoard(pos);
      PrintBoard(pos);
      printf("Evaluation (Mirror): %d\n", EvalPosition(pos));
      MirrorBoard(pos);
      continue;
    }

    move = ParseMove(inBuf, pos);
    if (move == NOMOVE) {
      printf("Command unknown:%s\n", inBuf);
      continue;
    }
    MakeMove(pos, move);
    pos->ply = 0;
  }
}
