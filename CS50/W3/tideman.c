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
int sorted_pair[MAX * (MAX - 1) / 2];

int candidate_count;
int pair_count;

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

    pair_count = candidate_count * (candidate_count - 1) / 2;
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
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }
    add_pairs();
    sort_pairs();
    for (int i = 0; i < pair_count; i++)
    {
        printf("sorted pair %i winner is %s\n", i, candidates[pairs[sorted_pair[i]].winner]);
        printf("sorted pair %i loser is %s\n", i, candidates[pairs[sorted_pair[i]].loser]);
    }
    // 100%sure pairs are sorted right!
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[i] = rank;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {

        for (int j = i + 1; j < candidate_count; j++)
        {
            // compare ranks of candidate[i] with each everyone else
            if (ranks[i] < ranks[j])
            {
                preferences[i][j]++;
            }
            if (ranks[j] < ranks[i])
            {
                preferences[j][i]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int pair_no = j * (j - 1) / 2 + i;
            if (preferences[i][j] >= preferences[j][i])
            {
                pairs[pair_no].winner = i;
                pairs[pair_no].loser = j;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_no].winner = j;
                pairs[pair_no].loser = i;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // sort pairs into decreasing order, sorted pair should indicate a number of pair_no

    for (int j = 0; j < pair_count; j++)
    {
        sorted_pair[j] = 0;
        for (int i = 0; i < j; i++)
        {
            for (int k = 0; k < j; k++)
            {
                if (sorted_pair[j] == sorted_pair[k])
                {
                    sorted_pair[j]++;
                }
            }
        }
        // now sorted pair is the smallest pair_no that haven't been sorted

        for (int i = 0; i < pair_count; i++)
        {
            if (j > 0 &&
                preferences[pairs[sorted_pair[j]].winner][pairs[sorted_pair[j]].loser] <
                    preferences[pairs[i + 1].winner][pairs[i + 1].loser] &&
                preferences[pairs[i + 1].winner][pairs[i + 1].loser] <
                    preferences[pairs[sorted_pair[j - 1]].winner][pairs[sorted_pair[j - 1]].loser])
            {
                sorted_pair[j] = i + 1;
            }
            else if (j == 0 &&
                     preferences[pairs[sorted_pair[j]].winner][pairs[sorted_pair[j]].loser] <
                         preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                sorted_pair[j] = i + 1;
            }
        }
    }
    // now all pairs are sorted in decreasing strength of victory
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    string winner = NULL;
    string loser[candidate_count];
    int n;
    for (int i = 0; i < candidate_count; i++)
    {
        loser[i] = NULL;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        n = 1;
        for (int j = 0; j < candidate_count; j++)
        {
            if (loser[j] != NULL)
            {
                n = strcmp(loser[j], candidates[pairs[sorted_pair[i]].winner]);
            }
            if (n == 0)
            {
                j = candidate_count;
            }
        }
        if (n != 0)
        {
            winner = candidates[pairs[sorted_pair[i]].winner];
        }
        loser[i] = candidates[pairs[sorted_pair[i]].loser];
    }
    printf("%s\n", winner);
    return;
}

// Print the winner of the election
void print_winner(void)
{
    return;
}
