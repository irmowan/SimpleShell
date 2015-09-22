//
// Created by irmo on 15/9/21.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define MAX_LINE 80
#define HISTORY_SIZE 6

int main(void) {
    char history[HISTORY_SIZE];
    char *args[MAX_LINE / 2 + 1];
    int should_run = 1;
    printf("Welcome to irmo's simple shell!\n");
    while (should_run) {
        char *command = (char *) (malloc(MAX_LINE * sizeof(char)));
        printf("osh > ");
        fgets(command, MAX_LINE, stdin);
        command[strlen(command) - 1] = '\0';
        char *p;
        int i;
        for (i = 0, p = strtok(command, " "); p && (i < MAX_LINE); p = strtok(NULL, " ")) {
            args[i] = p;
            i++;
        }
        for (; i < MAX_LINE; ++i) args[i] = NULL;
        struct stat *buf = (struct stat *) malloc(sizeof(struct stat *));
//        if (stat(args[0], buf) == -1) {
//            printf("%s : Command not found.\n", args[0]);
//        }
//        else {
            pid_t new_pid = fork();
            if (new_pid == 0) {
                execvp(args[0], args);
                printf("%s : Command not found.\n", args[0]);
            }
            waitpid(new_pid, NULL, 0);
//        }

        if (strcmp(command, "exit") == 0) break;
    }
    return 0;
}