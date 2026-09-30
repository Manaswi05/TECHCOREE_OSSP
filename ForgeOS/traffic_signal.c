#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t running = 1;

void handle_sigterm(int sig)
{
    running = 0;
}

void traffic_signal(int road)
{
    signal(SIGTERM, handle_sigterm);

    printf("\n[ROAD %d] Traffic signal process started. PID: %d\n",
           road, getpid());
    fflush(stdout);

    while (running)
    {
        printf("[ROAD %d] GREEN - Vehicles can move\n", road);
        fflush(stdout);
        sleep(3);

        if (!running)
            break;

        printf("[ROAD %d] YELLOW - Slow down\n", road);
        fflush(stdout);
        sleep(2);

        if (!running)
            break;

        printf("[ROAD %d] RED - Vehicles must stop\n", road);
        fflush(stdout);
        sleep(3);
    }

    printf("[ROAD %d] Signal process terminated.\n", road);
    fflush(stdout);

    exit(0);
}

int main()
{
    pid_t road1, road2;
    int choice;
    int status;

    printf("============================================\n");
    printf("      TRAFFIC SIGNAL CONTROL SYSTEM\n");
    printf("============================================\n");

    road1 = fork();

    if (road1 < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (road1 == 0)
    {
        traffic_signal(1);
    }

    road2 = fork();

    if (road2 < 0)
    {
        perror("fork failed");
        kill(road1, SIGTERM);
        waitpid(road1, &status, 0);
        return 1;
    }

    if (road2 == 0)
    {
        traffic_signal(2);
    }

    printf("\n[CONTROLLER] Traffic Control Center started.\n");
    printf("[CONTROLLER] Controller PID: %d\n", getpid());
    printf("[CONTROLLER] Road 1 PID: %d\n", road1);
    printf("[CONTROLLER] Road 2 PID: %d\n", road2);

    while (1)
    {
        printf("\n========== TRAFFIC CONTROL MENU ==========\n");
        printf("1. Pause Road 1 Signal\n");
        printf("2. Resume Road 1 Signal\n");
        printf("3. Pause Road 2 Signal\n");
        printf("4. Resume Road 2 Signal\n");
        printf("5. Emergency Stop All Signals\n");
        printf("6. Exit\n");
        printf("==========================================\n");
        printf("Enter choice: ");
        fflush(stdout);

        if (scanf("%d", &choice) != 1)
        {
            printf("\n[CONTROLLER] Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice == 1)
        {
            kill(road1, SIGSTOP);
            printf("\n[CONTROLLER] SIGSTOP sent to Road 1.\n");
            printf("[CONTROLLER] Road 1 signal PAUSED.\n");
        }
        else if (choice == 2)
        {
            kill(road1, SIGCONT);
            printf("\n[CONTROLLER] SIGCONT sent to Road 1.\n");
            printf("[CONTROLLER] Road 1 signal RESUMED.\n");
        }
        else if (choice == 3)
        {
            kill(road2, SIGSTOP);
            printf("\n[CONTROLLER] SIGSTOP sent to Road 2.\n");
            printf("[CONTROLLER] Road 2 signal PAUSED.\n");
        }
        else if (choice == 4)
        {
            kill(road2, SIGCONT);
            printf("\n[CONTROLLER] SIGCONT sent to Road 2.\n");
            printf("[CONTROLLER] Road 2 signal RESUMED.\n");
        }
        else if (choice == 5)
        {
            printf("\n[CONTROLLER] Emergency Stop activated!\n");

            kill(road1, SIGTERM);
            kill(road2, SIGTERM);

            waitpid(road1, &status, 0);
            waitpid(road2, &status, 0);

            printf("[CONTROLLER] All traffic signal processes terminated.\n");
            break;
        }
        else if (choice == 6)
        {
            printf("\n[CONTROLLER] Shutting down traffic control system...\n");

            kill(road1, SIGTERM);
            kill(road2, SIGTERM);

            waitpid(road1, &status, 0);
            waitpid(road2, &status, 0);

            printf("[CONTROLLER] Traffic control system stopped.\n");
            break;
        }
        else
        {
            printf("\n[CONTROLLER] Invalid choice.\n");
        }
    }

    return 0;
}
