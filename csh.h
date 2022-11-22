// File: csh.h
// Author:
// Date: 2022-11-22
// Version: 0.1.0
// Description: csh header file

#ifndef CSH_H
#define CSH_H

#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// Function prototypes
void csh_loop(void);
void restore_signals(void);
int count_args(char**);
void store_args(char*, char**);
void parse_args(char**);
// Executer Prototypes
void simple_executer(char*, char**);
// looped_executer
// redirected_executer
// piped_executer


#endif // CSH_H