#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

/* Function to list files and directories recursively */
void list_recursive(const char *path, int level)
{
    DIR *dir;
    struct dirent *entry;
    char full_path[1000];
    struct stat info;

    dir = opendir(path);

    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        /* Ignore . and .. */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        /* Create complete path */
        snprintf(full_path, sizeof(full_path),
                 "%s/%s", path, entry->d_name);

        /* Print indentation */
        for (int i = 0; i < level; i++)
        {
            printf("    ");
        }

        printf("|-- %s\n", entry->d_name);

        /* Check if entry is a directory */
        if (stat(full_path, &info) == 0 &&
            S_ISDIR(info.st_mode))
        {
            list_recursive(full_path, level + 1);
        }
    }

    closedir(dir);
}

int main()
{
    FILE *fp;

    /* 1. Create linux_task2 directory */
    if (mkdir("linux_task2", 0755) == 0)
    {
        printf("Directory linux_task2 created.\n");
    }
    else
    {
        printf("Directory linux_task2 already exists.\n");
    }

    /* 2. Create source.txt, data.txt and notes.txt */
    fp = fopen("linux_task2/source.txt", "w");

    if (fp == NULL)
    {
        perror("source.txt");
        return 1;
    }

    /* 3. Add content to source.txt */
    fprintf(fp, "This is the source file.\n");
    fprintf(fp, "This file is used for the copy operation.\n");
    fclose(fp);

    fp = fopen("linux_task2/data.txt", "w");

    if (fp == NULL)
    {
        perror("data.txt");
        return 1;
    }

    fclose(fp);

    fp = fopen("linux_task2/notes.txt", "w");

    if (fp == NULL)
    {
        perror("notes.txt");
        return 1;
    }

    fclose(fp);

    printf("source.txt, data.txt and notes.txt created.\n");

    /* 4. Create backup directory */
    if (mkdir("linux_task2/backup", 0755) == 0)
    {
        printf("backup directory created.\n");
    }
    else
    {
        printf("backup directory already exists.\n");
    }

    /*
     * 5. Copy source.txt into backup
     */
    fp = fopen("linux_task2/source.txt", "r");

    if (fp == NULL)
    {
        perror("source.txt");
        return 1;
    }

    FILE *dest = fopen("linux_task2/backup/source.txt", "w");

    if (dest == NULL)
    {
        perror("backup/source.txt");
        fclose(fp);
        return 1;
    }

    char ch;

    while ((ch = fgetc(fp)) != EOF)
    {
        fputc(ch, dest);
    }

    fclose(fp);
    fclose(dest);

    printf("source.txt copied to backup.\n");

    /*
     * 6. Copy data.txt to backup
     *    and rename it data_backup.txt
     */
    fp = fopen("linux_task2/data.txt", "r");

    if (fp == NULL)
    {
        perror("data.txt");
        return 1;
    }

    dest = fopen("linux_task2/backup/data_backup.txt", "w");

    if (dest == NULL)
    {
        perror("data_backup.txt");
        fclose(fp);
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF)
    {
        fputc(ch, dest);
    }

    fclose(fp);
    fclose(dest);

    printf("data.txt copied as data_backup.txt in backup.\n");

    /*
     * 7. Move notes.txt into backup
     */
    if (rename("linux_task2/notes.txt",
               "linux_task2/backup/notes.txt") == 0)
    {
        printf("notes.txt moved to backup.\n");
    }
    else
    {
        perror("notes.txt");
    }

    /*
     * 8. Rename source.txt as original.txt
     */
    if (rename("linux_task2/source.txt",
               "linux_task2/original.txt") == 0)
    {
        printf("source.txt renamed to original.txt.\n");
    }
    else
    {
        perror("source.txt");
    }

    /*
     * 9. List all files and directories recursively
     */
    printf("\nRecursive listing:\n");
    printf("linux_task2/\n");

    list_recursive("linux_task2", 1);

    return 0;
}
