#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)              // Check for correct number of arguments
    {
        printf("Usage: ./substitution key\n");
        return 1;               // Errors return a 1
    }
    int i = 0;
    int j = 0;
    string v = argv[1];
    string lilV = argv[1];
    int strPro = 1;
    int strSum = 0;
    if (strlen(v) != 26)        // Check cipher length
    {
        printf("Invalid cipher: Please provide a 26 letter cipher.\n");
        return 1;
    }
    for (i = 0; i < 26; i++)
    {
        j = v[i];
        if (v[i] == 0)
        {
            break;
        }
        if ((v[i] < 65 || v[i] > 90) && (v[i] < 97 || v[i] > 122))      // Check for letters
        {
            printf("%i\n%c\n", i, v[i]);
            printf("Invalid cipher: Please provide a cipher containing only letters.\n");
            return 1;
        }
        if (v[i] > 96 && v[i] < 123)        // If char is a lowercase letter
        {
            v[i] = v[i] - 32;               // Subtract 32 to make it uppercase (not working?)
        }
        strPro = (strPro * (v[i] - 64)) % 1234543 ;    // First hash of the input cipher
        strSum = strSum + (v[i] - 64);                 // Second hash of the inputs cipher
        lilV[i] = v[i] + 32;                           // Lowercase cipher
    }
    if (strPro != 385317 || strSum != 351)             // Valid inputs will have these values
    {
        printf("Invalid cipher: Please use each letter of the alphabet only once.\n");
        return 1;
    }
    string pText = get_string("plaintext: ");   // Get user input of a plaintext string
    i = 0;
    j = 0;
    printf("ciphertext: ");                     // Output ciphertext of the user string
    while (pText[i])
    {
        if (pText[i] > 96 && pText[i] < 123)    // If lowercase, use lilV
        {
            j = pText[i] - 97;
            printf("%c", lilV[j]);
        }
        else if (pText[i] > 64 && pText[i] < 91)     // If uppercase, use v
        {
            j = pText[i] - 65;                  // Set index variable from input
            printf("%c", v[j] - 32);            // Why does it need that -32?? why??
        }
        else                                    // For all other characters
        {
            printf("%c", pText[i]);             // Just print the original input
        }
        i++;
    }
    printf("\n");
}