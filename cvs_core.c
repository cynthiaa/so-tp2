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


#define MEMBER files
#include "element_ops.def"


#define MEMBER modifications
#include "element_ops.def"


int cvs_format(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    char base[MAX_PATH_LENGTH];

    expand_path(base, CVS_DIR);
    create_path("%s/", base);

    run_bash("rm -rf %s/*", base);
    run_bash("mkdir %s/info", base);

    write_server_file((struct info_file[]){{0, 0, 0, NULL, 0, NULL}});

    return 0;
}


int cvs_checkout(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    char base[MAX_PATH_LENGTH];

    if (find_file_in_parents(base, CVS_FILE)) {

        cvs_error(REPO_EXISTS_ERROR);
    }

    expand_path(base, CVS_DIR);

    struct info_file *info_file = read_server_file(-1);

    for (int i = 0; i < info_file->num_files; i++) {

        struct file *f = info_file->files + i;

        create_path(f->name);

        run_bash("cp %s/%d/%d \"%s\"", base, f->id, f->version, f->name);
    }

    run_bash("touch %s", CVS_FILE);

    info_file->num_modifications = 0;

    write_client_file(info_file);

    free_info_file(info_file);

    return 0;
}


static struct modification add_client_modification(int argc, char **argv, enum action action) {

    if (argc != 2 + (action == MOVE)) {

        cvs_help(2, (char*[]){"help", argv[0]});

        cvs_error("");
    }

    char info_path[MAX_PATH_LENGTH], file_path[MAX_PATH_LENGTH];

    if (!find_file_in_parents(info_path, CVS_FILE)) {

        cvs_error(REPO_MISSING_ERROR);
    }

    if (!file_exists("%s", argv[1])) {

        cvs_error(SRC_MISSING_ERROR, argv[1]);
    }

    expand_path(file_path, "%s", argv[1]);

    int offset = strlen(info_path) - strlen(CVS_FILE);

    struct modification mod;

    strcpy(mod.file.name, file_path + offset);

    mod.action = action;

    if (action == MOVE) {

        if (file_exists("%s", argv[2])) {

            cvs_error(DST_EXISTS_ERROR, argv[2]);
        }

        expand_path(file_path, "%s", argv[2]);

        strcpy(mod.new_name, file_path + offset);
    }

    return mod;
}


int cvs_add(int argc, char **argv) {

    struct modification mod = add_client_modification(argc, argv, ALTER);

    struct info_file *info_file = read_client_file();

    if (!find_file(info_file, &mod.file)) {

        mod.action = NEW;

        add_modifications(info_file, &mod);
        add_files        (info_file, &mod.file);
        write_client_file(info_file);
        free_info_file   (info_file);

        return 0;
    }

    if (!find_modification(info_file, &mod)) {

        add_modifications(info_file, &mod);
        write_client_file(info_file);
        free_info_file   (info_file);

        return 0;
    }

    free_info_file(info_file);

    return 0;
}


int cvs_delete(int argc, char **argv) {

    struct modification mod = add_client_modification(argc, argv, DELETE);

    struct info_file *info_file = read_client_file();

    struct file *file = find_file(info_file, &mod.file);

    if (!file) {

        cvs_error(SRC_MISSING_ERROR, argv[1]);
    }

    /* run_bash("rm %s", argv[1]); */

    remove_files     (info_file, file - info_file->files);
    add_modifications(info_file, &mod);
    write_client_file(info_file);
    free_info_file   (info_file);

    return 0;
}


int cvs_mv(int argc, char **argv) {

    struct modification mod = add_client_modification(argc, argv, MOVE);

    struct info_file *info_file = read_client_file();

    struct file *file = find_file(info_file, &mod.file);

    if (!file) {

        cvs_error(SRC_MISSING_ERROR, argv[1]);
    }

    struct file new_file;

    strcpy(new_file.name, mod.new_name);

    if (find_file(info_file, &new_file)) {

        cvs_error(DST_EXISTS_ERROR, argv[2]);
    }

    /* run_bash("mv %s %s", argv[1], argv[2]); */

    remove_files     (info_file, file - info_file->files);
    add_files        (info_file, &new_file);
    add_modifications(info_file, &mod);
    write_client_file(info_file);
    free_info_file   (info_file);

    return 0;
}


int cvs_commit(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_update(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_version(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_diff(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_revert(int argc, char **argv) {

    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int cvs_help(int argc, char **argv) {

    if (argc == 2) {

        struct command *cmd = find_command(argv[1]);

        if (cmd) {

            printf("%s", cmd->help);

            return 0;
        }
    }

    printf("%s", find_command("help")->help);

    return 1;
}

