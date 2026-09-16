#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

/* Function to display file permissions */
void display_permissions(const char *filename)
{
    struct stat file_info;

    if (stat(filename, &file_info) == -1)
    {
        perror(filename);
        return;
    }

    printf("%-12s Permissions: %04o\n",
           filename,
           file_info.st_mode & 0777);
}

int main()
{
    FILE *fp;

    /* 1. Create linux_task4 directory */
    if (mkdir("linux_task4", 0755) == 0)
    {
        printf("Directory linux_task4 created.\n");
    }
    else
    {
        printf("Directory linux_task4 already exists.\n");
    }

    /* 2. Create public.txt */
    fp = fopen("linux_task4/public.txt", "w");

    if (fp == NULL)
    {
        perror("public.txt");
        return 1;
    }

    /* 3. Add content */
    fprintf(fp, "This is a public file.\n");
    fclose(fp);

    /* Create private.txt */
    fp = fopen("linux_task4/private.txt", "w");

    if (fp == NULL)
    {
        perror("private.txt");
        return 1;
    }

    fprintf(fp, "This is a private file.\n");
    fclose(fp);

    /* Create script.sh */
    fp = fopen("linux_task4/script.sh", "w");

    if (fp == NULL)
    {
        perror("script.sh");
        return 1;
    }

    fprintf(fp, "#!/bin/bash\n");
    fprintf(fp, "echo \"Hello from script.sh\"\n");
    fclose(fp);

    printf("Three files created successfully.\n");

    /* 4. Display current permissions */
    printf("\n===== Current Permissions =====\n");

    display_permissions("linux_task4/public.txt");
    display_permissions("linux_task4/private.txt");
    display_permissions("linux_task4/script.sh");

    /* 5. Set private.txt permission to 600 */
    if (chmod("linux_task4/private.txt", 0600) == 0)
    {
        printf("\nprivate.txt permission set to 600.\n");
    }
    else
    {
        perror("chmod private.txt");
    }

    /* 6. Set public.txt permission to 644 */
    if (chmod("linux_task4/public.txt", 0644) == 0)
    {
        printf("public.txt permission set to 644.\n");
    }
    else
    {
        perror("chmod public.txt");
    }

    /* 7. Give execute permission to script.sh */
    if (chmod("linux_task4/script.sh", 0755) == 0)
    {
        printf("Execute permission given to script.sh.\n");
    }
    else
    {
        perror("chmod script.sh");
    }

    /* 8. Check permissions again */
    printf("\n===== Permissions After chmod =====\n");

    display_permissions("linux_task4/public.txt");
    display_permissions("linux_task4/private.txt");
    display_permissions("linux_task4/script.sh");

    /* 9. Remove execute permission from script.sh */
    if (chmod("linux_task4/script.sh", 0644) == 0)
    {
        printf("\nExecute permission removed from script.sh.\n");
    }
    else
    {
        perror("chmod script.sh");
    }

    printf("\n===== Final Permissions =====\n");

    display_permissions("linux_task4/public.txt");
    display_permissions("linux_task4/private.txt");
    display_permissions("linux_task4/script.sh");

    printf("\nTask 6 completed successfully.\n");

    return 0;
}
