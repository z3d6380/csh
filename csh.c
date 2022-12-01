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

// NAME: looped_executer
// INPUT: char*, int, char**
// OUTPUT: void
// DESCRIPTION: Executes a looped command (non built-in)
void looped_executer (char* command, int num, char *args[]) {
    char** args2;
    for (int i = 1; i <= num; i++) {
        args2 = copy_args(count_args(args), args);
        parse_args(args2);
        if (i+1 <= num) {
            char it[sizeof(int)];
            sprintf(it, "%d", i+1);
            setenv("LOOP", it, 1);
        }
    }
}

// NAME: parse_args
// INPUT: char**
// OUTPUT: void
// DESCRIPTION: Parses the args and determines the best executer to run
void parse_args(char *args[]) {
    int total_args = count_args(args); // For use with parser loop 'i < total_args'
    enum {SIMPLE, LOOPED, REDIRECTED, PIPED} exec_type;
    exec_type = SIMPLE; // Default exec type initialized
    int i = 0;

    while (i < total_args) {
        // Check if the argument is a call to shell/env variable
        if (i >= 1 && args[i][0] == '$') {
                // Skip args[i][0] since it is '$' and get rest of the string
                args[i] = getenv(args[i] + 1);
        }
        // Check if the argument is one of the exec types
        if (strcmp(args[i], "loop") == 0) {
            setenv("LOOP", "1", 1);
            exec_type = LOOPED;
            break;
        } else if (strcmp(args[i], ">") == 0) {
            exec_type = REDIRECTED;
            break;
        } else if (strcmp(args[i], "|") == 0) {
            exec_type = PIPED;
            break;
        }
        i++;
    }

    switch (exec_type) {
        case SIMPLE:
            simple_executer(args[0], args);
            break;
        case LOOPED:
            // Check if number of arguments is 3 or more
            if (count_args(args) < 3) {
                printf("csh: loop: too few arguments\n");
            }
            else {
                // Check if second argument is a number
                int i;
                for (i = 0; i < strlen(args[1]); i++) {
                    if (!isdigit(args[1][i])) {
                        printf("csh: loop: second argument must be a number\n");
                        break;
                    }
                }
                // If second argument is a number, loop command
                if (i == strlen(args[1])) {
                    int num = atoi(args[1]);
                    looped_executer(args[2], num, &args[2]);
                }
            }
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

// NAME: copy_args
// INPUT: int, char**
// OUTPUT: char**
// DESCRIPTION: Copies the arguments from one array to another
char **copy_args(int argc, char *args[]) {
  // calculate the contiguous args buffer size
  int length = 0;
  size_t ptr_args = argc + 1;
  for (int i = 0; i < argc; i++)
  {
    length += (strlen(args[i]) + 1);
  }
  char** new_args = (char**)malloc((ptr_args) * sizeof(char*) + length);
  // copy args into the contiguous buffer
  length = 0;
  for (int i = 0; i < argc; i++)
  {
    new_args[i] = &(((char*)new_args)[(ptr_args * sizeof(char*)) + length]);
    strcpy(new_args[i], args[i]);
    length += (strlen(args[i]) + 1);
  }
  // insert NULL terminating ptr at the end of the ptr array
  new_args[ptr_args-1] = NULL;
  return (new_args);
}