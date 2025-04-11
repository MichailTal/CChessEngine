#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "stdio.h"
#include "stdlib.h"

int GetPvLine(const int depth, board_representation *pos) {
  ASSERT(depth < MAXDEPTH);

  int move = ProbePvMove(pos);
  int count = 0;

  while (move != NOMOVE && count < depth) {
    ASSERT(count < MAXDEPTH);

    if (MoveExists(pos, move)) {
      MakeMove(pos, move);
      pos->PvArray[count++] = move;
    } else {
      break;
    }
    move = ProbePvMove(pos);
  }

  while (pos->ply > 0) {
    TakeMove(pos);
  }

  return count;
}

void ClearHashTable(S_HASHTABLE *table) {

  S_HASHENTRY *tableEntry;

  for (tableEntry = table->pTable;
       tableEntry < table->pTable + table->numEntries; tableEntry++) {
    tableEntry->posKey = 0ULL;
    tableEntry->move = NOMOVE;
    tableEntry->depth = 0;
    tableEntry->score = 0;
    tableEntry->flags = 0;
  }
  table->newWrite = 0;
}

void InitHashTable(S_HASHTABLE *table, const int MB) {
  int HashSize = 0x100000 * MB;
  table->numEntries = HashSize / sizeof(S_HASHENTRY);
  table->numEntries -= 2; // Prevent Out of Bound by indexing

  if (table->pTable != NULL) {
    free(table->pTable); // Free potential memory from left over PTable
  }

  table->pTable =
      (S_HASHENTRY *)malloc(table->numEntries * sizeof(S_HASHENTRY));
  
  if(table->pTable == NULL) {
    printf("Hash Allocation did fail, trying %d MB\n", MB/2);
  } else {
    ClearHashTable(table);        
  }
}


void StoreHashEntry(board_representation *pos, const int move, int score,
                    const int flags, const int depth) {

  int index = pos->posKey % pos->HashTable->numEntries;

  ASSERT(index >= 0 &&
         index <= pos->HashTable->numEntries - 1); // Checks calculation above
  ASSERT(depth >= 1 && depth < MAXDEPTH);
  ASSERT(flags >= HFALPHA && flags <= HFEXACT);
  ASSERT(score >= -INFINITE && score <= INFINITE);
  ASSERT(pos->ply >= 0 && pos->ply < MAXDEPTH);

  if (pos->HashTable->pTable[index].posKey == 0) {
    pos->HashTable->newWrite++;
  } else {
    pos->HashTable->overWrite++;
  }

  if (score > ISMATE)
    score += pos->ply;
  else if (score < -ISMATE)
    score -= pos->ply;

  pos->HashTable->pTable[index].move = move;
  pos->HashTable->pTable[index].posKey = pos->posKey;
  pos->HashTable->pTable[index].flags = flags;
  pos->HashTable->pTable[index].score = score;
  pos->HashTable->pTable[index].depth = depth;
}

int ProbeHashEntry(board_representation *pos, int *move, int *score, int alpha,
                   int beta, int depth) {

  int index = pos->posKey % pos->HashTable->numEntries;

  ASSERT(index >= 0 &&
         index <= pos->HashTable->numEntries - 1); // Checks calculation above
  ASSERT(depth >= 1 && depth < MAXDEPTH);
  ASSERT(alpha < beta);
  ASSERT(alpha >= -INFINITE && alpha <= INFINITE);
  ASSERT(beta >= -INFINITE && beta <= INFINITE);
  ASSERT(pos->ply >= 0 && pos->ply < MAXDEPTH);

  if (pos->HashTable->pTable[index].posKey == pos->posKey) {
    *move = pos->HashTable->pTable[index].move;
    if (pos->HashTable->pTable[index].depth >= depth) {
      pos->HashTable->hit++;

      ASSERT(pos->HashTable->pTable[index].depth >= 1 &&
             pos->HashTable->pTable[index].depth < MAXDEPTH);
      ASSERT(pos->HashTable->pTable[index].flags >= HFALPHA &&
             pos->HashTable->pTable[index].flags <= HFEXACT);

      *score = pos->HashTable->pTable[index].score;
      if (*score > ISMATE)
        *score -= pos->ply;
      else if (*score < -ISMATE)
        *score += pos->ply;

      switch (pos->HashTable->pTable[index].flags) {
        ASSERT(*score >= -INFINITE && *score <= INFINITE);

      case HFALPHA:
        if (*score <= alpha) {
          *score = alpha;
          return TRUE;
        }
        break;
      case HFBETA:
        if (*score >= beta) {
          *score = beta;
          return TRUE;
        }
        break;
      case HFEXACT:
        return TRUE;
        break;
      default:
        ASSERT(FALSE);
        break;
      }
    }
  }

  return FALSE;
}

int ProbePvMove(const board_representation *pos) {

  int index = pos->posKey % pos->HashTable->numEntries;
  ASSERT(index >= 0 && index <= pos->HashTable->numEntries - 1);

  if (pos->HashTable->pTable[index].posKey == pos->posKey) {
    return pos->HashTable->pTable[index].move;
  }

  return NOMOVE;
}
