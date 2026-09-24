#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int count = 0;

int main() {
    int pid;

    pid = fork();

    if (pid < 0) {
        perror("Problem in creating child");
        exit(0);
    }

    else if (pid == 0) {
        // Child process
        printf("Child process is executing\n");

        for (int i = 0; i < 5; i++) {
            count--;
            printf("Child count: %d\n", count);
            sleep(5);
        }
    }

    else {
        // Parent process
        printf("Parent process is executing\n");

        for (int i = 0; i < 5; i++) {
            count++;
            printf("Parent count: %d\n", count);
            sleep(5);
        }

        wait(NULL);

        printf("\nFinal parent count: %d\n", count);
    }

    return 0;
}
