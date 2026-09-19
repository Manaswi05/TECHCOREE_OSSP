#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define MAX_CLIENTS 3
#define BUFFER_SIZE 256

volatile sig_atomic_t server_running = 1;
volatile sig_atomic_t signal_received = 0;

/* Signal handler */
void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        server_running = 0;
    }
    else if (sig == SIGUSR1)
    {
        signal_received = 1;
    }
}

/* Handler process for each client */
void client_handler(int client_id, pid_t parent_pid)
{
    char client_to_server[50];
    char server_to_client[50];
    char buffer[BUFFER_SIZE];

    snprintf(client_to_server, sizeof(client_to_server),
             "client%d_to_server", client_id);

    snprintf(server_to_client, sizeof(server_to_client),
             "server_to_client%d", client_id);

    signal(SIGUSR1, handle_signal);
    signal(SIGINT, handle_signal);

    printf("Handler %d started. PID = %d\n",
           client_id, getpid());

    /* Open client -> server FIFO */
    int read_fd = open(client_to_server, O_RDONLY);

    if (read_fd == -1)
    {
        perror("open client_to_server");
        exit(1);
    }

    /* Open server -> client FIFO */
    int write_fd = open(server_to_client, O_WRONLY);

    if (write_fd == -1)
    {
        perror("open server_to_client");
        close(read_fd);
        exit(1);
    }

    while (server_running)
    {
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytes_read = read(read_fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';

            printf("Handler %d received: %s",
                   client_id, buffer);

            if (strcmp(buffer, "exit\n") == 0 ||
                strcmp(buffer, "exit") == 0)
            {
                snprintf(buffer, sizeof(buffer),
                         "Server: Client %d disconnected.\n",
                         client_id);

                write(write_fd, buffer, strlen(buffer));
                break;
            }

            char response[BUFFER_SIZE];

            snprintf(response, sizeof(response),
                     "Server response to Client %d: Message received successfully.\n",
                     client_id);

            write(write_fd, response, strlen(response));

            /* Notify server using SIGUSR1 */
            kill(parent_pid, SIGUSR1);
        }
        else if (bytes_read == 0)
        {
            break;
        }
        else if (errno == EINTR)
        {
            continue;
        }
        else
        {
            perror("read");
            break;
        }
    }

    close(read_fd);
    close(write_fd);

    printf("Handler %d terminated.\n", client_id);

    exit(0);
}

int main()
{
    pid_t handlers[MAX_CLIENTS];

    char client_to_server[50];
    char server_to_client[50];

    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    printf("============================================\n");
    printf("        MULTI-CLIENT FIFO SERVER\n");
    printf("============================================\n");

    printf("Server PID: %d\n", getpid());

    /* Create FIFOs */
    for (int i = 1; i <= MAX_CLIENTS; i++)
    {
        snprintf(client_to_server, sizeof(client_to_server),
                 "client%d_to_server", i);

        snprintf(server_to_client, sizeof(server_to_client),
                 "server_to_client%d", i);

        unlink(client_to_server);
        unlink(server_to_client);

        if (mkfifo(client_to_server, 0666) == -1)
        {
            perror("mkfifo client_to_server");
            return 1;
        }

        if (mkfifo(server_to_client, 0666) == -1)
        {
            perror("mkfifo server_to_client");
            return 1;
        }
    }

    printf("\nAll FIFOs created.\n");

    /* Create handler processes */
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        handlers[i] = fork();

        if (handlers[i] < 0)
        {
            perror("fork");
            return 1;
        }

        if (handlers[i] == 0)
        {
            client_handler(i + 1, getppid());
        }
    }

    printf("All client handlers created.\n");

    printf("\nWaiting for clients...\n");
    printf("Use Ctrl+C to stop the server.\n\n");

    /* Server main loop */
    while (server_running)
    {
        if (signal_received)
        {
            printf("Server received SIGUSR1 notification.\n");
            signal_received = 0;
        }

        sleep(1);
    }

    printf("\nServer shutting down...\n");

    /* Send SIGINT to handlers */
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        kill(handlers[i], SIGINT);
    }

    /* Wait for all handlers */
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        waitpid(handlers[i], NULL, 0);
    }

    /* Remove FIFOs */
    for (int i = 1; i <= MAX_CLIENTS; i++)
    {
        snprintf(client_to_server, sizeof(client_to_server),
                 "client%d_to_server", i);

        snprintf(server_to_client, sizeof(server_to_client),
                 "server_to_client%d", i);

        unlink(client_to_server);
        unlink(server_to_client);
    }

    printf("All handlers terminated.\n");
    printf("All FIFOs removed.\n");
    printf("Server terminated.\n");

    return 0;
}
