// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <cs50.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char *word;
    struct node *next;
}
Node;

// Buckets, node and unloads counters
const unsigned int N = 26;
unsigned int node_counter = 0;
unsigned int unloads = 0;

// Hash table for first and second letters; accomodate three more sub-buckets for \n [index 28] and ' [index 27], and \0 [index 26]
Node *Table[N][N + 3];

// Array to check for existing nodes
bool exists[N][N + 3];

// Free memory, second hash code, and filter non-alphabeticals functions
void free_list(Node *ptr);
unsigned int hash2(const char *word);

void populate(void)
{
    // Set all exists-or-not to false; set non-malloc'd buckets to NULL
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N + 3; j++)
        {
            Table[i][j] = NULL;
            exists[i][j] = false;
        }
    }
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // COMPUTE HASH CODES
    unsigned int first = hash(word);
    unsigned int second = hash2(word);

    Node *n = Table[first][second];
    if (n == NULL)
    {
        return false;
    }
    while (n != NULL)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
        n = n->next;
    }
    return false;
}

// Hashes word to a number between 0 and 675
unsigned int hash(const char *word)
{
    // Index for Table
    return tolower(word[0]) - 'a';
}

unsigned int hash2(const char *word)
{
    // Compute \n and ' separately; send to the last two sub-buckets
    if (word[1] == '\n')
    {
        return 28;
    }
    else if (word[1] == '\'')
    {
        return 27;
    }
    else if (word[1] == '\0')
    {
        return 26;
    }
    else
    {
        return tolower(word[1]) - 'a';
    }
}


// Loads dictionary into memory, returning true if successful, else false. // INPUT IS FILE NAME
bool load(const char *dictionary)
{
    populate();
    // dictionary is name of file; open it and check for NULL
    FILE *diction = fopen(dictionary, "r");
    if (diction == NULL) return false;

    // Declare a buffer string and temporary/placeholder pointers
    char *buffer = malloc(sizeof(char) * (LENGTH + 1));
    Node *n = NULL;
    Node *NODE;
    // Null terminator character might be implicitly present in fgets and may not be included in the char size.
    while (fscanf(diction, "%s", buffer) != EOF)
    {
        // COMPUTE HASH CODES
        unsigned int first = hash(buffer);
        unsigned int second = hash2(buffer);

        // LOAD (PREAPPEND) AND INCREASE # OF NODES LOADED
        Table[first][second] = malloc(sizeof(Node));
        if (Table[first][second] == NULL) return false;
        node_counter++;
        NODE = Table[first][second];

        // Derefence NODE->word by storing word pointed at by buffer
        NODE->word = malloc(sizeof(char) * (LENGTH + 1));
        if (NODE->word == NULL) return false;
        strcpy(NODE->word, buffer);

        // If already existing nodes, else
        if (exists[first][second])
        {
            NODE->next = n;
            n = NODE;
        }
        else
        {
            // Reset the n pointer so it doesn't point to the previous bucket's nodes
            n = NULL;
            NODE->next = n;
            n = NODE;
            exists[first][second] = true;
        }
    }
    free(buffer);
    fclose(diction);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return node_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N + 3; j++)
        {
            if (Table[i][j] != NULL)
            {
                free_list(Table[i][j]);
            }
        }
    }
    // Same # of nodes should've been loaded and unloaded
    if (unloads == node_counter)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void free_list(Node *ptr)
{
    // if last node, else
    if (ptr->next == NULL)
    {
        free(ptr->word);
        free(ptr);
        unloads++;
    }
    else
    {
        free_list(ptr->next);
        free(ptr->word);
        free(ptr);
        unloads++;
    }
}