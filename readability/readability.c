#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int count_letters(string);      // Letter counting function
int count_words(string);        // Word counting function
int count_sentences(string);    // Sentence counting function

int main(void)
{
    string userText = get_string("Text: ");             // Get user input
    float letterCount = count_letters(userText);        // Initialize letter count variable
    float wordCount = count_words(userText);            // Initialize words count variable
    float sentenceCount = count_sentences(userText);    // Initialize sentence count variable
    float lCstar = 100 * letterCount / wordCount;       // Letter count per 100 words
    float sCstar = 100 * sentenceCount / wordCount;     // Sentence count per 100 words
    float gradeLevelTemp = ((0.0588 * lCstar) - (0.296 * sCstar) - 15.8) * 10; // Coleman-Liau Index
    int gradeLevel = gradeLevelTemp;    // Grade level is an int to simplify output and rounding
    if (gradeLevel % 10 >= 5)
    {
        // Rounds up if first decimal place of grade level is >= 5
        gradeLevel = (gradeLevelTemp / 10) + 1;
    }
    else
    {
        // This will round down to the nearest int
        gradeLevel = gradeLevelTemp / 10;
    }
    if (gradeLevel >= 16)
    {
        // Special case for grades of 15.5 or higher (includes 15.5)
        printf("Grade 16+\n");
        return 0;
    }
    // Special case for grades lower than 1
    else if (gradeLevel < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }
    else
    {
        printf("Grade %i\n", gradeLevel);   // Print grade level
    }
}


/*
* counts the letters in a string by incrementing a counter when it checks a position that
* contains a letter by comparing ascii values to a range. Other characters are not counted.
*/
int count_letters(string text)
{
    int i = 0;
    int n = 0;
    while (text[i])
    {
        if ((text[i] > 64 && text[i] < 91) || (text[i] > 96 && text[i] < 123))
        {
            n++;
        }
        i++;
    }
    return n;
}
/*
* Counts the words in a string by incrementing a counter when it checks a position that
* contains a space, after checking a position containing a letter.
*/
int count_words(string text)
{
    int i = 0;
    int n = 0;
    bool f = 0;
    while (text[i])
    {
        if ((text[i] > 64 && text[i] < 91) || (text[i] > 96 && text[i] < 123))
        {
            f = 1;
        }
        else if (text[i] == 32)
        {
            n++;
            f = 0;
        }
        i++;
    }
    return n + 1;
}
/*
* Counts the sentences in a string by incrementing a counter when it checks a position that
* contains a . ! or ? after a sequence containing at least one letter.
*/
int count_sentences(string text)
{
    int i = 0;
    int n = 0;
    bool f = 0;
    while (text[i])
    {
        if ((text[i] > 64 && text[i] < 91) || (text[i] > 96 && text[i] < 123))
        {
            f = 1;
        }
        else if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            n++;
            f = 0;
        }
        i++;
    }
    return n;
}