#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*
* This program forensicly analyzes a raw bytestream for jpg files and saves them
*/

// Defines a 8 bit unsigned int as a BYTE for ease of use and clarity later
typedef uint8_t BYTE;

// Main function takes one argument
int main(int argc, char *argv[])
{
    // If more than one argument is provided, lodge a complaint with the user
    if (argc != 2)
    {
        printf("Usage: ./recover {fornsic image file name}");
        return 1;
    }

    // Declare the block size for reading, and a flag boolean for use later
    // when writing found jpg data to a file
    int BLOCK_SIZE = 512;
    bool writeFlag = false;

    // Open the file at the location provided by the user and check for load
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not load /'%s/'", argv[1]);
        return 1;
    }

    // Get memory for a char * that stores the new file names, check for success
    char *fname = malloc(sizeof(char) * 8);
    if (fname == NULL)
    {
        printf("Memory error, malloc failed.");
        return 1;
    }

    // Get memory for a 512 byte array, check for success
    BYTE *tmp = malloc(sizeof(BYTE) * 512);
    if (tmp == NULL)
    {
        printf("Memory error, malloc failed.");
        return 1;
    }

    // Initialize a file name counter, create the name, create the first file
    int fcount = 0;
    sprintf(fname, "%03i.jpg", fcount);
    FILE *img = fopen(fname, "w");

    // Read from the raw file until you can't anymore
    while (fread(tmp, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // If first 4 bytes match the jpg header:
        if (tmp[0] == 0xff && tmp[1] == 0xd8 && tmp[2] == 0xff && tmp[3] > 0xdf && tmp[3] < 0xf0)
        {
            // and the writeFlag is true:
            if (writeFlag == true)
            {
                // Close current img file, increment count, update fname
                fclose(img);
                fcount++;
                sprintf(fname, "%03i.jpg", fcount);
            }
            // open a new img file with name 'fname', set writeFlag to true
            img = fopen(fname, "w");
            writeFlag = true;
        }
        // Don't write if header hasn't been found yet, write if it has
        if (writeFlag == false)
        {
            continue;
        }
        fwrite(tmp, BLOCK_SIZE, 1, img);
    }
    // Close and free all the things, return 0 because success
    fclose(img);
    fclose(file);
    free(tmp);
    free(fname);
    return 0;
}