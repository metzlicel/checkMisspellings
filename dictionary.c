// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 4050;

// Hash table
node *table[N];

//Initialize counter of nodes (words)
int counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    node *cursor = table[index];

    //While linked lists
    for (cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //Take lenght of word
    int lenght = strlen(word);
    char upper[lenght];
    int key = 0;

    for (int i = 0; i < lenght; i++)
    {
        //Change every word to uppercase
        if (isalpha(word[i]))
        {
            upper[i] = toupper(word[i]);
        }

        //Apostrophes or anything that isn't a word, doesn't count
        else if ((word[i] == '\0') || (!isalpha(word[i])))
        {
            key = 0;
        }
        //Add every ASCII value from every letter
        key = upper[i] + key;

        //Multiply that sum time the lenght of word
        key = key * lenght;
    }
    //Divide key between number of buckets
    return key % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    //Create a buffer
    char word_dict[LENGTH + 1];

    //Read word by word from dictionary
    while ((fscanf(file, "%s", word_dict)) != EOF)
    {
        node *new_word = malloc(sizeof(node));
        if (new_word == NULL)
        {
            return false;
        }

        //Copy word from dictionary to node
        strcpy(new_word->word, word_dict);
        new_word->next = NULL;

        //Hash funtion into copy of word
        int index = hash(new_word->word);

        //Assing copy of word into hash table
        if (counter == 0)
        {
            table[index] = new_word;
            counter++;
        }
        else
        {
            new_word->next = table[index];
            table[index] = new_word;
            counter++;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Return number of words being count on 'load' function
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //Go through every node and free from last to first
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
        free(cursor);
    }
    return true;
}
