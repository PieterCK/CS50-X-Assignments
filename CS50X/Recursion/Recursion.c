#include <cs50.h>
#include <stdio.h>
#include <string.h>
// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(void)
{
    candidate_count=5;
    pair_count=4;
    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }
    // Get pairs array
    for (int i = 0; i < pair_count; i++)
    {
        int A = get_int("pair %i winner= ",i);
        int B = get_int("pair %i loser= ",i);
        printf("\n");
        pairs[i].winner=A;
        pairs[i].loser=B;
    }

    int Z;
    for(Z=0; Z < pair_count; Z++)
    {
        printf("# %i W= %i L=%i Str=%i\n", Z, pairs[Z].winner, pairs[Z].loser, preferences[pairs[Z].winner][pairs[Z].loser]);
    }

    lock_pairs();
    print_winner();
    return 0;
}


// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int size = 0;
    int n = pair_count;
    // the size of checklist array = the permutation of pair_count
    do
    {
        size = size + n;
        n--;
    } while (n != 0);
    int i;
    int checklist[size];
    int cn = 0;
    int cl = 0;
    int SHIZE = MAX+1;
    bool CYCLE;
    for (i = 0; i < pair_count; i++)
    {
        int AISHH;
        for(AISHH = 0; AISHH < size;AISHH++)
        {
            checklist[AISHH]='a';
        }
        cn=0;
        cl=0;
        int o;
        // Record nodes that are pointing towards the source.
        for (o = 0; o < candidate_count; o++)
        {
            if (locked[o][pairs[i].winner] == 0)
            {
            }
            else
            {
                checklist[cn] = o;
                cn++;
            }
        }

        do
        {
            if (checklist[cl] == pairs[i].loser)
            {
                CYCLE = 1;
                cl++;
                break;
            }
            else if (checklist[cl] == 'a')
            {}
            else
            {
                for (int p = 0; p < candidate_count; p++)
                {
                    if(locked[p][checklist[cl]]==1)
                    {
                        checklist[cn] = p;
                        cn++;
                    }
                }
                cl++;
            }
        } while (checklist[cl] != 'a');

        if (CYCLE == 0)
        {
            locked[pairs[i].winner][pairs[i].loser]=1;
        }
        else
        {
            CYCLE = 0;
        }

    }
    int Z;
    int z1;
    for(Z=0; Z < candidate_count; Z++)
    {
        for(z1=0; z1 < candidate_count; z1++)
        {
            printf("%i", locked[Z][z1]);
        }
        printf("\n");
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int i;
    int winner;
    for(i=0;i<candidate_count;i++)
    {
        int o;
        for(o=0;o<candidate_count;o++)
        {
            if(locked[o][i]==1)
            {
                winner++;
            }
        }
        if(winner==0)
        {
            printf("%d\n", pairs[i].winner);
            return;
        }
        else
        {
            winner=0;
        }
    }
    return;
}