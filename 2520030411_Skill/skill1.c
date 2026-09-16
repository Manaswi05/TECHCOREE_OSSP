#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define BUFFER_SIZE 100

void enable_raw_mode(struct termios *old)
{
    struct termios new;

    tcgetattr(STDIN_FILENO, old);
    new = *old;

    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
}

void disable_raw_mode(struct termios *old)
{
    tcsetattr(STDIN_FILENO, TCSANOW, old);
}

int main()
{
    char input[BUFFER_SIZE];
    int length;
    char ch;
    struct termios old;

    enable_raw_mode(&old);

    printf("Forge OS Interactive Shell\n");
    printf("Type a command or 'exit' to quit.\n\n");

    while (1)
    {
        length = 0;
        input[0] = '\0';

        printf("ForgeOS> ");
        fflush(stdout);

        while (1)
        {
            ch = getchar();

            if (ch == '\n' || ch == '\r')
            {
                input[length] = '\0';
                printf("\n");
                break;
            }

            if (ch == 127 || ch == '\b')
            {
                if (length > 0)
                {
                    length--;
                    input[length] = '\0';

                    printf("\b \b");
                    fflush(stdout);
                }
                continue;
            }

            if (length < BUFFER_SIZE - 1)
            {
                input[length++] = ch;
                input[length] = '\0';

                putchar(ch);
                fflush(stdout);
            }
        }

        if (length == 0)
        {
            continue;
        }

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting Forge OS...\n");
            break;
        }

        if (strcmp(input, "help") == 0)
        {
            printf("Available commands:\n");
            printf("  help  - Show available commands\n");
            printf("  hello - Display greeting\n");
            printf("  exit  - Exit the shell\n");
        }
        else if (strcmp(input, "hello") == 0)
        {
            printf("Hello from Forge OS!\n");
        }
        else
        {
            printf("Command received: %s\n", input);
        }
    }

    disable_raw_mode(&old);

    return 0;
}
