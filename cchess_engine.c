#include <stdio.h>
#include "definitions.h"
#include "macros.h"
#include "globals.h"
#include "init.h"

int main()
{
    AllInit();

    int index = 0;

    // Print SQUARE120TOSQUARE64
    for (index = 0; index < BRD_SQ_NUM; index++)
    {
        if (index % 10 == 0)
        {
            printf("\n");
        }
        printf("%5d", SQUARE120TOSQUARE64[index]);
    }

    printf("\n\n");

    // Print SQUARE64TOSQUARE120
    for (index = 0; index < 64; index++)
    {
        if (index % 8 == 0)
        {
            printf("\n");
        }
        printf("%5d", SQUARE64TOSQUARE120[index]);
    }

    printf("\n");

    return 0;
}