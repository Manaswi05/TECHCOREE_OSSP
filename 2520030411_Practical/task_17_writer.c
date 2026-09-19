#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int fd;
    int *marks;

    /* Create shared memory */
    fd = shm_open("/student1_marks", O_CREAT | O_RDWR, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    /* Set shared memory size */
    if (ftruncate(fd, 5 * sizeof(int)) == -1)
    {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    /* Map shared memory */
    marks = mmap(NULL, 5 * sizeof(int),
                 PROT_READ | PROT_WRITE,
                 MAP_SHARED, fd, 0);

    if (marks == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    /* Store student marks */
    marks[0] = 85;
    marks[1] = 90;
    marks[2] = 78;
    marks[3] = 88;
    marks[4] = 95;

    printf("Student 1 marks written to shared memory.\n");

    printf("Marks: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", marks[i]);
    }

    printf("\n");

    munmap(marks, 5 * sizeof(int));
    close(fd);

    return 0;
}
