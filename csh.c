// File: csh.c
// Author:
// Date: 2022-11-22
// Version: 0.1.0
// Description: Main driver code for csh shell loop

#include "csh.h"

// NAME: csh_loop
// INPUT: void
// OUTPUT: void
// DESCRIPTION: Main LOOP for the csh shell
void csh_loop(void) {
    // Variables
    char *prompt = "csh> ";
    char *input;
    char *command;
    char *args[256] = {NULL};

    // Main Loop
    while (1) {
        // Get Input
        input = readline(prompt);

        // Check for EOF (Ctrl-D)
        if (input == NULL) {
            printf("\n");
            break;
        }

        // Tokenize input to command and arguments
        if (strcmp(input, "") == 0) {
            continue; // If no input, continue
        } else {
            command = strtok(input, " ");
        }

        // Exit if command is exit
        // 0 arguments: exit(0)
        // 1+ arguments: error, print "too many arguments"
        if (strcmp(command, "exit") == 0) {
            store_args(command, args);
            if (count_args(args) - 1 == 0) {
                exit(0);
            } else {
                printf("csh: exit: too many arguments\n");
            }
        }

        // Change directory if command is cd
        // 0 arguments: error, print "too few arguments"
        // 1 argument: change directory to argument
        // 2+ arguments: error, print "too many arguments"
        else if (strcmp(command, "cd") == 0) {
            store_args(command, args);
            if (count_args(args) - 1 == 0) {
                printf("csh: cd: too few arguments\n");
            }
            else if (count_args(args) - 1 == 1) {
                // Check if directory exists, and if it does
                if (chdir(args[1]) == -1) {
                    // Failed, not directory
                    printf("csh: cd: invalid directory\n");
                }
            }
            else {
                printf("csh: cd: too many arguments\n");
            }
        }

        // Run parser so that correct executer is chosen
        else {
            store_args(command, args);
            parse_args(args);
        }

    }
}

// NAME: count_args
// INPUT: char *args[]
// OUTPUT: int
// DESCRIPTION: Counts the number of arguments in the array
int count_args(char *args[]) {
    int i = 0;
    while (args[i] != NULL) {
        i++;
    }
    return i;
}

// NAME: store_args
// INPUT: char*, char**
// OUTPUT: void
// DESCRIPTION: Stores the arguments in an array
void store_args(char* command, char *args[]) {
    int i = 1;
    args[0] = command;
    while (1) {
        args[i] = strtok(NULL, " ");
        if (args[i] == NULL) {
            break;
        }
        i++;
    }
}

// NAME: restore_signals
// INPUT: void
// OUTPUT: void
// DESCRIPTION: Restores the default signal handlers
void restore_signals(void) {
    signal(SIGINT, SIG_DFL); // Restore Ctrl-C
    signal(SIGTSTP, SIG_DFL); // Restore Ctrl-Z
}

// NAME: simple_executer
// INPUT: char*, char**
// OUTPUT: void
// DESCRIPTION: Executes a simple command (non built-in)
void simple_executer(char* command, char *args[]) {
    // Create Fork
        pid_t pid = fork();
        // Check if child
        if (pid == 0) {
            // Restore Signals
            restore_signals();
            // Execute command
            execvp(command, args);
        }
        // Wait for child process to finish
        else {
            wait(NULL);
        }
}

// NAME: parse_args
// INPUT: char**
// OUTPUT: void
// DESCRIPTION: Parses the args and determines the best executer to run
void parse_args(char *args[]) {
    char* commands[256];
    char* args2[256][2]; // 2-dimensional array that maps arguments to their respective commands
                        // args2 used for looped_executer and piped_executer
    int total_args = count_args(args); // For use with parser loop 'i < total_args'
    int total_commands = 0; // Keeps track of the number of commands; Looped and Piped Executers will likely need
    enum {SIMPLE, LOOPED, REDIRECTED, PIPED} exec_type;
    exec_type = SIMPLE; // Default exec type initialized
    int i = 0;
    // int j = 0; // Used for more complex parsing

    // Store the first command in the commands array since args[0] is always a command
    commands[0] = args[0];
    total_commands++;

    while (i < total_args) {
        if (strcmp(args[i], "loop") == 0) {
            exec_type = LOOPED;
            continue;
        } else if (strcmp(args[i], ">") == 0) {
            exec_type = REDIRECTED;
            continue;
        } else if (strcmp(args[i], "|") == 0) {
            exec_type = PIPED;
            continue;
        }
        i++;
    }

    switch (exec_type) {
        case SIMPLE:
            simple_executer(commands[0], args);
            break;
        case LOOPED:
            break;
        case REDIRECTED:
            break;
        case PIPED:
            break;
        default:
            printf("csh: parse: invalid command(s)");
            break;
    }

}