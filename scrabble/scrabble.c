#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet, plus an extra 0 at the end of the list.
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10, 0};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    int score = 0;
    int i = 0;
    int cur = 0;
    while (word[i]) // Watches for the null char at the end of the string
    {
        if (word[i] > 64 && word[i] < 91)           // Uppercase letters
        {
            cur = word[i] - 65;                     // Convert to POINTS format
        }
        else if (word[i] > 96 && word[i] < 123)     // Lowercase letters
        {
            cur = word[i] - 97;                     // Convert to POINTS format
        }
        else                                        // All other chars
        {
            cur = 26;                               // This is that zero value from POINTS
        }
        score = score + POINTS[cur];                // Tally the points
        i++;
    }
    return score;
}
