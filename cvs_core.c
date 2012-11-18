#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "cvs_core.h"
#include "cvs_helpers.h"
#include "cvs_errors.h"
#include "cvs.h"


#define UNUSED(x) ((void)(x))


int cvs_format(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    create_path(CVS_DIR);
    run_bash("chmod a+w %s", CVS_DIR);
    run_bash("rm -rf %s", CVS_DIR);
    run_bash("mkdir %sinfo", CVS_DIR);

    FILE *file = open_file("w", "%sinfo/0", CVS_DIR);

    write_server_file(file, (struct server_file[]){{0, 0, 0, NULL, 0, NULL}});

    fclose(file);

    run_bash("ln %sinfo/0 %sinfo/current", CVS_DIR, CVS_DIR);

    return 0;
}


int cvs_checkout(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    FILE *file = open_file("r", "%sinfo/current", CVS_DIR);

    struct server_file *server_file = read_server_file(file);

    fclose(file);

    for (int i = 0; i < server_file->num_files; i++) {

        struct file *f = server_file->files + i;

        create_path(f->name);

        run_bash("cp %s%d/%d %s", CVS_DIR, f->id, f->version, f->name);
    }

    file = open_file("w", ".cvs_info");

    write_client_file(file, (struct client_file[]){{server_file->version, 0, NULL}});

    fclose(file);

    free_server_file(server_file);

    return 0;
}


int cvs_add(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);


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

