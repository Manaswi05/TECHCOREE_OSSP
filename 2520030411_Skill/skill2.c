#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define INITIAL_SIZE 16

/* Node for command history */
typedef struct HistoryNode
{
    char *command;
    struct HistoryNode *next;
} HistoryNode;

/* Disable terminal raw mode */
void disable_raw_mode(struct termios *old)
{
    tcsetattr(STDIN_FILENO, TCSANOW, old);
}

/* Enable terminal raw mode */
void enable_raw_mode(struct termios *old)
{
    struct termios new;

    tcgetattr(STDIN_FILENO, old);
    new = *old;

    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
}

/* Add command to history */
void add_history(HistoryNode **head, const char *command)
{
    HistoryNode *new_node;
    HistoryNode *temp;

    new_node = malloc(sizeof(HistoryNode));

    if (new_node == NULL)
    {
        perror("malloc");
        return;
    }

    new_node->command = malloc(strlen(command) + 1);

    if (new_node->command == NULL)
    {
        perror("malloc");
        free(new_node);
        return;
    }

    strcpy(new_node->command, command);
    new_node->next = NULL;

    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        temp = *head;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = new_node;
    }
}

/* Count history entries */
int history_count(HistoryNode *head)
{
    int count = 0;

    while (head != NULL)
    {
        count++;
        head = head->next;
    }

    return count;
}

/* Get command at a particular position */
char *get_history_command(HistoryNode *head, int position)
{
    int i = 0;

    while (head != NULL)
    {
        if (i == position)
        {
            return head->command;
        }

        head = head->next;
        i++;
    }

    return NULL;
}

/* Free complete history list */
void free_history(HistoryNode *head)
{
    HistoryNode *temp;

    while (head != NULL)
    {
        temp = head->next;

        free(head->command);
        free(head);

        head = temp;
    }
}

/* Clear the current line on screen */
void clear_line(int length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        printf("\b \b");
    }

    fflush(stdout);
}

/* Copy history command into input buffer */
void update_buffer(char **buffer, size_t *capacity,
                   size_t *length, const char *command)
{
    size_t required;

    required = strlen(command) + 1;

    /* Resize buffer if required */
    while (required > *capacity)
    {
        *capacity *= 2;

        *buffer = realloc(*buffer, *capacity);

        if (*buffer == NULL)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
    }

    strcpy(*buffer, command);
    *length = strlen(command);

    printf("%s", *buffer);
    fflush(stdout);
}

int main()
{
    struct termios old_terminal;

    char *input;
    size_t capacity;
    size_t length;

    HistoryNode *history = NULL;

    int history_position;
    int count;

    char ch;

    /* Dynamically allocate input buffer */
    capacity = INITIAL_SIZE;

    input = malloc(capacity);

    if (input == NULL)
    {
        perror("malloc");
        return 1;
    }

    input[0] = '\0';

    enable_raw_mode(&old_terminal);

    printf("====================================\n");
    printf(" ForgeOS Command History System\n");
    printf("====================================\n");
    printf("Type commands and press Enter.\n");
    printf("Use UP/DOWN arrows for history.\n");
    printf("Type exit to quit.\n\n");

    while (1)
    {
        length = 0;
        input[0] = '\0';

        history_position = history_count(history);

        printf("ForgeOS> ");
        fflush(stdout);

        while (1)
        {
            ch = getchar();

            /* Enter key */
            if (ch == '\n' || ch == '\r')
            {
                input[length] = '\0';

                printf("\n");
                break;
            }

            /* Backspace */
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

            /*
             * Escape sequence handling
             *
             * UP arrow:
             * ESC [ A
             *
             * DOWN arrow:
             * ESC [ B
             */
            if (ch == 27)
            {
                char second;
                char third;

                second = getchar();
                third = getchar();

                count = history_count(history);

                /* UP arrow */
                if (second == '[' && third == 'A')
                {
                    if (count > 0 && history_position > 0)
                    {
                        clear_line(length);

                        history_position--;

                        update_buffer(
                            &input,
                            &capacity,
                            &length,
                            get_history_command(
                                history,
                                history_position
                            )
                        );
                    }
                }

                /* DOWN arrow */
                else if (second == '[' && third == 'B')
                {
                    if (history_position < count - 1)
                    {
                        clear_line(length);

                        history_position++;

                        update_buffer(
                            &input,
                            &capacity,
                            &length,
                            get_history_command(
                                history,
                                history_position
                            )
                        );
                    }
                    else
                    {
                        clear_line(length);

                        history_position = count;
                        length = 0;
                        input[0] = '\0';
                    }
                }

                continue;
            }

            /* Dynamically resize buffer */
            if (length + 1 >= capacity)
            {
                capacity *= 2;

                input = realloc(input, capacity);

                if (input == NULL)
                {
                    perror("realloc");
                    disable_raw_mode(&old_terminal);
                    free_history(history);
                    return 1;
                }
            }

            /* Store normal character */
            input[length] = ch;
            length++;

            input[length] = '\0';

            putchar(ch);
            fflush(stdout);
        }

        /* Ignore empty commands */
        if (length == 0)
        {
            continue;
        }

        /* Exit condition */
        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting ForgeOS...\n");
            break;
        }

        /* Store command in history */
        add_history(&history, input);

        printf("Command received: %s\n", input);
    }

    /* Disable raw terminal mode */
    disable_raw_mode(&old_terminal);

    /* Release dynamically allocated memory */
    free(input);

    /* Release linked-list history */
    free_history(history);

    printf("Memory released successfully.\n");

    return 0;
}
