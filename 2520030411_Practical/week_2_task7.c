#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    FILE *fp;
    const char *filename = "permission_lab/test.txt";

    /* 1. Create permission_lab directory */
    if (mkdir("permission_lab", 0755) == 0)
    {
        printf("Directory permission_lab created.\n");
    }
    else
    {
        printf("Directory permission_lab already exists.\n");
    }

    /* 2. Create test.txt */
    fp = fopen(filename, "w");

    if (fp == NULL)
    {
        perror("test.txt");
        return 1;
    }

    /* 3. Add content */
    fprintf(fp, "This is a permission testing file.\n");
    fprintf(fp, "We are testing Linux file permissions.\n");
    fclose(fp);

    /* Give normal read/write permission */
    chmod(filename, 0644);

    printf("\nFile test.txt created successfully.\n");

    /* 4. Remove read permission */
    printf("\n===== 4. Removing Read Permission =====\n");

    if (chmod(filename, 0200) == 0)
    {
        printf("Read permission removed.\n");
    }
    else
    {
        perror("chmod");
    }

    /* 5. Try to read using cat */
    printf("\n===== 5. Trying to Read Using cat =====\n");
    system("cat permission_lab/test.txt");

    printf("The file cannot normally be read because read permission is removed.\n");

    /* 6. Restore read permission */
    printf("\n===== 6. Restoring Read Permission =====\n");

    if (chmod(filename, 0644) == 0)
    {
        printf("Read permission restored.\n");
    }

    printf("\nReading file again:\n");
    system("cat permission_lab/test.txt");

    /* 7. Remove write permission */
    printf("\n===== 7. Removing Write Permission =====\n");

    if (chmod(filename, 0444) == 0)
    {
        printf("Write permission removed.\n");
    }

    /* 8. Try to modify the file */
    printf("\n===== 8. Trying to Modify the File =====\n");

    fp = fopen(filename, "a");

    if (fp == NULL)
    {
        printf("Modification failed: write permission is removed.\n");
    }
    else
    {
        fprintf(fp, "This line should not be added.\n");
        fclose(fp);
        printf("File was modified.\n");
    }

    /* 9. Restore write permission */
    printf("\n===== 9. Restoring Write Permission =====\n");

    if (chmod(filename, 0644) == 0)
    {
        printf("Write permission restored.\n");
    }

    /* 10. Remove execute permission */
    printf("\n===== 10. Removing Execute Permission =====\n");

    if (chmod(filename, 0644) == 0)
    {
        printf("Execute permission is removed.\n");
    }

    /* 11. Try to execute the file */
    printf("\n===== 11. Trying to Execute test.txt =====\n");

    printf("Command: ./permission_lab/test.txt\n");
    system("./permission_lab/test.txt");

    printf("\nExecution failed because test.txt does not have execute permission.\n");

    printf("\n===== Task 7 Completed =====\n");

    return 0;
}
