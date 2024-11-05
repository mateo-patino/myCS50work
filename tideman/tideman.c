#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
}
pair;

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
bool is_cycle(int start, int current);

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
        } // Interpret loops as: all values of i with all values of j
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank.
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
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
    bool valid = false;
    for (int i = 0; i < candidate_count; i++)
    {
        // Find the candidate index and store it in rank j (from main)
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            valid = true;
            break;
        }
    }
    return valid;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int counter = 0;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = (1 + counter); j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
        counter++;
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // You add the "i + 1" because you cannot logically compare the same candidate
        // (preferences[0][0] or preferences[1][1])
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                // Pair_count is automatically initialized to 0 when declared globally
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            // An else if condition must be used instead of else, otherwise tied pairs could be added to the array.
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // strength = preferences[i][j]. Pair ten has winner of index 10. Pair 3 has winner of index 3.
    for (int i = 0; i < (pair_count - 1); i++)
    {
        int indx;
        int largest = 0;
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] > largest)
            {
                largest = preferences[pairs[j].winner][pairs[j].loser];
                indx = j;
            }
        }
        if (preferences[pairs[indx].winner][pairs[indx].loser] > preferences[pairs[i].winner][pairs[i].loser])
        {
            pair temp = pairs[indx];
            pairs[indx] = pairs[i];
            pairs[i] = temp;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!is_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

bool is_cycle(int winner, int loser)
{
    //This algorithm somehow works. Derive your solution and understanding from it.
    if (winner == loser)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (is_cycle(winner, i))
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    int winner;
    bool losers[candidate_count];
    memset(losers, false, sizeof(losers));

    // Populate losers array
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 1; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                losers[j] = true;
            }
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (!losers[i])
        {
            winner = i;
        }
    }
    printf("%s\n", candidates[winner]);
}
