#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

/* Function to list contents of a directory */
void list_directory(const char *path)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);

    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    printf("\nContents of %s:\n", path);

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 &&
            strcmp(entry->d_name, "..") != 0)
        {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}

int main()
{
    FILE *fp;

    /* Create organization directory */
    if (mkdir("organization", 0755) == 0)
    {
        printf("organization directory created.\n");
    }
    else
    {
        printf("organization directory already exists.\n");
    }

    /* Create initial files */
    fp = fopen("organization/file1.txt", "w");
    if (fp != NULL)
    {
        fprintf(fp, "This is file1.\n");
        fclose(fp);
    }

    fp = fopen("organization/file2.txt", "w");
    if (fp != NULL)
    {
        fprintf(fp, "This is file2.\n");
        fclose(fp);
    }

    fp = fopen("organization/file3.txt", "w");
    if (fp != NULL)
    {
        fprintf(fp, "This is file3.\n");
        fclose(fp);
    }

    fp = fopen("organization/image1.jpg", "w");
    if (fp != NULL)
        fclose(fp);

    fp = fopen("organization/image2.jpg", "w");
    if (fp != NULL)
        fclose(fp);

    printf("Initial files created.\n");

    /* 1. Create documents directory */
    if (mkdir("organization/documents", 0755) == 0)
    {
        printf("documents directory created.\n");
    }
    else
    {
        printf("documents directory already exists.\n");
    }

    /* 2. Create images directory */
    if (mkdir("organization/images", 0755) == 0)
    {
        printf("images directory created.\n");
    }
    else
    {
        printf("images directory already exists.\n");
    }

    /* 3. Move all .txt files into documents */
    if (rename("organization/file1.txt",
               "organization/documents/file1.txt") == 0)
    {
        printf("file1.txt moved to documents.\n");
    }
    else
    {
        perror("file1.txt");
    }

    if (rename("organization/file2.txt",
               "organization/documents/file2.txt") == 0)
    {
        printf("file2.txt moved to documents.\n");
    }
    else
    {
        perror("file2.txt");
    }

    if (rename("organization/file3.txt",
               "organization/documents/file3.txt") == 0)
    {
        printf("file3.txt moved to documents.\n");
    }
    else
    {
        perror("file3.txt");
    }

    /* 4. Move all .jpg files into images */
    if (rename("organization/image1.jpg",
               "organization/images/image1.jpg") == 0)
    {
        printf("image1.jpg moved to images.\n");
    }
    else
    {
        perror("image1.jpg");
    }

    if (rename("organization/image2.jpg",
               "organization/images/image2.jpg") == 0)
    {
        printf("image2.jpg moved to images.\n");
    }
    else
    {
        perror("image2.jpg");
    }

    /* 5. List contents of each directory */
    list_directory("organization");
    list_directory("organization/documents");
    list_directory("organization/images");

    /* 6. Rename file1.txt to notes.txt */
    if (rename("organization/documents/file1.txt",
               "organization/documents/notes.txt") == 0)
    {
        printf("\nfile1.txt renamed to notes.txt.\n");
    }
    else
    {
        perror("file1.txt");
    }

    /* 7. Rename image1.jpg to photo.jpg */
    if (rename("organization/images/image1.jpg",
               "organization/images/photo.jpg") == 0)
    {
        printf("image1.jpg renamed to photo.jpg.\n");
    }
    else
    {
        perror("image1.jpg");
    }

    /* Display final contents */
    printf("\n===== Final Directory Structure =====\n");

    list_directory("organization");
    list_directory("organization/documents");
    list_directory("organization/images");

    printf("\nTask 4 completed successfully.\n");

    return 0;
}
