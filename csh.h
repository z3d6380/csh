// File: csh.h
// Author: Luis Moraguez
// Date: 2022-11-22
// Version: 0.1.0
// Description: csh header file

#ifndef CSH_H
#define CSH_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "colors.h"

// Function prototypes
void init_shell(void);
char* prompt1(void);
char* prompt2(void);
void print_help(void);
void print_about(void);
char* csh_interactive_input(void);
void csh_interactive_loop(void);
void csh_interactive_history(void);
void csh_batch_mode(char*);
void restore_signals(void);
int count_args(char**);
void store_args(char*, char**);
void parse_args(char**);
char** copy_args(int, char**);

// Executer Prototypes
void simple_executer(char*, char**);
void looped_executer(char*, int, char**);
// redirected_executer
// void piped_executer(char*, char**, int);

#endif // CSH_H