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
    // Calling the shell
    // If there is an argument, run in batch mode
    if (argc == 2) {
        printf("Batch mode not implemented yet\n");
        // csh_batch_mode(argv[1]);
        return EXIT_FAILURE;
    } else {
        // Signal Handling
        signal(SIGINT, SIG_IGN); // Ignore Ctrl-C
        signal(SIGTSTP, SIG_IGN); // Ignore Ctrl-Z

        // Initialize Shell
        init_shell();

        csh_interactive_loop();
    }

    return EXIT_SUCCESS;
}