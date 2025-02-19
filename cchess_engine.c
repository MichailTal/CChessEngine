#include <stdio.h>
#include "definitions.h"
#include "macros.h"
#include "globals.h"
#include "init.h"

int main()
{
    AllInit();

    U64 playBitBoard = 0LL;

    printf("Start: \n\n");
    PrintBitBoard(playBitBoard);


    playBitBoard |= (1ULL << SQUARE120TOSQUARE64[A2]);

    printf("A2 added: \n\n");
    PrintBitBoard(playBitBoard);

    playBitBoard |= (1ULL << SQUARE120TOSQUARE64[G7]);

    printf("G7 added: \n\n");
    PrintBitBoard(playBitBoard);


    return 0;
}