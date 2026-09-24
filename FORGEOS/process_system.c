#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

/*
 * Linux Process Creation, Execution and Termination System
 *
 * Demonstrates:
 * fork()
 * getpid()
 * getppid()
 * exec()
 * wait()
 * waitpid()
 * exit()
 * kill()
 * Signals
 * Zombie process
 * Orphan process
 */

void print_status(int status)
{
    if (WIFEXITED(status))
    {
        printf("[Parent] Child exited normally with status = %d\n",
               WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        printf("[Parent] Child terminated by signal = %d\n",
               WTERMSIG(status));
    }
    else if (WIFSTOPPED(status))
    {
        printf("[Parent] Child stopped by signal = %d\n",
               WSTOPSIG(status));
    }
}

/* ---------------------------------------------------------
   1. PROCESS CREATION
   --------------------------------------------------------- */

void process_creation()
{
    printf("\n========================================\n");
    printf("       PROCESS CREATION\n");
    printf("========================================\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        // Child process
        printf("[Child]\n");
        printf("PID  = %d\n", getpid());
        printf("PPID = %d\n", getppid());

        printf("[Child] Process created successfully.\n");

        exit(0);
    }
    else
    {
        // Parent process
        printf("[Parent]\n");
        printf("PID       = %d\n", getpid());
        printf("Child PID = %d\n", pid);

        int status;

        waitpid(pid, &status, 0);

        printf("[Parent] Child process completed.\n");
        print_status(status);
    }
}


/* ---------------------------------------------------------
   2. EXECUTION USING EXEC()
   --------------------------------------------------------- */

void process_execution()
{
    printf("\n========================================\n");
    printf("       PROCESS EXECUTION\n");
    printf("========================================\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        printf("[Child] PID = %d\n", getpid());
        printf("[Child] Executing 'ls -l' using exec().\n\n");

        /*
         * execl() replaces the current child process
         * with the /bin/ls program.
         */
        execl("/bin/ls", "ls", "-l", NULL);

        // Only reached if exec fails
        perror("exec failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;

        printf("[Parent] Waiting for child PID %d...\n", pid);

        waitpid(pid, &status, 0);

        printf("[Parent] exec child finished.\n");
        print_status(status);
    }
}


/* ---------------------------------------------------------
   3. WAIT() SYNCHRONIZATION
   --------------------------------------------------------- */

void process_synchronization()
{
    printf("\n========================================\n");
    printf("       PROCESS SYNCHRONIZATION\n");
    printf("========================================\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        printf("[Child] PID = %d\n", getpid());
        printf("[Child] Working for 3 seconds...\n");

        sleep(3);

        printf("[Child] Work completed.\n");

        exit(42);
    }
    else
    {
        int status;

        printf("[Parent] Waiting using waitpid()...\n");

        waitpid(pid, &status, 0);

        printf("[Parent] Child has finished.\n");

        if (WIFEXITED(status))
        {
            printf("[Parent] Exit status received = %d\n",
                   WEXITSTATUS(status));
        }
    }
}


/* ---------------------------------------------------------
   4. SIGNAL AND KILL()
   --------------------------------------------------------- */

volatile sig_atomic_t signal_received = 0;

void signal_handler(int signal)
{
    signal_received = signal;

    printf("\n[Child] Signal %d received!\n", signal);

    if (signal == SIGTERM)
    {
        printf("[Child] Termination signal received.\n");
        printf("[Child] Exiting safely...\n");
    }
}

void signal_control()
{
    printf("\n========================================\n");
    printf("       SIGNAL / PROCESS CONTROL\n");
    printf("========================================\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        // Child installs signal handler
        signal(SIGTERM, signal_handler);

        printf("[Child] PID = %d\n", getpid());
        printf("[Child] Waiting for SIGTERM...\n");

        while (!signal_received)
        {
            sleep(1);
        }

        exit(0);
    }
    else
    {
        printf("[Parent] Child PID = %d\n", pid);

        sleep(2);

        printf("[Parent] Sending SIGTERM using kill()...\n");

        if (kill(pid, SIGTERM) == -1)
        {
            perror("kill failed");
        }

        int status;

        waitpid(pid, &status, 0);

        printf("[Parent] Signal-controlled child terminated.\n");

        print_status(status);
    }
}


/* ---------------------------------------------------------
   5. EXPLICIT TERMINATION USING EXIT()
   --------------------------------------------------------- */

void process_termination()
{
    printf("\n========================================\n");
    printf("       PROCESS TERMINATION\n");
    printf("========================================\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        printf("[Child] PID = %d\n", getpid());
        printf("[Child] Performing some work...\n");

        sleep(2);

        printf("[Child] Calling exit(10)...\n");

        exit(10);
    }
    else
    {
        int status;

        waitpid(pid, &status, 0);

        printf("[Parent] Child termination detected.\n");

        if (WIFEXITED(status))
        {
            printf("[Parent] Child exit status = %d\n",
                   WEXITSTATUS(status));
        }
    }
}


/* ---------------------------------------------------------
   6. ZOMBIE PROCESS
   --------------------------------------------------------- */

void zombie_process()
{
    printf("\n========================================\n");
    printf("          ZOMBIE PROCESS\n");
    printf("========================================\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        printf("[Child] PID = %d\n", getpid());
        printf("[Child] Exiting immediately...\n");

        exit(0);
    }
    else
    {
        printf("[Parent] PID = %d\n", getpid());
        printf("[Parent] Child PID = %d\n", pid);

        printf("[Parent] Sleeping for 10 seconds.\n");
        printf("[Parent] During this time the child becomes a ZOMBIE.\n");

        printf("\nOpen another terminal and run:\n");
        printf("ps -eo pid,ppid,stat,cmd | grep defunct\n\n");

        sleep(10);

        /*
         * Reap the zombie.
         */
        waitpid(pid, NULL, 0);

        printf("[Parent] Zombie child has now been reaped.\n");
    }
}


/* ---------------------------------------------------------
   7. ORPHAN PROCESS
   --------------------------------------------------------- */

void orphan_process()
{
    printf("\n========================================\n");
    printf("          ORPHAN PROCESS\n");
    printf("========================================\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        printf("[Child] Original PID  = %d\n", getpid());
        printf("[Child] Original PPID = %d\n", getppid());

        printf("[Child] Sleeping while parent exits...\n");

        sleep(5);

        printf("\n[Child] Parent has terminated.\n");
        printf("[Child] New PPID = %d\n", getppid());

        printf("[Child] I am now an orphan process.\n");

        exit(0);
    }
    else
    {
        printf("[Parent] PID = %d\n", getpid());
        printf("[Parent] Child PID = %d\n", pid);

        printf("[Parent] Exiting before child...\n");

        exit(0);
    }
}


/* ---------------------------------------------------------
   MAIN MENU
   --------------------------------------------------------- */

void display_menu()
{
    printf("\n\n");
    printf("=============================================\n");
    printf("   LINUX PROCESS MANAGEMENT SYSTEM\n");
    printf("=============================================\n");

    printf("1. Process Creation\n");
    printf("2. Process Execution using exec()\n");
    printf("3. Process Synchronization\n");
    printf("4. Signal / Process Control\n");
    printf("5. Process Termination\n");
    printf("6. Zombie Process Demonstration\n");
    printf("7. Orphan Process Demonstration\n");
    printf("8. Run Complete Demonstration\n");
    printf("0. Exit\n");

    printf("---------------------------------------------\n");
    printf("Enter your choice: ");
}


int main()
{
    int choice;

    while (1)
    {
        display_menu();

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input.\n");

            while (getchar() != '\n');

            continue;
        }

        switch (choice)
        {
            case 1:
                process_creation();
                break;

            case 2:
                process_execution();
                break;

            case 3:
                process_synchronization();
                break;

            case 4:
                signal_control();
                break;

            case 5:
                process_termination();
                break;

            case 6:
                zombie_process();
                break;

            case 7:
                orphan_process();
                break;

            case 8:

                process_creation();

                process_execution();

                process_synchronization();

                signal_control();

                process_termination();

                printf("\nComplete demonstration finished.\n");

                break;

            case 0:
                printf("\nExiting system...\n");
                printf("Goodbye!\n");
                return 0;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }

    return 0;
}