#include <cs50.h>
#include <stdio.h>

/*
* This program makes a set of block stairs as in mario. The user provides an input
* from 1 to 8 inclusive, and the program makes the stairs at the height requested.
*/

int main(void)
{
    int numBricks = 0;                      /* Height */
    int i = 1;                              /* Counters i,j,k */
    int j = 1;
    int k = 1;
    while (numBricks < 1 | numBricks > 8)  /* Loop until input is 1:8 */
    {
        numBricks = get_int("Height: ");    /* Request Height from user */
    }
    for (i = 1; i <= numBricks; i++)        /* New line loop */
    {
        for (j = i; j < numBricks; j++)     /* First space loop */
        {
            printf(" ");
        }
        for (k = 1; k <= i; k++)            /* First # loop */
        {
            printf("#");
        }
        printf("  ");                       /* Print middle spaces */
        for (k = 1; k <= i; k++)            /* Second # loop */
        {
            printf("#");
        }
        printf("\n");
    }

}