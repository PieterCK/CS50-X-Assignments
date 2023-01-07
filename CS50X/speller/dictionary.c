// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include "dictionary.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;
long trk_size = 0;
int shrt = 0;
int mid = 0;
int lng = 0;

int f_hash(int tmp[10], long times);
// TODO: Choose number of buckets in hash table
const unsigned long N = 143091;
// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hashed = hash(word);

    int C = strlen(word);
    char wordd[C];
    for (int i = 0; i <= C; i++)
    {
        wordd[i] = toupper(word[i]);
    }
    //printf("wordd=%s\n",wordd);

    if (table[hashed] == NULL)
    {
        return false;
    }
    else
    {
        node *start = table[hashed];
        while (start != NULL)
        {
            if (strcmp(start->word, wordd) == 0)
            {
                return true;
            }
            else
            {
                start = start->next;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // hashing for short words (< 5 letters)
    if (strlen(word) < 5)
    {
        shrt++;
        int i = 0;
        int ii = 0;
        long times = 1;
        int tmp[strlen(word) * 2];

        while (word[i] != '\0')
        {
            int tmp2;
            if (isalpha(word[i]))
            {
                tmp2 = toupper(word[i]) - 'A' + 1;
            }
            else
            {
                tmp2 = 9;
            }

            if (tmp2 <= 9)
            {
                tmp[ii] = tmp2;
            }
            else if (tmp2 > 9)
            {
                tmp[ii] = tmp2 / 10;
                ii++;
                tmp[ii] = tmp2 % 10;
            }
            i++;
            ii++;
        }
        // Converts strings of numbers (tmp) to an actual number
        for (int n = 0; n < ii - 1; n++)
        {
            times = times * 10;
        }
        return f_hash(tmp, times);
    }

    else if (strlen(word) >= 5 && strlen(word) < 10)
    {
        mid++;
        int ii = 0;
        int tmp[6];
        int e_i = strlen(word) - 1;
        int s_i = strlen(word) / 2;
        bool alter = true;
        while (ii < 7)
        {
            if (alter == true)
            {
                int tmp2;
                if (isalpha(word[s_i]))
                {
                    tmp2 = toupper(word[s_i]) - 'A' + 1;
                }
                else
                {
                    tmp2 = 9;
                }

                if (tmp2 <= 9)
                {
                    tmp[ii] = tmp2;
                }
                else
                {
                    tmp[ii] = tmp2 / 10;
                    ii++;
                    tmp[ii] = tmp2 % 10;
                }
                s_i--;
                ii++;
                alter = false;
            }

            else if (alter == false)
            {
                int tmp2;
                if (isalpha(word[e_i]))
                {
                    tmp2 = toupper(word[e_i]) - 'A' + 1;
                }
                else
                {
                    tmp2 = 9;
                }

                if (tmp2 <= 9)
                {
                    tmp[ii] = tmp2;
                }
                else
                {
                    tmp[ii] = tmp2 / 10;
                    ii++;
                    tmp[ii] = tmp2 % 10;
                }
                e_i--;
                ii++;
                alter = true;
            }
        }
        // Converts strings of numbers (tmp) to an actual number
        long times = 1;
        for (int n = 0; n < ii - 1; n++)
        {
            times = times * 10;
        }
        return f_hash(tmp, times);
    }

    else if (strlen(word) >= 10)
    {
        lng++;
        int ii = 0;
        int tmp[4];
        int e_i = strlen(word) - 1;
        int s_i = strlen(word) / 2;
        bool alter = true;
        while (ii < 5)
        {
            if (alter == true)
            {
                int tmp2;
                if (isalpha(word[s_i]))
                {
                    tmp2 = toupper(word[s_i]) - 'A' + 1;
                }
                else
                {
                    tmp2 = '9';
                }

                if (tmp2 < 9)
                {
                    tmp[ii] = tmp2;
                }
                else
                {
                    tmp[ii] = tmp2 / 10;
                    ii++;
                    tmp[ii] = tmp2 % 10;
                }
                s_i--;
                ii++;
                alter = false;
            }

            else if (alter == false)
            {
                int tmp2;
                if (isalpha(word[e_i]))
                {
                    tmp2 = toupper(word[e_i]) - 'A' + 1;
                }
                else
                {
                    tmp2 = '9';
                }

                if (tmp2 < 9)
                {
                    tmp[ii] = tmp2;
                }
                else
                {
                    tmp[ii] = tmp2 / 10;
                    ii++;
                    tmp[ii] = tmp2 % 10;
                }
                e_i--;
                ii++;
                alter = true;
            }
        }
        // Converts strings of numbers (tmp) to an actual number
        long times = 1;
        for (int n = 0; n < ii - 1; n++)
        {
            times = times * 10;
        }
        return f_hash(tmp, times);
    }
    else
    {
        //printf("anomaly: %s\n", word);
        return 0;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    int linkedlists = 0;
    int insidearray = 0;
    int avglengthword = 0;
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        return 0;
    }
    char temp[LENGTH + 1];
    while (fscanf(input, "%s", temp) == 1)
    {
        int hashed = hash(temp);
        // printf("Word: %s, Hashed: %i\n", temp, hashed);

        int C = strlen(temp);
        for (int i = 0; i <= C; i++)
        {
            temp[i] = toupper(temp[i]);
        }

        node *neo = malloc(sizeof(node));
        strcpy(neo->word, temp);
        neo->next = NULL;

        if (table[hashed] == NULL)
        {
            table[hashed] = neo;
            insidearray++;
            trk_size++;
        }
        else
        {
            node *start = table[hashed];
            while (start != NULL)
            {
                start = start->next;
            }
            linkedlists++;
            trk_size++;
            start = neo;
            // printf("Word: %s, Hashed: %i\n", temp, hashed);
        }
    }
    fclose(input);
    //printf("# unique lots: %i\n# linked lists: %i\n# short letter words: %i\n# mid letter words: %i\n# long letter words: %i\n", insidearray, linkedlists, shrt, mid, lng);
    return 1;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return trk_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    int i = 0;
    do
    {
        if (table[i] != NULL)
        {
            node *start = table[i];
            while (start != NULL)
            {
                node *temp = start->next;
                free(start);
                start = temp;
            }
        }
        i++;
    } while (i <= N);
    return true;
}

int f_hash(int tmp[10], long times)
{
    int final = 0;
    int i = 0;
    while (times >= 10)
    {
        int tmp2 = tmp[i] * times;
        final = final + tmp2;
        times = times / 10;
        i++;
    }
    if (final == 0 && i == 0)
    {
        final = tmp[i];
    }
    if (final <= 143091)
    {
        return final;
    }
    else
    {
        while (final > 14300)
        {
            final = final / 10;
        }
        return final;
    }
}