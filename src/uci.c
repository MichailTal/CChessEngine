#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "../include/tinycthread.h"
#include "stdio.h"
#include "string.h"

#define INPUTBUFFER 400 * 6

thrd_t MainSearchThread;

thrd_t LaunchSearchThread(board_representation *pos, S_SEARCHINFO *info, S_HASHTABLE *table) {
  S_SEARCH_THREAD_DATA *pSearchData = malloc(sizeof(S_SEARCH_THREAD_DATA));

  pSearchData -> pos = pos;
  pSearchData -> info = info;
  pSearchData -> ttable = table;

  thrd_t th;
  thrd_create(&th, &SearchPositionThread, (void*)pSearchData);

  return th;
}

void JoinSearchThread(S_SEARCHINFO *info) {
  info -> stopped = TRUE;
  thrd_join(MainSearchThread, NULL);
}

// go depth 6 wtime 180000 btime 100000 binc 1000 winc 1000 movetime 1000
// movestogo 40
void ParseGo(char *line, S_SEARCHINFO *info, board_representation *pos, S_HASHTABLE *table) {

  int depth = -1, movestogo = 30, movetime = -1;
  int time = -1, inc = 0;
  char *ptr = NULL;
  info->timeset = FALSE;

  if ((ptr = strstr(line, "infinite"))) {
    ;
  }

  if ((ptr = strstr(line, "binc")) && pos->side == BLACK) {
    inc = atoi(ptr + 5);
  }

  if ((ptr = strstr(line, "winc")) && pos->side == WHITE) {
    inc = atoi(ptr + 5);
  }

  if ((ptr = strstr(line, "wtime")) && pos->side == WHITE) {
    time = atoi(ptr + 6);
  }

  if ((ptr = strstr(line, "btime")) && pos->side == BLACK) {
    time = atoi(ptr + 6);
  }

  if ((ptr = strstr(line, "movestogo"))) {
    movestogo = atoi(ptr + 10);
  }

  if ((ptr = strstr(line, "movetime"))) {
    movetime = atoi(ptr + 9);
  }

  if ((ptr = strstr(line, "depth"))) {
    depth = atoi(ptr + 6);
  }

  if (movetime != -1) {
    time = movetime;
    movestogo = 1;
  }

  info->starttime = GetTimeMs();
  info->depth = depth;

  if (time != -1) {
    info->timeset = TRUE;
    time /= movestogo;
    time -= 50;
    info->stoptime = info->starttime + time + inc;
  }

  if (depth == -1) {
    info->depth = MAXDEPTH;
  }

  printf("time:%d start:%d stop:%d depth:%d timeset:%d\n", time,
         info->starttime, info->stoptime, info->depth, info->timeset);
  //SearchPosition(pos, info, table);
  MainSearchThread = LaunchSearchThread(pos, info, table);
}

void ParsePosition(char *lineIn, board_representation *pos) {

  lineIn += 9;
  char *ptrChar = lineIn;

  if (strncmp(lineIn, "startpos", 8) == 0) {
    ParseFen(START_FEN, pos);
  } else {
    ptrChar = strstr(lineIn, "fen");
    if (ptrChar == NULL) {
      ParseFen(START_FEN, pos);
    } else {
      ptrChar += 4;
      ParseFen(ptrChar, pos);
    }
  }

  ptrChar = strstr(lineIn, "moves");
  int move;

  if (ptrChar != NULL) {
    ptrChar += 6;
    while (*ptrChar) {
      move = ParseMove(ptrChar, pos);
      if (move == NOMOVE)
        break;
      MakeMove(pos, move);
      pos->ply = 0;
      while (*ptrChar && *ptrChar != ' ')
        ptrChar++;
      ptrChar++;
    }
  }
  PrintBoard(pos);
}

void UCI_Loop(board_representation *pos, S_SEARCHINFO *info) {

  info->GAME_MODE = UCIMODE;

  setbuf(stdin, NULL);
  setbuf(stdout, NULL);

  char line[INPUTBUFFER];
  printf("option name Hash type spin default 64 min 4 max %d\n", MAX_HASH);
  printf("option name Book type check default true\n");
  printf("uciok\n");

  int MB = 64;

  while (TRUE) {
    memset(&line[0], 0, sizeof(line));
    fflush(stdout);
    if (!fgets(line, INPUTBUFFER, stdin))
      continue;

    if (line[0] == '\n')
      continue;

    if (!strncmp(line, "isready", 7)) {
      printf("readyok\n");
      continue;
    } else if (!strncmp(line, "position", 8)) {
      ParsePosition(line, pos);
    } else if (!strncmp(line, "ucinewgame", 10)) {
      ClearHashTable(HashTable);
      ParsePosition("position startpos\n", pos);
    } else if (!strncmp(line, "run", 3)) {
      ParseFen(START_FEN, pos);
      ParseGo("go infinte", info, pos, HashTable);
    } else if (!strncmp(line, "quit", 4)) {
      info -> quit = TRUE;
      JoinSearchThread(info);
      break;
    } else if (!strncmp(line, "stop", 4)) {
      JoinSearchThread(info);
    } else if (!strncmp(line, "go", 2)) {
      printf("Seen Go..\n");
      ParseGo(line, info, pos, HashTable);
    } else if (!strncmp(line, "uci", 3)) {
      printf("uciok\n");
    } else if (!strncmp(line, "setoption name Hash value ", 26)) {
      sscanf(line, "%*s %*s %*s %*s %d", &MB);
      if (MB < 4)
        MB = 4;
      if (MB > MAX_HASH)
        MB = MAX_HASH;
      printf("Set Hash to %d MB\n", MB);
      InitHashTable(HashTable, MB);
    } else if (!strncmp(line, "setoption name Book value ", 26)) {
      char *ptrTrue = NULL;
      ptrTrue = strstr(line, "true");
      if (ptrTrue != NULL) {
        EngineOptions->UseBook = TRUE;
      } else {
        EngineOptions->UseBook = FALSE;
      }
    }
    if (info->quit)
      break;
  }
}
