#ifndef _CVS_ERRORS_H_
#define _CVS_ERRORS_H_


#define CORRUPT_REPO_ERROR  "The repository is corrupted. Aborting.\n"
#define WRITE_ERROR         "There was an error while writing. Aboring.\n"
#define MEMORY_ERROR        "Not enough memory. Aborting.\n"
#define COMMAND_ERROR       "There was a bash error. Aborting.\n"
#define GENERIC_ERROR       "There was an error. Aborting.\n"
#define REPO_MISSING_ERROR  "You have to be in a repository to run this command.\n"
#define REPO_EXISTS_ERROR   "You already are in a repository.\n"
#define INVALID_PATH_ERROR  "Invalid path: `%s`\n"
#define SRC_MISSING_ERROR   "Source path does not exist: `%s`\n"
#define SRC_CONTROL_ERROR   "Source path is not under version control: `%s`\n"
#define DST_EXISTS_ERROR    "Destination path already exists: `%s`\n"
#define DST_NOT_DIR         "Destination must be an existing directory when having multiple sources.\n"
#define MOVE_INSIDE_ERROR   "Destination inside source: rename `%s` to `%s`\n"
#define CLIENT_BEHIND_ERROR "Your repository is behind the server by %d commits. Update before commiting.\n"
#define IDENTICAL_ERROR     "The source is identical to the destination: `%s`\n"
#define TREE_CONFLICT_ERROR "Tree conflict. Aborting. If I were you I would do a clean checkout. Sorry.\n"


#endif /* _CVS_ERRORS_H_ */

