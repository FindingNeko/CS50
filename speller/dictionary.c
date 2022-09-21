// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 46933;

// Hash table
node *table[N];

// Word counter

int wc = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Assigns tmp, verifies it isn't NULL, and increments it to next
    for (node *tmp  = table[hash(word)]; tmp != NULL; tmp = tmp->next)
    {
        // Return true if the word is found
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int token = 1;
    for (int i = strlen(word) - 1; i >= 0; i--)
    {
        token = token * toupper(word[i]) % 46933;
    }
    return token;
}


// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    char *tmp = malloc(sizeof(char) * (LENGTH + 1));
    FILE *dict = fopen(dictionary, "r");
    // Check for NULL
    if (dict == NULL)
    {
        free(tmp);
        return false;
    }
    if (tmp == NULL)
    {
        fclose(dict);
        return false;
    }
    // copy words over to tmp one at a time using fscanf
    while (fscanf(dict, "%s", tmp) == 1)
    {
        // Use malloc to get a new node
        node *new = malloc(sizeof(node));
        // Check for NULL and free tmp if found
        if (new == NULL)
        {
            free(tmp);
            return false;
        }
        // copy tmp to new word
        strcpy(new->word, tmp);
        int prehash = hash(tmp);
        // Check for NULL and update pointers
        if (table[prehash] != NULL)
        {
            new->next = table[hash(tmp)];
            table[prehash] = new;
        }
        else
        {
            new->next = NULL;
            table[prehash] = new;
        }
        // Increment word count
        wc++;
    }
    // Memory cleanup
    free(tmp);
    fclose(dict);
    return 1;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return wc;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    int i = 0;
    while (i < N)
    {
        if (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
        i++;
    }
    if (i == N)
    {
        return true;
    }
    return false;
}