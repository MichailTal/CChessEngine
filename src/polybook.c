#include "../include/definitions.h"
#include "../include/globals.h"
#include "../include/init.h"
#include "../include/macros.h"
#include "../include/polykeys.h"

U64 PolyKeyFromBoard(board_representation *board) {
    return Random64Poly[400];
}