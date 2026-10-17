                                                                                     Q3.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0)
    {
        printf("Fork failed\n");
        return 1;
    }
    else if (pid == 0)
    {
        printf("Child process created\n");
        printf("Executing ls -la:\n");

        execlp("ls", "ls", "-la", (char *)NULL);

        perror("exec failed");
        exit(1);
    }
    else
    {
        printf("Parent process waiting for child...\n");

        wait(&status);

        if (WIFEXITED(status))
            printf("Child exited with code: %d\n", WEXITSTATUS(status));
        else
            printf("Child terminated abnormally\n");

        printf("Parent process continues execution\n");
    }

    return 0;
}



