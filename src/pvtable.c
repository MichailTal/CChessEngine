#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "../include/tinycthread.h"
#include "stdio.h"
#include "stdlib.h"

#define EXTRACT_SCORE(x) ((x & 0xFFFF) - INF_BOUND)
#define EXTRACT_DEPTH(x) ((x >> 16) & 0x3F)
#define EXTRACT_FLAGS(x) ((x >> 23) & 0x3)
#define EXTRACT_MOVE(x) ((int)(x >> 25))

#define FOLD_DATA(score, depth, flag, move)                                    \
  ((score + INF_BOUND) | (depth << 16) | (flag << 23) | ((U64)move << 25))

S_HASHTABLE HashTable[1];

void VerifyEntrySMP(S_HASHENTRY *entry) {
  U64 data = FOLD_DATA(entry->score, entry->depth, entry->flags, entry->move);
  U64 key = entry->posKey ^ data;

  if(data != entry -> smp_data) {printf("data error"); exit(1);}
  if(key != entry -> smp_key) {printf("key error"); exit(1);}

  int move = EXTRACT_MOVE(data);
  int flag = EXTRACT_FLAGS(data);
  int depth = EXTRACT_DEPTH(data);
  int score = EXTRACT_SCORE(data);

  if(move != entry -> move) {printf("move error %s", PrintMove(move)); exit(1);}
  if(flag != entry -> flags) {printf("flag error %d", flag); exit(1);}
  if(depth != entry -> depth) {printf("depth error %d", depth); exit(1);}
  if(score != entry -> score) {printf("score error %d", score); exit(1);}
  
}

int GetPvLine(const int depth, board_representation *pos,
              const S_HASHTABLE *table) {
  ASSERT(depth <= MAXDEPTH);

  int move = ProbePvMove(pos, table);
  int count = 0;

  while (move != NOMOVE && count < depth) {
    ASSERT(count <= MAXDEPTH);

    if (MoveExists(pos, move)) {
      MakeMove(pos, move);
      pos->PvArray[count++] = move;
    } else {
      break;
    }
    move = ProbePvMove(pos, table);
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
    tableEntry->age = 0;
    tableEntry->smp_data = 0ULL;
    tableEntry->smp_key = 0ULL;
  }
  table->newWrite = 0;
  table->currentage = 0;
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

  if (table->pTable == NULL) {
    printf("Hash Allocation did fail, trying %d MB\n", MB / 2);
  } else {
    ClearHashTable(table);
  }
}

void StoreHashEntry(board_representation *pos, S_HASHTABLE *table,
                    const int move, int score, const int flags,
                    const int depth) {

  int index = pos->posKey % table->numEntries;

  ASSERT(index >= 0 &&
         index <= table->numEntries - 1); // Checks calculation above
  ASSERT(depth >= 1 && depth <= MAXDEPTH);
  ASSERT(flags >= HFALPHA && flags <= HFEXACT);
  ASSERT(score >= -AB_BOUND && score <= AB_BOUND);
  ASSERT(pos->ply >= 0 && pos->ply <= MAXDEPTH);

  int replace = FALSE;

  if (table->pTable[index].posKey == 0) {
    table->newWrite++;
    replace = TRUE;
  } else {
    if (table->pTable[index].age < table->currentage ||
        table->pTable[index].depth <= depth) {
      replace = TRUE;
    }
  }

  if (replace == FALSE)
    return;

  if (score > ISMATE)
    score += pos->ply;
  else if (score < -ISMATE)
    score -= pos->ply;

  U64 smp_data = FOLD_DATA(score, depth, flags, move);
  U64 smp_key = pos -> posKey ^ smp_data;

  table->pTable[index].move = move;
  table->pTable[index].posKey = pos->posKey;
  table->pTable[index].flags = flags;
  table->pTable[index].score = score;
  table->pTable[index].depth = depth;
  table->pTable[index].age = table->currentage;
  table->pTable[index].smp_data = smp_data;
  table->pTable[index].smp_key = smp_key;

  VerifyEntrySMP(&table -> pTable[index]);

}

int ProbeHashEntry(board_representation *pos, S_HASHTABLE *table, int *move,
                   int *score, int alpha, int beta, int depth) {

  int index = pos->posKey % table->numEntries;

  ASSERT(index >= 0 &&
         index <= table->numEntries - 1); // Checks calculation above
  ASSERT(depth >= 1 && depth <= MAXDEPTH);
  ASSERT(alpha < beta);
  ASSERT(alpha >= -AB_BOUND && alpha <= AB_BOUND);
  ASSERT(beta >= -AB_BOUND && beta <= AB_BOUND);
  ASSERT(pos->ply >= 0 && pos->ply <= MAXDEPTH);

  if (table->pTable[index].posKey == pos->posKey) {
    *move = table->pTable[index].move;
    if (table->pTable[index].depth >= depth) {
      table->hit++;

      ASSERT(table->pTable[index].depth >= 1 &&
             table->pTable[index].depth <= MAXDEPTH);
      ASSERT(table->pTable[index].flags >= HFALPHA &&
             table->pTable[index].flags <= HFEXACT);

      *score = table->pTable[index].score;
      if (*score > ISMATE)
        *score -= pos->ply;
      else if (*score < -ISMATE)
        *score += pos->ply;

      ASSERT(*score >= -AB_BOUND && *score <= AB_BOUND);
      switch (table->pTable[index].flags) {

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

int ProbePvMove(const board_representation *pos, const S_HASHTABLE *table) {

  int index = pos->posKey % table->numEntries;
  ASSERT(index >= 0 && index <= table->numEntries - 1);

  if (table->pTable[index].posKey == pos->posKey) {
    return table->pTable[index].move;
  }

  return NOMOVE;
}
