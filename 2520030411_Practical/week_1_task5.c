#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/* Function to display directory structure recursively */
void list_directory(const char *path, int level)
{
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    char full_path[1000];

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

        snprintf(full_path, sizeof(full_path),
                 "%s/%s", path, entry->d_name);

        for (int i = 0; i < level; i++)
        {
            printf("    ");
        }

        printf("|-- %s\n", entry->d_name);

        if (stat(full_path, &info) == 0 &&
            S_ISDIR(info.st_mode))
        {
            list_directory(full_path, level + 1);
        }
    }

    closedir(dir);
}

int main()
{
    FILE *fp;

    /* 1. Create project directory */
    if (mkdir("project", 0755) == 0)
    {
        printf("project directory created.\n");
    }
    else
    {
        printf("project directory already exists.\n");
    }

    /* 2. Create all subdirectories */
    mkdir("project/source", 0755);
    mkdir("project/source/c", 0755);
    mkdir("project/source/python", 0755);
    mkdir("project/docs", 0755);
    mkdir("project/backup", 0755);

    printf("All subdirectories created.\n");

    /* 3. Create three files inside source/c */
    fp = fopen("project/source/c/program1.c", "w");
    if (fp != NULL)
    {
        fprintf(fp, "C Program 1\n");
        fclose(fp);
    }

    fp = fopen("project/source/c/program2.c", "w");
    if (fp != NULL)
    {
        fprintf(fp, "C Program 2\n");
        fclose(fp);
    }

    fp = fopen("project/source/c/program3.c", "w");
    if (fp != NULL)
    {
        fprintf(fp, "C Program 3\n");
        fclose(fp);
    }

    /* 4. Create two files inside source/python */
    fp = fopen("project/source/python/program1.py", "w");
    if (fp != NULL)
    {
        fprintf(fp, "print(\"Python Program 1\")\n");
        fclose(fp);
    }

    fp = fopen("project/source/python/program2.py", "w");
    if (fp != NULL)
    {
        fprintf(fp, "print(\"Python Program 2\")\n");
        fclose(fp);
    }

    printf("Files created successfully.\n");

    /* 5. List complete directory structure */
    printf("\n===== Complete Directory Structure =====\n");
    printf("project/\n");
    list_directory("project", 1);

    /* 6. Remove empty backup directory */
    printf("\n===== Removing Empty Backup Directory =====\n");

    if (rmdir("project/backup") == 0)
    {
        printf("Empty backup directory removed successfully.\n");
    }
    else
    {
        perror("rmdir");
    }

    /* 7. Create backup directory again */
    if (mkdir("project/backup", 0755) == 0)
    {
        printf("backup directory created again.\n");
    }

    /* 8. Create a file inside backup */
    fp = fopen("project/backup/backup.txt", "w");

    if (fp == NULL)
    {
        perror("backup.txt");
        return 1;
    }

    fprintf(fp, "This file is inside the backup directory.\n");
    fclose(fp);

    printf("backup.txt created inside backup.\n");

    /* 9. Try to remove non-empty backup directory */
    printf("\n===== Trying to Remove Non-Empty Backup =====\n");

    if (rmdir("project/backup") == 0)
    {
        printf("Backup directory removed.\n");
    }
    else
    {
        printf("Cannot remove backup because it is not empty.\n");
        perror("rmdir");
    }

    /* 10. Delete the file inside backup */
    if (remove("project/backup/backup.txt") == 0)
    {
        printf("\nbackup.txt deleted successfully.\n");
    }
    else
    {
        perror("backup.txt");
    }

    /* 11. Remove backup using rmdir */
    if (rmdir("project/backup") == 0)
    {
        printf("backup directory removed successfully.\n");
    }
    else
    {
        perror("rmdir");
    }

    /* Final structure */
    printf("\n===== Final Directory Structure =====\n");
    printf("project/\n");
    list_directory("project", 1);

    printf("\nTask 5 completed successfully.\n");

    return 0;
}
