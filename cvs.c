#include <stdio.h>
#include <string.h>
#include "cvs.h"
#include "cvs_helpers.h"


const struct command commands[] = {

    {"add",      cvs_add,      "Usage:\n\tgit add file\n"
                               "Sets a file for adding in the next commit\n"},
    {"checkout", cvs_checkout, "Usage:\n\tgit checkout\n"
                               "Creates a local copy of the last version of the repository\n"},
    {"commit",   cvs_commit,   "Usage:\n\tgit commit\n"
                               "Commits the local changes made to the repository\n"},
    {"delete",   cvs_delete,   "Usage:\n\tgit delete file\n"
                               "Sets a file for deletion in the next commit\n"},
    {"diff",     cvs_diff,     "Usage:\n\tgit diff file version\n"
                               "Prints the diff between a file and itself in a previous version\n"},
    {"format",   cvs_format,   "Usage:\n\tgit format\n"
                               "Formats the server\n"},
    {"help",     cvs_help,     "Usage:\n\tgit help [command]\n"
                               "Show command help\n"},
    {"mv",       cvs_mv,       "Usage:\n\tgit mv old_path new_path\n"
                               "Moves a path to a new path\n"},
    {"revert",   cvs_revert,   "Usage:\n\tgit revert file version\n"
                               "Reverts a file to a previous version\n"},
    {"version",  cvs_version,  "Usage:\n\tgit version file\n"
                               "Shows a list of the previous versions of the file\n"},
};


const size_t num_commands = (sizeof(commands) / sizeof(commands[0]));


int command_cmp(const void *name, const void *cmd) {

    return strcmp(name, ((struct command*)cmd)->name);
}


static void usage(void) {

    printf("Usage:\n\tcvs command [options]\n\nAvailable commands:\n");

    for (size_t i = 0; i < num_commands; i++) {

        printf("\t%s\n", commands[i].name);
    }

    cvs_error(NULL);
}


int main(int argc, char **argv) {

    if (argc < 2)
        usage();

    struct command *cmd = find_command(argv[1]);

    if (!cmd)
        usage();

    return cmd->handler(argc - 1, argv + 1);
}

