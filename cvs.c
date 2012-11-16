#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cvs_back.h"

const char command_names[][20] = {
    "add",
    "checkout",
    "commit",
    "delete",
    "diff",
    "format",
    "mv",
    "revert",
    "version",
};

int (*command_funcs[])(int argc, char **argv) = {
    cvs_add,
    cvs_checkout,
    cvs_commit,
    cvs_delete,
    cvs_diff,
    cvs_format,
    cvs_mv,
    cvs_revert,
    cvs_version,
};

#define NUM_COMMANDS (sizeof(command_funcs) / sizeof(command_funcs[0]))

void usage(void) {

    printf("Usage:\n\tcvs command [options]\nAvailable options:\n");

    for(int i = 0; i < NUM_COMMANDS; i++) {

        printf("\t%s\n", command_names[i]);
    }

    exit(1);
}

int main(int argc, char **argv) {

    if(argc < 2)
        usage();

    const char (*pos)[20] = bsearch(argv[1], command_names, NUM_COMMANDS,
                                    sizeof(command_names[0]), (__compar_fn_t)strcmp);

    if(!pos)
        usage();

    return command_funcs[pos - command_names](argc - 2, argv + 2);
}
