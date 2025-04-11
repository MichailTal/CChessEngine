#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char *argv[]) {
  AllInit();

  board_representation *pos = GenBoard();
  S_SEARCHINFO info[1];

  info->quit = FALSE;

  if (argc > 1) {
    if (!strcmp(argv[1], "-xboard")) {
      XBoard_Loop(pos, info);
      return 0;
    } else if (!strcmp(argv[1], "-uci")) {
      UCI_Loop(pos, info);
      return 0;
    } else if (!strcmp(argv[1], "-console")) {
      Console_Loop(pos, info);
      return 0;
    }
  }

  // Fallback: interactive stdin mode
  printf("Welcome to the Chess Engine! Type 'console', 'xboard', or 'uci'\n");

  char line[256];
  while (TRUE) {
    memset(&line[0], 0, sizeof(line));

    fflush(stdout);
    if (!fgets(line, 256, stdin))
      continue;
    if (line[0] == '\n')
      continue;

    if (!strncmp(line, "uci", 3)) {
      UCI_Loop(pos, info);
      if (info->quit == TRUE)
        break;
    } else if (!strncmp(line, "xboard", 6)) {
      XBoard_Loop(pos, info);
      if (info->quit == TRUE)
        break;
    } else if (!strncmp(line, "console", 7)) {
      Console_Loop(pos, info);
      if (info->quit)
        break;
    } else if (!strncmp(line, "quit", 4)) {
      break;
    }
  }

  CleanPolyBook();
  return 0;
}
