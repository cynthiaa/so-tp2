#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cvs.h"
#include "cvs_helpers.h"


int cmdcmp(const void *cmd1, const void *cmd2) {

    return strcmp(((struct command*)cmd1)->name, ((struct command*)cmd2)->name);
}


static void usage(void) {

    printf("Usage:\n\tcvs command [options]\nAvailable options:\n");

    for (int i = 0; i < NUM_COMMANDS; i++) {

        printf("\t%s\n", commands[i].name);
    }

    cvs_error(NULL);
}


int main(int argc, char **argv) {

    if (argc < 2)
        usage();

    struct command *cmd = bsearch(argv[1], commands, NUM_COMMANDS, sizeof(struct command), cmdcmp);

    if (!cmd)
        usage();

    return cmd->handler(argc - 2, argv + 2);
}

