#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];
    pid_t producer, consumer;

    /* Create anonymous pipe */
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    /* Create Producer Process */
    producer = fork();

    if (producer < 0)
    {
        perror("fork");
        return 1;
    }

    if (producer == 0)
    {
        /* Producer: ls -l */

        close(pipefd[0]);  // Close unused read end

        /* Redirect stdout to pipe */
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }

        close(pipefd[1]);

        /* Replace process with ls -l */
        execlp("ls", "ls", "-l", (char *)NULL);

        /* Executes only if execlp fails */
        perror("execlp ls");
        exit(1);
    }

    /* Create Consumer Process */
    consumer = fork();

    if (consumer < 0)
    {
        perror("fork");
        return 1;
    }

    if (consumer == 0)
    {
        /* Consumer: grep ".c" */

        close(pipefd[1]);  // Close unused write end

        /* Redirect stdin from pipe */
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }

        close(pipefd[0]);

        /* Replace process with grep ".c" */
        execlp("grep", "grep", ".c", (char *)NULL);

        /* Executes only if execlp fails */
        perror("execlp grep");
        exit(1);
    }

    /* Parent / Supervisor Process */

    /* Parent must close both pipe ends */
    close(pipefd[0]);
    close(pipefd[1]);

    /* Wait for producer */
    if (waitpid(producer, NULL, 0) == -1)
    {
        perror("waitpid producer");
    }

    /* Wait for consumer */
    if (waitpid(consumer, NULL, 0) == -1)
    {
        perror("waitpid consumer");
    }

    printf("\nProducer and Consumer processes completed.\n");

    return 0;
}
