#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#define TOTAL_STUDENTS 15

int main()
{
    FILE *fp;
    char *students[TOTAL_STUDENTS] = {
        "Aarav",
        "Bhavik",
        "Chaitanya",
        "Deepak",
        "Harini",
        "Ishita",
        "Karthik",
        "Manaswi",
        "Neha",
        "Pranav",
        "Rahul",
        "Sanjana",
        "Tejas",
        "Varun",
        "Zoya"
    };

    char line[100];
    int line_number;

    /* 1. Create linux_task3 directory */
    if (mkdir("linux_task3", 0755) == 0)
    {
        printf("Directory linux_task3 created.\n");
    }
    else
    {
        printf("Directory linux_task3 already exists.\n");
    }

    /* 2. Create students.txt */
    fp = fopen("linux_task3/students.txt", "w");

    if (fp == NULL)
    {
        perror("students.txt");
        return 1;
    }

    /* 3. Add 15 student names */
    for (int i = 0; i < TOTAL_STUDENTS; i++)
    {
        fprintf(fp, "%s\n", students[i]);
    }

    fclose(fp);

    printf("students.txt created with 15 student names.\n");

    /* 4. Display entire file */
    fp = fopen("linux_task3/students.txt", "r");

    if (fp == NULL)
    {
        perror("students.txt");
        return 1;
    }

    printf("\n===== Entire File =====\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }

    fclose(fp);

    /* 5. Display first 5 lines */
    fp = fopen("linux_task3/students.txt", "r");

    printf("\n===== First 5 Lines =====\n");

    line_number = 0;

    while (line_number < 5 &&
           fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
        line_number++;
    }

    fclose(fp);

    /* 6. Display first 10 lines */
    fp = fopen("linux_task3/students.txt", "r");

    printf("\n===== First 10 Lines =====\n");

    line_number = 0;

    while (line_number < 10 &&
           fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
        line_number++;
    }

    fclose(fp);

    /* 7. Display last 5 lines */
    fp = fopen("linux_task3/students.txt", "r");

    printf("\n===== Last 5 Lines =====\n");

    char *last_five[5];
    int count = 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (count < 5)
        {
            last_five[count] = malloc(strlen(line) + 1);
            strcpy(last_five[count], line);
            count++;
        }
        else
        {
            free(last_five[0]);

            for (int i = 0; i < 4; i++)
            {
                last_five[i] = last_five[i + 1];
            }

            last_five[4] = malloc(strlen(line) + 1);
            strcpy(last_five[4], line);
        }
    }

    fclose(fp);

    for (int i = 0; i < count; i++)
    {
        printf("%s", last_five[i]);
        free(last_five[i]);
    }

    /* 8. Display last 10 lines */
    fp = fopen("linux_task3/students.txt", "r");

    printf("\n===== Last 10 Lines =====\n");

    char *last_ten[10];
    count = 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (count < 10)
        {
            last_ten[count] = malloc(strlen(line) + 1);
            strcpy(last_ten[count], line);
            count++;
        }
        else
        {
            free(last_ten[0]);

            for (int i = 0; i < 9; i++)
            {
                last_ten[i] = last_ten[i + 1];
            }

            last_ten[9] = malloc(strlen(line) + 1);
            strcpy(last_ten[9], line);
        }
    }

    fclose(fp);

    for (int i = 0; i < count; i++)
    {
        printf("%s", last_ten[i]);
        free(last_ten[i]);
    }

    printf("\nTask 3 completed successfully.\n");

    return 0;
}
