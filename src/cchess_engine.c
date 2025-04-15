#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char *argv[]) {
  const char *bookPath = NULL;    // Default: no book
  EngineOptions->UseBook = FALSE; // Default: book disabled

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-book") && i + 1 < argc) {
      bookPath = argv[i + 1];
      EngineOptions->UseBook = TRUE;
      i++;
    } else if (!strcmp(argv[i], "-nobook")) {
      EngineOptions->UseBook = FALSE;
    }
  }

  AllInit(bookPath);

  board_representation *pos = GenBoard();
  S_SEARCHINFO info[1];
  info->quit = FALSE;

  // Mode selection
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-xboard")) {
      XBoard_Loop(pos, info);
      return 0;
    } else if (!strcmp(argv[i], "-uci")) {
      UCI_Loop(pos, info);
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
    } else if (!strncmp(line, "quit", 4)) {
      break;
    }
  }

  CleanPolyBook();
  return 0;
}
