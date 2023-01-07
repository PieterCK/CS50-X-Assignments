#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    // Registers the candidates' name from command line argument
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
            i--;
        }
    }
    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    int I = 0;
    do
    {
        if (strcmp(candidates[I].name, name) == 0)
        {
            candidates[I].votes++;
            return true;
        }
        else
        {
            I++;
        }

    } while (candidates[I].name != (void *)0);
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int C = 0;
    int I = 0;
    int Tie = 0;
    do
    {

        if (candidates[I].votes > candidates[C].votes)
        {
            C++;
        }
        else if (candidates[I].votes == candidates[C].votes)
        {
            C++;
            Tie++;
        }
        else if (candidates[I].votes < candidates[C].votes)
        {
            I++;
            C = 0;
            Tie = 0;
        }
    } while (candidates[C].votes != '\0');
    printf("%s\n", candidates[I].name);
    int Nu = '\0';
    if (Tie > 1)
    {
        candidates[I] = candidates[C - 1];
        candidates[C - 1].votes = Nu;
        return print_winner();
    }
    return;
}