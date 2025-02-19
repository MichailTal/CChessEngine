#include <stdio.h>
#include "definitions.h"
#include "init.h"
#include "macros.h"
#include "globals.h"


void PrintBitBoard(U64 bb) {
    U64 shiftMe = 1ULL;

    int rank = 0;
    int file = 0;
    int square = 0;
    int square64 = 0;

    printf("\n");
    for (rank  = RANK_8; rank >= RANK_1; rank--) 
    {
        for (file = FILE_A; file <= FILE_H; file++)
        {
            square = FILERANK2SQUARE(file, rank); // 120 based index
            square64 = SQUARE120TOSQUARE64[square]; // 64 based
            if ((shiftMe << square64) & bb)
            {
                printf("X");
            }
            else
            {
                printf("-");
            }
            
        }
        printf("\n");
    }
    printf("\n\n");


}