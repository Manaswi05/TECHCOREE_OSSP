#include <stdio.h>

int n, m;
int alloc[10][10], max[10][10], need[10][10];
int avail[10], finish[10], seq[10];
int safeCount = 0;

void findSafeSequences(int count)
{
    int i, j, found;

    if (count == n)
    {
        safeCount++;
        printf("Safe Sequence %d: ", safeCount);

        for (i = 0; i < n; i++)
            printf("P%d ", seq[i]);

        printf("\n");
        return;
    }

    found = 0;

    for (i = 0; i < n; i++)
    {
        if (!finish[i])
        {
            int possible = 1;

            for (j = 0; j < m; j++)
            {
                if (need[i][j] > avail[j])
                {
                    possible = 0;
                    break;
                }
            }

            if (possible)
            {
                found = 1;

                for (j = 0; j < m; j++)
                    avail[j] += alloc[i][j];

                finish[i] = 1;
                seq[count] = i;

                findSafeSequences(count + 1);

                finish[i] = 0;

                for (j = 0; j < m; j++)
                    avail[j] -= alloc[i][j];
            }
        }
    }
}

int main()
{
    int i, j;

    printf("Enter number of processes and resources: ");
    scanf("%d %d", &n, &m);

    printf("Enter Allocation Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter Maximum Claim Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    printf("Enter Available Resources:\n");
    for (j = 0; j < m; j++)
        scanf("%d", &avail[j]);

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    printf("\nNeed Matrix:\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
            printf("%d ", need[i][j]);

        printf("\n");
    }

    for (i = 0; i < n; i++)
        finish[i] = 0;

    findSafeSequences(0);

    if (safeCount > 0)
        printf("\nSystem is in SAFE state.\n");
    else
        printf("\nSystem is in UNSAFE state. Deadlock may occur.\n");

    printf("Total Safe Sequences: %d\n", safeCount);

    return 0;
}
