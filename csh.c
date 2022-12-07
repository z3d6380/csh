// File: csh.c
// Author: Luis Moraguez
// Date: 2022-12-05
// Version: 0.1.0
// Description: Main driver code for csh shell loop

#include "csh.h"

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

// NAME: init_shell
// INPUT: void
// OUTPUT: void
// DESCRIPTION: Displays shell initialization message
void init_shell(void) {
    clear();
    char* username = getenv("USER");
    printf(" ██████╗███████╗██╗  ██╗" RED "    ██╗  \n" reset);
    printf("██╔════╝██╔════╝██║  ██║" RED "    ╚██╗ \n" reset);
    printf("██║     ███████╗███████║" RED "     ╚██╗\n" reset);
    printf("██║     ╚════██║██╔══██║" RED "     ██╔╝\n" reset);
    printf("╚██████╗███████║██║  ██║" RED "    ██╔╝ \n" reset);
    printf(" ╚═════╝╚══════╝╚═╝  ╚═╝" RED "    ╚═╝  \n" reset);
    printf("Welcome to csh, " GRN "%s!\n" reset, username);
    printf("Type " BYEL "'help'" reset " for built-in command list\nType " BYEL "'exit'" reset " to exit the shell\n");
    sleep(3);
    clear();
}

// NAME: prompt1
// INPUT: void
// OUTPUT: char*
// DESCRIPTION: Displays the PS1 prompt
char* prompt1(void) {
    char* cwd = getcwd(NULL, 0);
    char* prompt;
    asprintf(&prompt, RED "┌[" CYN "%d" RED "]─[" YEL "%s" RED "]─[" MAG "%s" RED "]\n└╼" GRN "%s" YEL "$ " reset, (int)getpid(), "csh", cwd, getenv("USER"));
    free(cwd);
    return prompt;
}

// NAME: prompt2
// INPUT: void
// OUTPUT: char*
// DESCRIPTION: Displays the PS2 prompt
char* prompt2(void) {
    return "> ";
}

// NAME: print_help
// INPUT: void
// OUTPUT: void
// DESCRIPTION: Displays the help message
void print_help(void) {
    printf(BWHT "Built-in commands:\n" reset);
    printf("help\t\t\t\tDisplays this help message\n");
    printf("about\t\t\t\tDisplays information about csh\n");
    printf("exit\t\t\t\tExits the shell\n");
    printf("cd\t\t\t\tChanges the current working directory\n");
    printf("history\t\t\t\tDisplays the command history\n");
    printf("loop # {command to execute}\tLoops a command # of times\n");
    printf("path {dir_1} {dir_n}\t\tSets the path to the specified directories\n");
    printf("export {varname} {value}\tSets the env variable to the specified value\n");
    printf("clear\t\t\t\tClears the screen\n");
}

// NAME: print_about
// INPUT: void
// OUTPUT: void
// DESCRIPTION: Displays the about message
void print_about(void) {
    printf(BWHT "ABOUT:" reset "\tcsh is a simple shell written in C.\n");
    printf("\tProject for OS course at Florida Polytechnic University\n");
    printf(BWHT "AUTHOR:" reset "\tLuis Moraguez\n");
    printf(BWHT "VER:" reset "\t0.1.0\n");
    printf(BWHT "DATE:" reset "\t2022-12-05\n");
}

// NAME: csh_batch_mode
// INPUT: char*
// OUTPUT: void
// DESCRIPTION: Runs the shell in batch mode, reading commands from a file
void csh_batch_mode(char* filename) {
    // Variables
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp;
    char *command;
    char *args[256] = {NULL};

    // Open file
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("csh: %s: No such file or directory\n", filename);
        exit(1);
    }

    // Read file line by line
    while ((read = getline(&line, &len, fp)) != -1) {
        // Remove newline character and return character
        line[strcspn(line, "\r\n")] = 0;

        // Tokenize input to command and arguments
        if (strcmp(line, "") == 0) {
            continue; // If no input, continue
        } else {
            command = strtok(line, " ");
        }

        // Check for built-in commands
        if (!builtin_executer(command, args)) {
            // Store command and arguments
            store_args(command, args);
            // Parse command and arguments to execute
            parse_args(args);
        }
    }

    // Close file
    fclose(fp);

}

// NAME: csh_interactive_input
// INPUT: void
// OUTPUT: char*
// DESCRIPTION: Gets the user input and stores it in the buffer
char* csh_interactive_input() {
    char* buf;

    // Get input from readline
    buf = readline(prompt1());
    if (buf != NULL && strlen(buf) != 0) {
        // Add input to history
        add_history(buf);
    }
    return buf;
}

// NAME: csh_interactive_loop
// INPUT: void
// OUTPUT: void
// DESCRIPTION: Runs the shell in interactive mode
void csh_interactive_loop(void) {
    // Variables
    char *input;
    char *command;
    char *args[256] = {NULL};

    // Main Loop
    while (1) {
        // Get Input
        input = csh_interactive_input();

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

        // Check for built-in commands
        if (!builtin_executer(command, args)) {
            // Store command and arguments
            store_args(command, args);
            // Parse command and arguments to execute
            parse_args(args);
        }
    }
}

// NAME: csh_interactive_history
// INPUT: void
// OUTPUT: void
// DESCRIPTION: Displays the command history
void csh_interactive_history(void) {
    HIST_ENTRY **history;
    int i;

    history = history_list();
    if (history) {
        for (i = 0; history[i]; i++) {
            printf("%d %s\n", i + history_base, history[i]->line);
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

// NAME: print_args
// INPUT: char**
// OUTPUT: void
// DESCRIPTION: Prints the arguments in an array
void print_args(char *args[]) {
    int i = 0;
    while (args[i] != NULL) {
        printf("%s ", args[i]);
        i++;
    }
    printf("\n");
}

// NAME: restore_signals
// INPUT: void
// OUTPUT: void
// DESCRIPTION: Restores the default signal handlers
void restore_signals(void) {
    signal(SIGINT, SIG_DFL); // Restore Ctrl-C
    signal(SIGTSTP, SIG_DFL); // Restore Ctrl-Z
}

// NAME: builtin_executer
// INPUT: char*
// OUTPUT: void
// DESCRIPTION: Executes a built-in command
int builtin_executer(char* command, char** args) {

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

        // Print help if command is help
        // 0 arguments: print help
        // 1+ arguments: error, print "too many arguments"
        else if (strcmp(command, "help") == 0) {
            store_args(command, args);
            if (count_args(args) - 1 == 0) {
                print_help();
            } else {
                printf("csh: help: too many arguments\n");
            }
        }

        // Print about if command is about
        // 0 arguments: print about
        // 1+ arguments: error, print "too many arguments"
        else if (strcmp(command, "about") == 0) {
            store_args(command, args);
            if (count_args(args) - 1 == 0) {
                print_about();
            } else {
                printf("csh: about: too many arguments\n");
            }
        }

        // Print history if command is history
        // 0 arguments: print history
        // 1+ arguments: error, print "too many arguments"
        else if (strcmp(command, "history") == 0) {
            store_args(command, args);
            if (count_args(args) - 1 == 0) {
                csh_interactive_history();   
            } else {
                printf("csh: history: too many arguments\n");
            }
        }

        // Set PATH environment variable if command is path
        // 0 arguments: Sets PATH to empty string
        // Space separated list of directories stored in PATH env variable as concatenated with string with ':' as delimiter between directories (e.g. /bin:/usr/bin)
        else if (strcmp(command, "path") == 0) {
            store_args(command, args);
            if (count_args(args) - 1 == 0) {
                setenv("PATH", "", 1);
            }
            else {
                // Set PATH to first argument
                if (access(args[1], F_OK) == -1) {
                    printf("csh: path: invalid directory, %s\n", args[1]);
                    // Set PATH to empty string
                    setenv("PATH", "", 1);
                } else {
                    // Set PATH to first argument
                    setenv("PATH", args[1], 1);
                }
                // Concatenate the rest of the arguments
                for (int i = 2; i < count_args(args); i++) {
                    // Check if valid directory
                    if (access(args[i], F_OK) == -1) {
                        printf("csh: path: invalid directory, %s\n", args[i]);
                        continue;
                    }
                    // Check if PATH is empty
                    if (strcmp(getenv("PATH"), "") != 0) {
                        // Add delimiter if not empty
                        strcat(getenv("PATH"), ":");
                    }
                    // Add directory to PATH
                    strcat(getenv("PATH"), args[i]);
                }
            }
        }

        // Set environment variable if command is export
        // 0 arguments: error, print "too few arguments"
        // 1 argument: error, print "too few arguments"
        // 2 arguments: set environment variable to second argument, eg. export PATH /bin:/usr/bin
        // 3+ arguments: error, print "too many arguments"
        else if (strcmp(command, "export") == 0) {
            store_args(command, args);
            if (count_args(args) - 1 == 0) {
                printf("csh: export: too few arguments\n");
            }
            else if (count_args(args) - 1 == 1) {
                printf("csh: export: too few arguments\n");
            }
            else if (count_args(args) - 1 == 2) {
                setenv(args[1], args[2], 1);
            }
            else {
                printf("csh: export: too many arguments\n");
            }
        }
        else {
            return 0; // Return 0 if not built-in command
        }
    return 1; // Return 1 if built-in command executed
}

// NAME: simple_executer
// INPUT: char*, char**
// OUTPUT: void
// DESCRIPTION: Executes a simple command (non built-in)
void simple_executer(char* command, char *args[]) {
    // Check if PATH is set or not empty
    if (getenv("PATH") != NULL && strcmp(getenv("PATH"), "") != 0) {
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

// NAME: redirected_executer
// INPUT: char*, char**, int
// OUTPUT: void
// DESCRIPTION: Executes a redirected command (non built-in)
void redirected_executer(char* command, char* args[], char* file_name, int append_bit) {
    freopen(file_name, append_bit ? "a+" : "w+", stdout);
    simple_executer(command, args);
    freopen("/dev/tty", "w", stdout);
}

// NAME: parse_args
// INPUT: char**
// OUTPUT: void
// DESCRIPTION: Parses the args and determines the best executer to run
void parse_args(char *args[]) {
    int total_args = count_args(args); // For use with parser loop 'i < total_args'
    enum {SIMPLE, LOOPED, REDIRECTED} exec_type;
    exec_type = SIMPLE; // Default exec type initialized
    int i = 0;
    int append_bit = 0;
    char* file_name;
    int redirect_index = 0;
    char** redirected_args;

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
            redirect_index = i;
            redirected_args = copy_args(redirect_index, args);
            file_name = args[i+1];
            break;
        }
        else if (strcmp(args[i], ">>") == 0) {
            exec_type = REDIRECTED;
            redirect_index = i;
            redirected_args = copy_args(redirect_index, args);
            file_name = args[i+1];
            append_bit = 1;
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
            redirected_executer(redirected_args[0], redirected_args, file_name, append_bit);
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