#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

int main()
{
    FILE *fp;
    DIR *dir;
    struct dirent *entry;
    struct stat file_info;

    /* 1. Create directory */
    if (mkdir("linux_task1", 0755) == 0)
        printf("Directory linux_task1 created.\n");
    else
        printf("Directory linux_task1 already exists.\n");

    /* 2. Enter into directory */
    if (chdir("linux_task1") != 0)
    {
        perror("chdir");
        return 1;
    }

    printf("Entered linux_task1 directory.\n\n");

    /* 3. Create five empty files */
    fp = fopen("file1.txt", "w");
    if (fp != NULL)
        fclose(fp);

    fp = fopen("file2.txt", "w");
    if (fp != NULL)
        fclose(fp);

    fp = fopen("file3.txt", "w");
    if (fp != NULL)
        fclose(fp);

    fp = fopen("file4.txt", "w");
    if (fp != NULL)
        fclose(fp);

    fp = fopen("file5.txt", "w");
    if (fp != NULL)
        fclose(fp);

    printf("Five empty files created.\n\n");

    /* 4. List all files */
    printf("List of files:\n");

    dir = opendir(".");

    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
            printf("%s\n", entry->d_name);
    }

    closedir(dir);

    /* 5. Display detailed information */
    printf("\nDetailed information:\n");

    dir = opendir(".");

    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            if (stat(entry->d_name, &file_info) == 0)
            {
                printf("%-12s Size: %ld bytes  Permissions: %o\n",
                       entry->d_name,
                       (long)file_info.st_size,
                       file_info.st_mode & 0777);
            }
        }
    }

    closedir(dir);

    /* 6. Add content to file1.txt */
    fp = fopen("file1.txt", "w");

    if (fp == NULL)
    {
        perror("file1.txt");
        return 1;
    }

    fprintf(fp, "This is file1.txt.\n");
    fprintf(fp, "Created using C programming in Linux.\n");

    fclose(fp);

    printf("\nContent added to file1.txt.\n");

    /* 7. Display contents of file1.txt */
    fp = fopen("file1.txt", "r");

    if (fp == NULL)
    {
        perror("file1.txt");
        return 1;
    }

    printf("\nContents of file1.txt:\n");

    char line[200];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }

    fclose(fp);

    /* 8. Delete file5.txt */
    if (remove("file5.txt") == 0)
        printf("\nfile5.txt deleted successfully.\n");
    else
        perror("file5.txt");

    /* 9. List files again */
    printf("\nFiles after deleting file5.txt:\n");

    dir = opendir(".");

    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
            printf("%s\n", entry->d_name);
    }

    closedir(dir);

    return 0;
}
