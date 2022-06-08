#include <stdlib.h>
#include <stdio.h>
#include "print.h"

void print_logo() {
    printf("\n");
    printf("\t _           _                          _\n");
    printf("\t| |__   __ _| |__  _   _  ___ __ _  ___| |__   ___\n");
    printf("\t| '_ \\ / _` | '_ \\| | | |/ __/ _` |/ __| '_ \\ / _ \\\n");
    printf("\t| |_) | (_| | |_) | |_| | (_| (_| | (__| | | |  __/\n");
    printf("\t|_.__/ \\__,_|_.__/ \\__, |\\___\\__,_|\\___|_| |_|\\___|\n");
    printf("\t                   |___/\n");
    printf("\n");
}

void print_awaiting_connections() {
    printf("\tListening on localhost:6969\n");
    printf("\n");
}

int exit_oom() {
    // Unable to allocate memory
    printf("Unable to allocate memory.");
    exit(137);  // Out of memory
}