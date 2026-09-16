#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

/* Function to copy a file */
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

/* Function to list directory contents recursively */
void list_directory(const char *path, int level)
{
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    char full_path[1000];

    dir = opendir(path);

    if (dir == NULL)
    {
        perror(path);
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
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
    char line[200];

    /* 1. Create linux_project */
    if (mkdir("linux_project", 0755) == 0)
    {
        printf("linux_project directory created.\n");
    }
    else
    {
        printf("linux_project directory already exists.\n");
    }

    /* 2. Create source, backup and docs */
    mkdir("linux_project/source", 0755);
    mkdir("linux_project/backup", 0755);
    mkdir("linux_project/docs", 0755);

    printf("source, backup and docs directories created.\n");

    /* 3. Create main.c */
    fp = fopen("linux_project/source/main.c", "w");

    if (fp == NULL)
    {
        perror("main.c");
        return 1;
    }

    fprintf(fp, "#include <stdio.h>\n");
    fprintf(fp, "int main()\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    printf(\"Hello Linux Project\\n\");\n");
    fprintf(fp, "    return 0;\n");
    fprintf(fp, "}\n");

    fclose(fp);

    /* Create helper.c */
    fp = fopen("linux_project/source/helper.c", "w");

    if (fp == NULL)
    {
        perror("helper.c");
        return 1;
    }

    fprintf(fp, "#include <stdio.h>\n");
    fprintf(fp, "void helper()\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    printf(\"Helper function\\n\");\n");
    fprintf(fp, "}\n");

    fclose(fp);

    /* Create README.txt */
    fp = fopen("linux_project/source/README.txt", "w");

    if (fp == NULL)
    {
        perror("README.txt");
        return 1;
    }

    /* 4. Add 20 lines */
    fprintf(fp, "Line 1: Linux Project\n");
    fprintf(fp, "Line 2: Operating Systems\n");
    fprintf(fp, "Line 3: File Management\n");
    fprintf(fp, "Line 4: Directory Management\n");
    fprintf(fp, "Line 5: C Programming\n");
    fprintf(fp, "Line 6: System Programming\n");
    fprintf(fp, "Line 7: Linux Commands\n");
    fprintf(fp, "Line 8: File Operations\n");
    fprintf(fp, "Line 9: Copy Operation\n");
    fprintf(fp, "Line 10: Rename Operation\n");
    fprintf(fp, "Line 11: Delete Operation\n");
    fprintf(fp, "Line 12: Backup Operation\n");
    fprintf(fp, "Line 13: Documentation\n");
    fprintf(fp, "Line 14: File Permissions\n");
    fprintf(fp, "Line 15: Read Permission\n");
    fprintf(fp, "Line 16: Write Permission\n");
    fprintf(fp, "Line 17: Execute Permission\n");
    fprintf(fp, "Line 18: Source Files\n");
    fprintf(fp, "Line 19: Linux Practical\n");
    fprintf(fp, "Line 20: Task Completed\n");

    fclose(fp);

    printf("main.c, helper.c and README.txt created.\n");
    printf("20 lines added to README.txt.\n");

    /* 5. Display first 5 lines */
    printf("\n===== First 5 Lines of README.txt =====\n");

    fp = fopen("linux_project/source/README.txt", "r");

    if (fp == NULL)
    {
        perror("README.txt");
        return 1;
    }

    int count = 0;

    while (count < 5 &&
           fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
        count++;
    }

    fclose(fp);

    /* 6. Display last 5 lines */
    printf("\n===== Last 5 Lines of README.txt =====\n");

    fp = fopen("linux_project/source/README.txt", "r");

    if (fp == NULL)
    {
        perror("README.txt");
        return 1;
    }

    char *last_five[5];

    count = 0;

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

    /* 7. Copy main.c to backup */
    printf("\n===== Copying main.c =====\n");

    if (copy_file("linux_project/source/main.c",
                  "linux_project/backup/main.c"))
    {
        printf("main.c copied to backup.\n");
    }

    /* 8. Copy README.txt to docs */
    if (copy_file("linux_project/source/README.txt",
                  "linux_project/docs/README.txt"))
    {
        printf("README.txt copied to docs.\n");
    }

    /* 9. Rename helper.c to functions.c */
    if (rename("linux_project/source/helper.c",
               "linux_project/source/functions.c") == 0)
    {
        printf("helper.c renamed to functions.c.\n");
    }
    else
    {
        perror("helper.c");
    }

    /* 10. Change README.txt permissions to 644 */
    if (chmod("linux_project/source/README.txt", 0644) == 0)
    {
        printf("README.txt permissions changed to 644.\n");
    }
    else
    {
        perror("chmod");
    }

    /* 11. Delete copied main.c from backup */
    if (remove("linux_project/backup/main.c") == 0)
    {
        printf("Copied main.c deleted from backup.\n");
    }
    else
    {
        perror("main.c");
    }

    /* 12. Remove empty backup directory */
    if (rmdir("linux_project/backup") == 0)
    {
        printf("Empty backup directory removed.\n");
    }
    else
    {
        perror("backup");
    }

    /* 13. List final directory structure */
    printf("\n===== Final Directory Structure =====\n");
    printf("linux_project/\n");

    list_directory("linux_project", 1);

    printf("\nTask 9 completed successfully.\n");

    return 0;
}
