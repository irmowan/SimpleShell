//
// Created by irmo on 15/9/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LINE 80
#define HISTORY_SIZE 10

struct HistoryCommands {
    int count;
    char *commands[HISTORY_SIZE];
};

void cd_command(char *args[], int num_args);

int check_command(char *command, struct HistoryCommands *history);

void copy_command(char *command, struct HistoryCommands *history, int k);

int divide_command(char *command, char *args[]);

void store_command(char *command, struct HistoryCommands *history);

int main(void) {
    struct HistoryCommands history;
    char *args[MAX_LINE / 2 + 1];
    int should_run = 1;
    history.count = 0;
    printf("Welcome to irmo's simple shell!\n");
    signal(SIGINT, NULL);
    while (should_run) {
        char *command = (char *) (malloc(MAX_LINE * sizeof(char)));
        int num_args = 0;
        int daemon = 0;
        printf("osh > ");
        fflush(stdout);
        fgets(command, MAX_LINE, stdin);
        command[strlen(command) - 1] = '\0';

        if (strlen(command) == 0) continue;

        // Check if the command is !!/!N
        if (check_command(command, &history) == 1) continue;

        // Store the command to history, then divide the command to args[]
        store_command(command, &history);
        num_args = divide_command(command, args);

        // Check the ampersand &
        if (strcmp(args[num_args - 1], "&") == 0) {
            daemon = 1;
            args[num_args - 1] = NULL;
            num_args--;
        }

        // Implement exit command, by changing the variable 'should_run'
        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }

        // Implement cd command, this doesn't need to create a child process
        if (strcmp(args[0], "cd") == 0) {
            cd_command(args, num_args);
            continue;
        }

        // Implement history command, this also doesn't need to create a child process
        if (strcmp(args[0], "history") == 0) {
            for (int i = history.count; i > 0; i--)
                printf("%d %s\n", i, history.commands[i - 1]);
            continue;
        }

        // Create a new process to execute the command
        pid_t new_pid = fork();
        if (new_pid == 0) {
            // Implement the child process
            execvp(args[0], args);
            printf("%s : Command not found.\n", args[0]);
            exit(0);
        }

        // Check whether the shell need to wait
        if (daemon == 0)
            waitpid(new_pid, NULL, 0);
    }
    return 0;
}

void cd_command(char *args[], int num_args) {
    char *path = args[1];
    char *home = (char *) malloc(MAX_LINE * sizeof(char));
    strcpy(home, getenv("HOME"));
    // Implement default cd command
    if (num_args == 1) {
        chdir(home);
        return;
    }
    // Implement HOME parameter
    if (args[1][0] == '~') {
        if ((strlen(args[1]) == 1) || (strlen(args[1]) == 2 && args[1][1] == '/')) {
            chdir(home);
            return;
        }
        // Joint a primitive path
        if (strlen(args[1]) > 2 && args[1][1] == '/') {
            path = strcat(strcat(home, "/"), args[1] + 2);
        }
    }
    // Run the normal cd command
    if (chdir(path) == -1) {
        printf("%s : No such file or directory.\n", args[1]);
    }
    return;
}

int check_command(char *command, struct HistoryCommands *history) {
    char *args_temp[MAX_LINE / 2 + 1];
    char *command_temp = (char *) (malloc(MAX_LINE * sizeof(char)));
    int num_args = 0;
    strcpy(command_temp, command);
    num_args = divide_command(command_temp, args_temp);
    if (num_args == 0) return 1;       // Handle the case of the all-space command
    if (args_temp[0][0] == '!') {
        if (num_args > 1) {
            printf("Please use only one parameter.\n");
            return 1;
        }
        if (strcmp(args_temp[0], "!!") == 0) {
            if (history->count == 0) {
                printf("No commands in history.\n");
                return 1;
            }
            else
                copy_command(command, history, history->count);
        }
        else {
            char *num_char = (char *) (malloc(MAX_LINE * sizeof(char)));
            int num = 0;
            if (strcmp(args_temp[0], "!") == 0) {
                printf("Event not found.\n");
                return 1;
            }
            strcpy(num_char, args_temp[0] + 1);
            num = atoi(num_char);   // It will return 0 when the parameter is not a number
            if (num <= 0 || num > history->count) {
                printf("No such command in history.\n");
                return 1;
            }
            copy_command(command, history, num);
        }
    }
    return 0;
}

void copy_command(char *command, struct HistoryCommands *history, int k) {
    strcpy(command, history->commands[k - 1]);
    printf("%s\n", command);
    return;
}

int divide_command(char *command, char *args[]) {
    int count = 0;
    char *p;
    int i;
    // This token logic divide the command by space
    // It will not useful when the space is in the quotation mark
    for (i = 0, p = strtok(command, " "); p && (i < MAX_LINE); p = strtok(NULL, " ")) {
        args[i] = (char *) (malloc(MAX_LINE * sizeof(char)));
        strcpy(args[i], p);
        count++;
        i++;
    }
    for (; i < MAX_LINE / 2 + 1; ++i) args[i] = NULL;
    return count;
}

void store_command(char *command, struct HistoryCommands *history) {
    if (history->count != HISTORY_SIZE) {
        history->commands[history->count] = (char *) malloc(MAX_LINE * sizeof(MAX_LINE));
        strcpy(history->commands[history->count++], command);
    }
    else {
        // Remove old history command to store the new one
        for (int i = 0; i < HISTORY_SIZE - 1; ++i)
            strcpy(history->commands[i], history->commands[i + 1]);
        strcpy(history->commands[HISTORY_SIZE - 1], command);
    }
    return;
}
