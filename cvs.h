#ifndef _CVS_H_
#define _CVS_H_

#include "cvs_core.h"


/**
 * Información de todos los comandos disponibles
 *
 * @warning tienen que estar ordenados alfabéticamente
 */
struct command {

    char *name;
    int (*handler)(int argc, char **argv);
    char *help;

} commands[] = {

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

#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))


/**
 * Función de comparación de `commands` para usar con bsearch, etc
 */
int cmdcmp(const void *, const void *);


#endif /* _CVS_H_ */

