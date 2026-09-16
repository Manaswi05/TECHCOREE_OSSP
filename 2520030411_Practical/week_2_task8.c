#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

/* Function to copy one file to another */
int copy_file(const char *source, const char *destination)
{
    FILE *src;
    FILE *dest;
    char ch;

    src = fopen(source, "r");

    if (src == NULL)
    {
        perror(source);
        return 0;
    }

    dest = fopen(destination, "w");

    if (dest == NULL)
    {
        perror(destination);
        fclose(src);
        return 0;
    }

    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, dest);
    }

    fclose(src);
    fclose(dest);

    return 1;
}

int main()
{
    FILE *fp;
    char line[200];

    /* 1. Create linux_task6 directory */
    if (mkdir("linux_task6", 0755) == 0)
    {
        printf("Directory linux_task6 created.\n");
    }
    else
    {
        printf("Directory linux_task6 already exists.\n");
    }

    /* 2. Create important.txt */
    fp = fopen("linux_task6/important.txt", "w");

    if (fp == NULL)
    {
        perror("important.txt");
        return 1;
    }

    /* 3. Add 10 lines of content */
    fprintf(fp, "Line 1: Operating Systems\n");
    fprintf(fp, "Line 2: File Management\n");
    fprintf(fp, "Line 3: Linux Programming\n");
    fprintf(fp, "Line 4: Backup and Restore\n");
    fprintf(fp, "Line 5: System Programming\n");
    fprintf(fp, "Line 6: C Programming\n");
    fprintf(fp, "Line 7: Process Management\n");
    fprintf(fp, "Line 8: File Permissions\n");
    fprintf(fp, "Line 9: Directory Management\n");
    fprintf(fp, "Line 10: Practical Task 8\n");

    fclose(fp);

    printf("important.txt created with 10 lines.\n");

    /* 4. Create backup directory */
    if (mkdir("linux_task6/backup", 0755) == 0)
    {
        printf("backup directory created.\n");
    }
    else
    {
        printf("backup directory already exists.\n");
    }

    /* 5. Copy important.txt into backup */
    if (copy_file("linux_task6/important.txt",
                  "linux_task6/backup/important.txt"))
    {
        printf("important.txt copied to backup.\n");
    }
    else
    {
        return 1;
    }

    /* 6. Rename backup copy */
    if (rename("linux_task6/backup/important.txt",
               "linux_task6/backup/important_backup.txt") == 0)
    {
        printf("Backup renamed to important_backup.txt.\n");
    }
    else
    {
        perror("rename");
        return 1;
    }

    /* 7. Delete original important.txt */
    if (remove("linux_task6/important.txt") == 0)
    {
        printf("Original important.txt deleted.\n");
    }
    else
    {
        perror("important.txt");
        return 1;
    }

    /* 8. Copy backup back to main directory */
    if (copy_file("linux_task6/backup/important_backup.txt",
                  "linux_task6/important_restored.txt"))
    {
        printf("Backup copied back to main directory.\n");
    }
    else
    {
        return 1;
    }

    /* 9. The restored file is already named important_restored.txt */
    printf("Restored file named important_restored.txt.\n");

    /* 10. Display contents */
    printf("\n===== Contents of important_restored.txt =====\n");

    fp = fopen("linux_task6/important_restored.txt", "r");

    if (fp == NULL)
    {
        perror("important_restored.txt");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }

    fclose(fp);

    printf("\nTask 8 completed successfully.\n");

    return 0;
}
