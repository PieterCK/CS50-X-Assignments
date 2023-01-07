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

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                j--;
            }
        }
        record_preferences(ranks);
    }
    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    int i = 0;
    int U;
    int V = 0;
    int Je = 0;
    int Ej = rank + 1;
    int Check_Validity2 = 0;
    // Checks & updates the chosen candidates to the ranking array
    do
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            {
                ranks[rank] = i;
                Je = i;
                i++;
                Check_Validity2++;
            }
        }
        else
        {
            i++;
        }
    } while (candidates[i] != (void *)0);
    // Ensures voter votes a valid candidate.
    if (Check_Validity2 == 0)
    {
        return false;
    }
    // Ensures voter only vote each candidate once.
    U = 0;
    for (U = 0; U < Ej; U++)
    {
        if (ranks[U] == Je)
        {
            V++;
        }
    }
    if (V > 1)
    {
        return false;
    }
    else if (V == 1)
    {
        return true;
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int A;
    int B;
    int F = candidate_count - 1;
    for (A = 0; A < F; A++)
    {
        B = A + 1;
        int C = ranks[A];
        for (B = A + 1; B < candidate_count; B++)
        {
            int D = ranks[B];
            preferences[C][D]++;
        }
    }
    // Displays preferences tabel
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int C = 0;
    int CC = candidate_count;
    for (int A = 0; A < CC - 1; A++)
    {
        for (int B = A + 1; B < CC; B++)
        {
            int Can1 = preferences[A][B];
            int Can2 = preferences[B][A];
            if (Can1 > Can2)
            {
                pairs[C].winner = A;
                pairs[C].loser = B;
                pair_count++;
                C++;
            }
            else if (Can1 < Can2)
            {
                pairs[C].winner = B;
                pairs[C].loser = A;
                pair_count++;
                C++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // bubble sort
    int H;
    do
    {
        int A = 0;
        H = 0;
        do
        {
            int B = A + 1;
            int D = preferences[pairs[A].winner][pairs[A].loser];
            int E = preferences[pairs[B].winner][pairs[B].loser];
            if (D < E)
            {
                int F = pairs[A].winner;
                int G = pairs[A].loser;
                pairs[A].winner = pairs[B].winner;
                pairs[A].loser = pairs[B].loser;
                pairs[B].winner = F;
                pairs[B].loser = G;
                H++;
                A++;
            }
            else
            {
                A++;
            }
        } while (A <= pair_count);
    } while (H > 0);
    //    int Z;
    //    for(Z=0; Z < pair_count; Z++)
    //    {
    //        printf("# %i W= %i L=%i Str=%i\n", Z, pairs[Z].winner, pairs[Z].loser, preferences[pairs[Z].winner][pairs[Z].loser]);
    //   }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int size = 0;
    int n = pair_count;
    // Determine the size of checklist array, the maximum number of arrows is the permutation(with + not x) of pair_count.
    do
    {
        size = size + n;
        n--;
    } while (n != 0);
    int i;
    // If A wants to connect to B and there are arrows pointing towards A, candidate thats pointing towards it will be stored in "checklist" array
    int checklist[size];
    // "add" will be used to help store data in "checklist" array
    int add = 0;
    // "track" will be used to check the contents of "checklist" array
    int track = 0;
    // "CYCLE" will be used to indicate whether there's a cycle or not
    bool CYCLE;

    // Populate "checklist" array as to avoid garbage value(?), all of checklist will initially be filled with '97' / 'a'.
    for (i = 0; i < pair_count; i++)
    {
        int H;
        for (H = 0; H < size; H++)
        {
            checklist[H] = 'a';
        }
        add = 0;
        track = 0;
        int o;
        // Record nodes that are pointing towards the source.
        for (o = 0; o < candidate_count; o++)
        {
            if (locked[o][pairs[i].winner] == 0)
            {
                // Do nothing
            }
            else
            { // Add candidate thats pointing towards A to "checklist"
                checklist[add] = o;
                add++;
            }
        }
        // This do-while-loop will be used to check the contents of "checklist". ends when "break;" or we're at the end of "checklist"
        // if we found B(pairs[i].loser) in "checklist", then set CYCLE = 1 and break the loop
        // else if we find 'a', meaning its the end of checklist, we may assume theres no cycle, CYCLE stays 0.
        // else if we find a candidate thats not B(pairs[i].loser) for example D, go check whether theres an arrow(s) pointing towards it
        //(continue) ... and add them to "checklist", if D is also source, track++ and continue the loop.
        //  note that because of the last else if, "checklist" can become longer and thus do-while-loop will also be extended until we find B or not.
        do
        {
            if (checklist[track] == pairs[i].loser)
            {
                CYCLE = 1;
                track++;
                break;
            }
            else if (checklist[track] == 'a')
            {
            }
            else
            {
                for (int p = 0; p < candidate_count; p++)
                {
                    if (locked[p][checklist[track]] == 1)
                    {
                        checklist[add] = p;
                        add++;
                    }
                }
                track++;
            }
        } while (checklist[track] != 'a');
        // Locks them pairs if no cycle and vice versa.
        if (CYCLE == 0)
        {
            locked[pairs[i].winner][pairs[i].loser] = 1;
        }
        else
        {
            CYCLE = 0;
        }
    }
    // DONT MIND THIS
    int Z;
    int z1;
    for (Z = 0; Z < candidate_count; Z++)
    {
        for (z1 = 0; z1 < candidate_count; z1++)
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
    // Condition for a winner  = its a source
    // print winner when "winner" is 0,
    int i;
    int winner = 0;
    for (i = 0; i < candidate_count; i++)
    {
        int o;
        for (o = 0; o < candidate_count; o++)
        {
            if (locked[o][i] == 1)
            {
                winner++;
            }
        }
        if (winner == 0)
        {
            printf("%s\n", candidates[i]);
            return;
        }
        else
        {
            winner = 0;
        }
    }
    return;
}