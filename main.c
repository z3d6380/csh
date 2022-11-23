// File: main.c
// Author: Luis Moraguez
// Date: 2022-11-22
// Version: 0.1.0
// Description: Main driver code for csh shell

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "csh.h"

int main(int argc, char *argv[]) {
    // Signal Handling
    signal(SIGINT, SIG_IGN); // Ignore Ctrl-C
    signal(SIGTSTP, SIG_IGN); // Ignore Ctrl-Z

    // Calling the shell loop
    csh_loop();

    return EXIT_SUCCESS;
}