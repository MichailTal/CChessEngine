#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "stdlib.h"

int GetPvLine(const int depth, board_representation *pos) {
  ASSERT(depth < MAXDEPTH);

  int move = ProbePvTable(pos);
  int count = 0;

  while (move != NOMOVE && count < depth) {
    ASSERT(count < MAXDEPTH);

    if (MoveExists(pos, move)) {
      MakeMove(pos, move);
      pos->PvArray[count++] = move;
    } else {
      break;
    }
    move = ProbePvTable(pos);
  }

  while (pos->ply > 0) {
    TakeMove(pos);
  }

  return count;
}

// 2 MB init size
const int PVSize = 0x100000 * 2;

void ClearPvTable(S_PVTABLE *table) {

  S_PVENTRY *pvEntry;

  for (pvEntry = table->pTable; pvEntry < table->pTable + table->numEntries;
       pvEntry++) {
    pvEntry->posKey = 0ULL;
    pvEntry->move = NOMOVE;
  }
}

void InitPvTable(S_PVTABLE *table) {
  table->numEntries = PVSize / sizeof(S_PVENTRY);
  table->numEntries -= 2; // Prevent Out of Bound by indexing

  if (table->pTable != NULL) {
    free(table->pTable); // Free potential memory from left over PTable
  }

  table->pTable = (S_PVENTRY *)malloc(table->numEntries * sizeof(S_PVENTRY));
  ClearPvTable(table);
  printf("PvTable init complete with %d entries\n", table->numEntries);
}

void StorePvMove(const board_representation *pos, const int move) {

  int index = pos->posKey % pos->PvTable->numEntries;

  ASSERT(index >= 0 &&
         index <= pos->PvTable->numEntries - 1); // Checks calculation above

  pos->PvTable->pTable[index].move = move;
  pos->PvTable->pTable[index].posKey = pos->posKey;
}

int ProbePvTable(const board_representation *pos) {

  int index = pos->posKey % pos->PvTable->numEntries;

  ASSERT(index >= 0 &&
         index <= pos->PvTable->numEntries - 1); // Checks calculation above

  if (pos->PvTable->pTable[index].posKey == pos->posKey) {
    return pos->PvTable->pTable[index].move;
  }

  return NOMOVE;
}
