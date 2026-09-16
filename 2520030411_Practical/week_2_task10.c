#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

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

/* Function to display first 5 lines */
void display_first_five(const char *filename)
{
    FILE *fp;
    char line[200];
    int count = 0;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror(filename);
        return;
    }

    printf("\n===== First 5 Lines of system.log =====\n");

    while (count < 5 &&
           fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
        count++;
    }

    fclose(fp);
}

/* Function to display last 5 lines */
void display_last_five(const char *filename)
{
    FILE *fp;
    char line[200];
    char *last_five[5];
    int count = 0;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror(filename);
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (count < 5)
        {
            last_five[count] = malloc(strlen(line) + 1);

            if (last_five[count] == NULL)
            {
                perror("malloc");
                fclose(fp);
                return;
            }

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

            if (last_five[4] == NULL)
            {
                perror("malloc");
                fclose(fp);
                return;
            }

            strcpy(last_five[4], line);
        }
    }

    fclose(fp);

    printf("\n===== Last 5 Lines of system.log =====\n");

    for (int i = 0; i < count; i++)
    {
        printf("%s", last_five[i]);
        free(last_five[i]);
    }
}

/* Function to display process information */
void display_process_info(const char *stage)
{
    printf("\n[%s]\n", stage);
    printf("PID  : %d\n", getpid());
    printf("PPID : %d\n", getppid());
}

int main()
{
    FILE *fp;
    pid_t pid;

    /* =====================================================
       PART 1: STUDENT PORTAL FILE MANAGEMENT
       ===================================================== */

    printf("============================================\n");
    printf("       STUDENT PORTAL FILE MANAGEMENT\n");
    printf("============================================\n");

    /* 1. Create all directories */
    if (mkdir("student_portal", 0755) == 0)
        printf("student_portal created.\n");
    else
        printf("student_portal already exists.\n");

    mkdir("student_portal/src", 0755);
    mkdir("student_portal/docs", 0755);
    mkdir("student_portal/backup", 0755);
    mkdir("student_portal/logs", 0755);
    mkdir("student_portal/config", 0755);

    printf("src, docs, backup, logs and config created.\n");

    /* 2 & 3. Create C files inside src */

    fp = fopen("student_portal/src/main.c", "w");

    if (fp == NULL)
    {
        perror("main.c");
        return 1;
    }

    fprintf(fp, "#include <stdio.h>\n");
    fprintf(fp, "int main()\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    printf(\"Student Portal\\n\");\n");
    fprintf(fp, "    return 0;\n");
    fprintf(fp, "}\n");

    fclose(fp);

    fp = fopen("student_portal/src/student.c", "w");

    if (fp == NULL)
    {
        perror("student.c");
        return 1;
    }

    fprintf(fp, "#include <stdio.h>\n");
    fprintf(fp, "void student_info()\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    printf(\"Student information\\n\");\n");
    fprintf(fp, "}\n");

    fclose(fp);

    /* 4. Create README.txt inside docs */

    fp = fopen("student_portal/docs/README.txt", "w");

    if (fp == NULL)
    {
        perror("README.txt");
        return 1;
    }

    fprintf(fp, "Student Portal Project\n");
    fprintf(fp, "This project manages student information.\n");

    fclose(fp);

    /* 5. Create config.txt inside config */

    fp = fopen("student_portal/config/config.txt", "w");

    if (fp == NULL)
    {
        perror("config.txt");
        return 1;
    }

    fprintf(fp, "database=student_portal\n");
    fprintf(fp, "version=1.0\n");

    fclose(fp);

    /* 6 & 7. Create system.log with 15 lines */

    fp = fopen("student_portal/logs/system.log", "w");

    if (fp == NULL)
    {
        perror("system.log");
        return 1;
    }

    fprintf(fp, "Log 1: Student Portal started.\n");
    fprintf(fp, "Log 2: System initialized.\n");
    fprintf(fp, "Log 3: Configuration loaded.\n");
    fprintf(fp, "Log 4: Database connection established.\n");
    fprintf(fp, "Log 5: Student module loaded.\n");
    fprintf(fp, "Log 6: User authentication checked.\n");
    fprintf(fp, "Log 7: Student records accessed.\n");
    fprintf(fp, "Log 8: Student information updated.\n");
    fprintf(fp, "Log 9: Backup process started.\n");
    fprintf(fp, "Log 10: Backup process completed.\n");
    fprintf(fp, "Log 11: System permissions checked.\n");
    fprintf(fp, "Log 12: Configuration verified.\n");
    fprintf(fp, "Log 13: Log monitoring active.\n");
    fprintf(fp, "Log 14: System running normally.\n");
    fprintf(fp, "Log 15: Student Portal task completed.\n");

    fclose(fp);

    printf("All required files created.\n");

    /* 8. Display first 5 lines */
    display_first_five("student_portal/logs/system.log");

    /* 9. Display last 5 lines */
    display_last_five("student_portal/logs/system.log");

    /* 10. Copy README.txt to backup */
    if (copy_file("student_portal/docs/README.txt",
                  "student_portal/backup/README.txt"))
    {
        printf("\nREADME.txt copied to backup.\n");
    }

    /* 11. Rename backup */
    if (rename("student_portal/backup/README.txt",
               "student_portal/backup/README_backup.txt") == 0)
    {
        printf("Backup renamed to README_backup.txt.\n");
    }
    else
    {
        perror("rename");
    }

    /* 12. Change config.txt permission to 600 */
    if (chmod("student_portal/config/config.txt", 0600) == 0)
    {
        printf("config.txt permission changed to 600.\n");
    }
    else
    {
        perror("chmod config.txt");
    }

    /* 13. Change README.txt permission to 644 */
    if (chmod("student_portal/docs/README.txt", 0644) == 0)
    {
        printf("README.txt permission changed to 644.\n");
    }
    else
    {
        perror("chmod README.txt");
    }

    /* 14. Delete README_backup.txt */
    if (remove("student_portal/backup/README_backup.txt") == 0)
    {
        printf("README_backup.txt deleted.\n");
    }
    else
    {
        perror("README_backup.txt");
    }

    /* 15. Remove empty backup directory */
    if (rmdir("student_portal/backup") == 0)
    {
        printf("Empty backup directory removed.\n");
    }
    else
    {
        perror("backup");
    }

    /* =====================================================
       PART 2: PROCESS CREATION USING fork()
       ===================================================== */

    printf("\n============================================\n");
    printf("          PROCESS CREATION USING fork()\n");
    printf("============================================\n");

    display_process_info("Before fork()");

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    /* Child process */
    if (pid == 0)
    {
        display_process_info("Child Process - Running");

        printf("Child: My PID is %d\n", getpid());
        printf("Child: My Parent PID is %d\n", getppid());

        printf("Child: Entering waiting state using sleep().\n");

        sleep(3);

        display_process_info("Child Process - Running Again");

        printf("Child: Work completed.\n");

        printf("Child: Terminating now.\n");

        exit(0);
    }

    /* Parent process */
    else
    {
        display_process_info("Parent Process - Running");

        printf("Parent: Created child with PID %d\n", pid);

        printf("Parent: Waiting for child to finish.\n");

        wait(NULL);

        printf("Parent: Child process has terminated.\n");

        display_process_info("Parent Process - Final Stage");
    }

    /* =====================================================
       PART 3: FINAL DIRECTORY STRUCTURE
       ===================================================== */

    printf("\n============================================\n");
    printf("       FINAL DIRECTORY STRUCTURE\n");
    printf("============================================\n");

    /*
       Requirement 16:
       Display final structure using ls -R
    */

    system("ls -R student_portal");

    printf("\n============================================\n");
    printf("Task 10 completed successfully.\n");
    printf("============================================\n");

    return 0;
}
