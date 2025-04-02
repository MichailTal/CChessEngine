#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "string.h"

#define INPUTBUFFER 400 * 6

void ParseGo(char *line, S_SEARCHINFO *info, board_representation *pos){};

// position fen fenstr
// position startpos
// ... moves e2e4 e7e5 b7b8q
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

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);

  char line[INPUTBUFFER];

  // See: https://www.wbec-ridderkerk.nl/html/UCIProtocol.html
  printf("id name %s\n", NAME);
  printf("id author Nils\n");
  printf("uci ok\n");

  InitPvTable(pos->PvTable);

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
      ParsePosition("position startpos\n", pos);
    } else if (!strncmp(line, "go", 2)) {
      ParseGo(line, info, pos);
    } else if (!strncmp(line, "quit", 4)) {
      info->quit = TRUE;
      break;
    } else if (!strncmp(line, "uci", 3)) {
      printf("id name %s\n", NAME);
      printf("id author Nils\n");
      printf("uci ok\n");
    }
    if (info->quit)
      break;
  }
}
