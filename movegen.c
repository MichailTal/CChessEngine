#include "definitions.h"
#include "globals.h"
#include "init.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

void AddQuietMove(const board_representation *pos, int move, move_list *list) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list->count++;
}

void AddCaptureMove(const board_representation *pos, int move, move_list *list) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list->count++;
}

void AddEnPassantMove(const board_representation *pos, int move, move_list *list) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list->count++;
}

void GenerateAllMoves(const board_representation *pos, move_list *list) {
    list -> count = 0;
}