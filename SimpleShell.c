//
// Created by irmo on 15/9/21.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define MAX_LINE 80
#define HISTORY_SIZE 10

struct HistoryCommands {
    int count;
    char *commands[HISTORY_SIZE];
};

int main(void) {
    struct HistoryCommands history;
    char *args[MAX_LINE / 2 + 1];
    int should_run = 1;
    history.count = 0;
    printf("Welcome to irmo's simple shell!\n");
    printf("The pid of this shell is %d.\n", (int)getpid());

    while (should_run) {
        char *command = (char *) (malloc(MAX_LINE * sizeof(char)));
        printf("osh > ");
        fgets(command, MAX_LINE, stdin);
        command[strlen(command) - 1] = '\0';

        // Divide the command to args
        char *p;
        int i;
        for (i = 0, p = strtok(command, " "); p && (i < MAX_LINE); p = strtok(NULL, " ")) {
            args[i] = p;
            i++;
        }

        // Check the legality of the command
        struct stat *buf = (struct stat *) malloc(sizeof(struct stat *));
        if (stat(args[0], buf) == 0) {
            printf("%s : Command not found.\n", args[0]);
        }

        // Create a new process to execute the command
        for (; i < MAX_LINE; ++i) args[i] = NULL;
        pid_t new_pid = fork();
        if (new_pid == 0) {
            execvp(args[0], args);
            if (strcmp(args[0], "history") != 0) {
                printf("%s : Command not found.\n", args[0]);
            }
            else {
                // Complement history command
                for (i = history.count; i > 0; i--)
                    printf("%d %s\n", i, history.commands[i - 1]);
            }
        }
        waitpid(new_pid, NULL, 0);

        // Store this command to history
        if (history.count != HISTORY_SIZE) {
            history.commands[history.count] = (char *) malloc(MAX_LINE * sizeof(MAX_LINE));
            strcpy(history.commands[history.count++], command);
        }
        else {
            for (int i = 0; i < HISTORY_SIZE - 1; ++i)
                strcpy(history.commands[i], history.commands[i + 1]);
            strcpy(history.commands[HISTORY_SIZE - 1], command);
        }

        // Exit the shell
        if (strcmp(command, "exit") == 0) break;
    }
    return 0;
}