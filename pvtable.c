#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

// 2 MB init size
const int PVSize = 0x100000 * 2;

static void ClearPvTable(S_PVTABLE *table) {

    S_PVENTRY *pvEntry;

    for (pvEntry = table -> pTable; pvEntry < table -> pTable + table -> numEntries; pvEntry++) {
        pvEntry -> posKey = 0ULL;
        pvEntry -> move = NOMOVE;
    }
}

void InitPvTable(S_PVTABLE *table) {
    table -> numEntries = PVSize / sizeof(S_PVENTRY);
    table -> numEntries -= 2; // Prevent Out of Bound by indexing

    if(table->pTable!=NULL) {
		free(table->pTable); // Free potential memory from left over PTable
	}

    table -> pTable = (S_PVENTRY *) malloc(table -> numEntries * sizeof(S_PVENTRY));
    ClearPvTable(table);
    printf("PvTable init complete with %d entries\n", table -> numEntries);
}