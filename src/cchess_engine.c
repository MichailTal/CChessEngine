#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define WAC1                                                                   \
  "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

int main() {
  AllInit();

  board_representation *pos = GenBoard();
  S_SEARCHINFO info[1];

  printf("Welcome to the Chess Engine! Type 'console' for the terminal mode\n");

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
      continue;
    } else if (!strncmp(line, "xboard", 6)) {
      XBoard_Loop(pos, info);
      if (info->quit == TRUE)
        break;
      continue;
    } else if (!strncmp(line, "console", 7)) {
      Console_Loop(pos, info);
      if (info->quit)
        break;
      continue;
    } else if (!strncmp(line, "quit", 4)) {
      break;
    }
  }

  return 0;
}
