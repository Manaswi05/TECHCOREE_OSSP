#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define BUFFER_SIZE 256

volatile sig_atomic_t running = 1;

/* Signal handler */
void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        running = 0;
    }
    else if (sig == SIGUSR1)
    {
        printf("\nClient received SIGUSR1.\n");
    }
}

int main(int argc, char *argv[])
{
    int client_id;
    pid_t server_pid;

    char client_to_server[50];
    char server_to_client[50];

    char message[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    if (argc != 3)
    {
        printf("Usage: %s <client_id> <server_pid>\n", argv[0]);
        printf("Example: %s 1 12345\n", argv[0]);
        return 1;
    }

    client_id = atoi(argv[1]);
    server_pid = (pid_t)atoi(argv[2]);

    if (client_id < 1 || client_id > 3)
    {
        printf("Client ID must be 1, 2 or 3.\n");
        return 1;
    }

    snprintf(client_to_server, sizeof(client_to_server),
             "client%d_to_server", client_id);

    snprintf(server_to_client, sizeof(server_to_client),
             "server_to_client%d", client_id);

    signal(SIGINT, handle_signal);
    signal(SIGUSR1, handle_signal);

    printf("============================================\n");
    printf("              CLIENT %d\n", client_id);
    printf("============================================\n");

    printf("Client PID : %d\n", getpid());
    printf("Server PID : %d\n", server_pid);

    /* Open Client -> Server FIFO */
    int write_fd = open(client_to_server, O_WRONLY);

    if (write_fd == -1)
    {
        perror("open client_to_server");
        return 1;
    }

    /* Open Server -> Client FIFO */
    int read_fd = open(server_to_client, O_RDONLY);

    if (read_fd == -1)
    {
        perror("open server_to_client");
        close(write_fd);
        return 1;
    }

    printf("\nConnected to server.\n");
    printf("Type messages. Type 'exit' to disconnect.\n\n");

    while (running)
    {
        printf("Client %d > ", client_id);
        fflush(stdout);

        if (fgets(message, sizeof(message), stdin) == NULL)
        {
            break;
        }

        /* Send message to server */
        if (write(write_fd, message, strlen(message)) == -1)
        {
            perror("write");
            break;
        }

        /* Notify server */
        kill(server_pid, SIGUSR1);

        if (strncmp(message, "exit", 4) == 0)
        {
            break;
        }

        /* Read server response */
        memset(response, 0, sizeof(response));

        ssize_t bytes_read =
            read(read_fd, response, sizeof(response) - 1);

        if (bytes_read > 0)
        {
            response[bytes_read] = '\0';

            printf("%s", response);
        }
        else if (bytes_read == -1 && errno != EINTR)
        {
            perror("read");
            break;
        }
    }

    close(write_fd);
    close(read_fd);

    printf("\nClient %d terminated.\n", client_id);

    return 0;
}
