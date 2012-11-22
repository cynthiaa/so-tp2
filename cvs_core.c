#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "cvs_core.h"
#include "cvs_helpers.h"
#include "cvs_files.h"
#include "cvs_errors.h"
#include "cvs.h"


#define UNUSED(x) ((void)(x))


int cvs_format(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    char base[MAX_PATH_LENGTH];

    expand_path(base, CVS_DIR);
    create_path("%s/", base);

    run_bash("rm -rf %s/*", base);
    run_bash("mkdir %s/info", base);

    write_server_file((struct server_file[]){{0, 0, 0, NULL, 0, NULL}});

    return 0;
}


int cvs_checkout(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    char base[MAX_PATH_LENGTH];

    if (find_file_in_parents(base, CVS_FILE)) {

        cvs_error(ALREADY_REPO_ERROR);
    }

    expand_path(base, CVS_DIR);

    struct server_file *server_file = read_server_file(-1);

    for (int i = 0; i < server_file->num_files; i++) {

        struct file *f = server_file->files + i;

        create_path(f->name);

        run_bash("cp %s/%d/%d \"%s\"", base, f->id, f->version, f->name);
    }

    run_bash("touch %s", CVS_FILE);

    write_client_file((struct client_file[]){{server_file->version, 0, NULL}});

    free_server_file(server_file);

    return 0;
}


#define STRUCT client_file
#define MEMBER modification
#include "append.def"


#define STRUCT server_file
#define MEMBER files
#include "append.def"


#define STRUCT server_file
#define MEMBER modification
#include "append.def"


int cvs_add(int argc, char **argv) {

    if (argc != 1) {

        cvs_help(1, (char*[]){"add"});
        return 1;
    }

    char info_path[MAX_PATH_LENGTH], file_path[MAX_PATH_LENGTH];

    if (!find_file_in_parents(info_path, ".cvs_info")) {

        cvs_error(NO_REPO_ERROR);
    }

    expand_path(file_path, argv[0]);

    int offset = strlen(info_path) - strlen(".cvs_info");

    memmove(file_path + offset, file_path, MAX_PATH_LENGTH - offset);

    /* rewrite .cvs_info */

    struct client_file *cf = read_client_file();

    struct client_modification *mod = append_client_file_modification(cf);

    strcpy(mod->name, file_path);

    mod->action = ADD;

    write_client_file(cf);

    return 0;
}


int cvs_delete(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_update(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_commit(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_diff(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_version(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_revert(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_mv(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_help(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}

