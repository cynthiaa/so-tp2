#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <dirent.h>
#include "cvs_core.h"
#include "cvs_helpers.h"
#include "cvs_files.h"
#include "cvs_errors.h"
#include "cvs.h"


#define MEMBER files
#include "element_ops.def"


#define MEMBER modifications
#include "element_ops.def"


#include "cvs_core_helpers.h"


static void check_num_args(int num_args, int argc, char **argv) {

    if (argc != num_args) {
        cvs_help(2, (char*[]){"help", argv[0]});
        cvs_error("");
    }
}


int cvs_format(int argc, char **argv) {

    check_num_args(1, argc, argv);

    create_path("%s/", base_path());

    run_bash("rm -rf %s/*", base_path());

    run_bash("mkdir %s/info", base_path());

    write_server_file((struct info_file[]){{0, 0, 0, NULL, 0, NULL}});

    return 0;
}


int cvs_checkout(int argc, char **argv) {

    check_num_args(1, argc, argv);

    if (repo_path()) {

        cvs_error(REPO_EXISTS_ERROR);
    }

    struct info_file *info_file = read_server_file(-1);

    for (int i = 0; i < info_file->num_files; i++) {

        struct file *f = info_file->files + i;

        create_path(f->name);

        run_bash("cp %s/%d/%d \"%s\"", base_path(), f->id, f->version, f->name);
    }

    run_bash("touch %s", CVS_FILE);

    info_file->num_modifications = 0;

    write_client_file(info_file);

    free_info_file(info_file);

    return 0;
}


struct cmd_tmp {

    char (*argv)[MAX_PATH_LENGTH];

    int path_offset;
};


static struct cmd_tmp expand_arguments(int argc, char **argv) {

    if (argc == 1) {

        cvs_help(2, (char*[]){"help", argv[0]});
        cvs_error("");
    }

    struct cmd_tmp ret;

    ret.argv = cvs_malloc(argc * MAX_PATH_LENGTH);

    if (!repo_path()) {

        cvs_error(REPO_MISSING_ERROR);
    }

    strcpy(ret.argv[0], repo_path());

    ret.path_offset = strlen(ret.argv[0]) + 1;

    strcpy(ret.argv[0] + ret.path_offset - 1, "/");

    for (int pos = 1; pos < argc; pos++) {

        expand_path(ret.argv[pos], "%s", argv[pos]);

        memmove(ret.argv[pos], ret.argv[pos] + ret.path_offset, MAX_PATH_LENGTH - ret.path_offset);

        if (!ret.argv[pos][0]) {

            strcpy(ret.argv[pos], ".");
        }
    }

    return ret;
}


static void cvs_add_file(struct info_file* info_file, char *path) {

    struct modification mod;
    mod.action = 0;
    strcpy(mod.file.name, path);

    if (!find_files(info_file, path)) {

        mod.action = NEW;
        add_files(info_file, &mod.file);

        struct modification *pos = find_last_modifications(info_file, &mod);

        if (pos && pos->action == DELETE) {

            mod.action = ALTER;
            remove_modifications(info_file, info_file->modifications - pos);
        }

        add_modifications(info_file, &mod);

        return;
    }

    if (!find_modifications(info_file, &mod)) {

        mod.action = ALTER;
        add_modifications(info_file, &mod);

        return;
    }
}


static void cvs_add_folder(struct info_file* info_file, char *path, int path_offset) {

    char file_path[MAX_PATH_LENGTH];

    DIR *dir = opendir(path);

    size_t len = strlen(path);
    path[len++] = '/';

    for(struct dirent *dp; (dp = readdir(dir)); ) {

        if (!strcmp(dp->d_name,".") || !strcmp(dp->d_name,"..")) {

            continue;
        }

        strcpy(path + len, dp->d_name);

        if (is_directory(path)) {

            cvs_add_folder(info_file, path, path_offset);

            continue;
        }

        expand_path(file_path, "%s", path);

        cvs_add_file(info_file, file_path + path_offset);
    }

    closedir(dir);
}


int cvs_add(int argc, char **argv) {

    struct cmd_tmp args = expand_arguments(argc, argv);

    struct info_file *info_file = read_client_file();

    for (int pos = 1; pos < argc; pos++) {

        if (!file_exists("%s", argv[pos])) {

            cvs_error(SRC_MISSING_ERROR, argv[pos]);
        }

        if (is_directory(argv[pos])) {

            strcpy(args.argv[pos], argv[pos]);

            cvs_add_folder(info_file, args.argv[pos], args.path_offset);

        } else {

            cvs_add_file(info_file, args.argv[pos]);
        }
    }

    write_client_file(info_file);
    free_info_file(info_file);
    free(args.argv);

    return 0;
}


static void delete_empty_dirs(char *path, char *cur_path, int cur_path_len, int arg_path_len) {

    for (int pos = strlen(path) - 1; pos > arg_path_len; pos--) {

        if (path[pos] == '/') {

            path[pos] = 0;

            if (pos == cur_path_len && !strcmp(path, cur_path)) break;
            if (!is_empty(path)) break;

            run_bash("rmdir %s", path);
        }
    }
}


static void cvs_delete_file(struct info_file* info_file, struct file *file) {

    struct modification mod;
    mod.action = DELETE;
    mod.file = *file;

    remove_files(info_file, file - info_file->files);

    struct modification *pos = find_last_modifications(info_file, &mod);
    enum action action = 0;

    if (pos) {

        action = pos->action;

        remove_modifications(info_file, pos - info_file->modifications);
    }

    struct modification *m = info_file->modifications;

    for ( ; m < info_file->modifications + info_file->num_modifications && m->action == MOVE; m++ ) ;

    while ( --m >= info_file->modifications ) {

        if (!strcmp(m->new_name, mod.file.name)) {

            strcpy(mod.file.name, m->file.name);

            remove_modifications(info_file, m - info_file->modifications);
        }
    }

    if (action != NEW) {

        add_modifications(info_file, &mod);
    }
}


int cvs_delete(int argc, char **argv) {

    struct cmd_tmp args = expand_arguments(argc, argv);

    struct info_file *info_file = read_client_file();

    char path[MAX_PATH_LENGTH], cur_path[MAX_PATH_LENGTH];

    expand_path(cur_path, ".");

    int cur_path_len = strlen(cur_path);

    int repo_path_len = strlen(repo_path()) + 1;

    struct file *f;

    for (int i = 1; i < argc; i++) {

        int len = strlen(args.argv[i]);

        while ((f = bsearch(args.argv[i], info_file->files, info_file->num_files, sizeof(struct file), sub_files_cmp))) {

            if (args.argv[i][len - 1] == '/' || f->name[len] == '/' || !f->name[len]) {

                sprintf(path, "%s/%s", repo_path(), f->name);

                cvs_delete_file(info_file, f);

                run_bash("rm %s", path);

                delete_empty_dirs(path, cur_path, cur_path_len, repo_path_len + len);
            }
        }
    }

    write_client_file(info_file);
    free_info_file(info_file);
    free(args.argv);

    return 0;
}


void cvs_move_file(struct info_file *info_file, struct file *file_pos, char *old_path, char *new_path) {

    int old_path_len = strlen(old_path);

    if (!strncmp(old_path, new_path, old_path_len) && new_path[old_path_len] == '/') {

        cvs_error(MOVE_INSIDE_ERROR, old_path, new_path);
    }

    if (!strcmp(old_path, new_path)) {

        cvs_error(IDENTICAL_ERROR, old_path);
    }

    if (!file_exists("%s/%s", repo_path(), old_path) && !file_exists("%s/%s", repo_path(), new_path)) {

        cvs_error(SRC_MISSING_ERROR, old_path);
    }

    remove_files(info_file, file_pos - info_file->files);

    struct file new_file;
    strcpy(new_file.name, new_path);

    if (find_files(info_file, &new_file)) {

        cvs_error(DST_EXISTS_ERROR, new_path);
    }

    add_files(info_file, &new_file);

    struct modification mod, new_mod, *mod_pos;
    strcpy(mod.file.name, old_path);
    mod.action = 0;

    while ((mod_pos = find_first_modifications(info_file, &mod))) {

        memcpy(&new_mod, mod_pos, sizeof(struct modification));

        remove_modifications(info_file, mod_pos - info_file->modifications);

        strcpy(new_mod.file.name, new_path);

        add_modifications(info_file, &new_mod);
    }

    strcpy(mod.new_name, new_path);
    mod.action = MOVE;
    add_modifications(info_file, &mod);
}


int cvs_mv(int argc, char **argv) {

    if (argc < 3) {

        cvs_help(2, (char*[]){"help", argv[0]});
        return 1;
    }

    bool isdir = is_directory(argv[argc - 1]);

    if (!isdir && (argc > 3)) {

        cvs_error(DST_NOT_DIR);
    }

    struct cmd_tmp args = expand_arguments(argc, argv);

    struct info_file *info_file = read_client_file();

    char old_path[MAX_PATH_LENGTH], new_path[MAX_PATH_LENGTH], cur_path[MAX_PATH_LENGTH];

    expand_path(cur_path, ".");

    int cur_path_len = strlen(cur_path);

    int repo_path_len = strlen(repo_path()) + 1;

    struct file *f;

    for (int i = 1; i < argc - 1; i++) {

        int len = strlen(args.argv[i]);

        while ((f = bsearch(args.argv[i], info_file->files, info_file->num_files, sizeof(struct file), sub_files_cmp))) {

            if (args.argv[i][len - 1] == '/' || f->name[len] == '/' || !f->name[len]) {

                sprintf(old_path, "%s/%s", repo_path(), f->name);

                char *start = f->name + len - (f->name[len - 1] == '/');

                if (isdir) {

                    for (start--; start > f->name && *(start - 1) != '/'; start--) ;

                    expand_path(new_path, "%s/%s/%s", repo_path(), args.argv[argc - 1], start);

                } else {

                    expand_path(new_path, "%s/%s%s", repo_path(), args.argv[argc - 1], start);
                }

                cvs_move_file(info_file, f, old_path + repo_path_len, new_path + repo_path_len);

                create_path("%s", new_path);

                run_bash("mv %s %s", old_path, new_path);

                delete_empty_dirs(old_path, cur_path, cur_path_len, repo_path_len + len);
            }
        }
    }

    write_client_file(info_file);
    free_info_file(info_file);
    free(args.argv);

    return 0;
}


int cvs_commit(int argc, char **argv) {

    check_num_args(1, argc, argv);

    struct info_file *client_info_file = read_client_file();
    struct info_file *server_info_file = read_server_file(-1);

    if (client_info_file->version != server_info_file->version) {

        cvs_error(CLIENT_BEHIND_ERROR, server_info_file->version - client_info_file->version);
    }

    if (!client_info_file->num_modifications) {

        return 0;
    }

    free(server_info_file->modifications);

    server_info_file->modifications     = client_info_file->modifications;
    server_info_file->num_modifications = client_info_file->num_modifications;

    client_info_file->modifications = NULL;
    client_info_file->num_modifications = 0;

    server_info_file->version++;
    client_info_file->version++;

    for ( struct modification *mod = server_info_file->modifications;
          mod < server_info_file->modifications + server_info_file->num_modifications;
          mod++ )
    {
        struct file *file = find_files(server_info_file, &mod->file);

        if (file) {

            mod->file = *file;

        } else if (mod->action != NEW && mod->action != MOVE) {

            cvs_error(CORRUPT_REPO_ERROR);
        }

        if (!mod->action || !strchr("ADMN", mod->action)) {

            cvs_error(CORRUPT_REPO_ERROR);
        }

        action_helpers[mod->action].move_helper(server_info_file, mod, file);
    }

    write_server_file(server_info_file);
    write_client_file(client_info_file);
    free_info_file(server_info_file);
    free_info_file(client_info_file);

    return 0;
}


int cvs_update(int argc, char **argv) {

    check_num_args(1, argc, argv);

    run_bash("mkdir %s/.cvs && mv %s/* %s/.cvs/", repo_path(), repo_path(), repo_path());

    struct info_file *server_info_file = read_server_file(-1);
    struct info_file *client_info_file = read_client_file();

    for (int i = 0; i < server_info_file->num_files; i++) {

        struct file *f = server_info_file->files + i;

        create_path("%s/%s", repo_path(), f->name);

        run_bash("cp %s/%d/%d %s/%s", base_path(), f->id, f->version, repo_path(), f->name);
    }

    free(client_info_file->files);

    client_info_file->files     = server_info_file->files;
    client_info_file->num_files = server_info_file->num_files;

    free(server_info_file->files);
    server_info_file->files = NULL;

    free_info_file(server_info_file);

    for ( struct modification *mod = client_info_file->modifications;
          mod < client_info_file->modifications + client_info_file->num_modifications;
          mod++ )
    {
        struct file *file = find_files(client_info_file, &mod->file);

        if (!mod->action || !strchr("ADMN", mod->action)) {

            cvs_error(CORRUPT_REPO_ERROR);
        }

        action_helpers[mod->action].update_helper(client_info_file, mod, file);
    }

    run_bash("rm -r %s/.cvs", repo_path());

    write_client_file(client_info_file);
    free_info_file(client_info_file);

    return 0;
}


static struct file* find_file_in_version(struct info_file* info_file, char *name) {

    char tmp[MAX_PATH_LENGTH];

    if (!find_file_in_parents(tmp, CVS_FILE)) {

        cvs_error(REPO_MISSING_ERROR);
    }

    file_name(tmp)[0] = 0;

    int path_offset = strlen(tmp);

    struct file file, *pos;

    expand_path(file.name, name);

    memmove(file.name, file.name + path_offset, MAX_PATH_LENGTH - path_offset);

    if (!(pos = find_files(info_file, &file))) {

        cvs_error(SRC_CONTROL_ERROR, name);
    }

    return pos;
}


static int check_int(char *cmd, char *number) {

    int ret;

    if (!sscanf(number, "%d", &ret)) {

        cvs_help(2, (char*[]){"help", cmd});
        cvs_error("");
    }

    return ret;
}


int cvs_version(int argc, char **argv) {

    check_num_args(2, argc, argv);

    struct info_file *client_info_file = read_client_file();
    struct info_file *server_info_file = read_server_file(client_info_file->version);

    free_info_file(client_info_file);

    struct file *pos = find_file_in_version(server_info_file, argv[1]);

    printf("The file has been modified in the following versions:\n");

    run_bash("ls %s/%d", base_path(), pos->id);

    free_info_file(server_info_file);

    return 0;
}


int cvs_diff(int argc, char **argv) {

    check_num_args(3, argc, argv);

    if (!file_exists(argv[1])) {

        cvs_error(SRC_MISSING_ERROR, argv[1]);
    }

    struct info_file *info_file = read_server_file(check_int(argv[0], argv[2]));

    struct file *pos = find_file_in_version(info_file, argv[1]);

    run_bash("diff %s %s/%d/%d", argv[1], base_path(), pos->id, pos->version);

    free_info_file(info_file);

    return 0;
}


int cvs_revert(int argc, char **argv) {

    check_num_args(3, argc, argv);

    struct info_file *info_file = read_server_file(check_int(argv[0], argv[2]));

    struct file *pos = find_file_in_version(info_file, argv[1]);

    char path[MAX_PATH_LENGTH];

    expand_path(path, "%s", argv[1]);

    create_path("%s", path);

    run_bash("cp %s/%d/%d %s", base_path(), pos->id, pos->version, argv[1]);

    free_info_file(info_file);

    cvs_add(2, (char*[]){"add", argv[1]});

    return 0;
}


int cvs_erased(int argc, char **argv) {

    int version = 0;

    if (argc == 2) {

        version = check_int(argv[0], argv[1]);

    } else {

        check_num_args(1, argc, argv);
    }

    printf("Erased files since version %d:\n", version);

    struct info_file *client_info_file = read_client_file();
    struct info_file *new_info_file = NULL;
    struct info_file *old_info_file = read_server_file(client_info_file->version);

    for (int i = client_info_file->version; i > version; i--) {

        free_info_file(new_info_file);
        new_info_file = old_info_file;
        old_info_file = read_server_file(i - 1);

        for ( struct file *old = old_info_file->files;
              old < old_info_file->files + old_info_file->num_files;
              old++ )
        {
            bool erased = true;

            for ( struct file *new = new_info_file->files;
                  new < new_info_file->files + new_info_file->num_files;
                  new++)
            {
                if (new->id == old->id) {

                    erased = false;
                    break;
                }
            }

            if (erased) {

                printf("%s in version %d\n", old->name, i);
            }
        }
    }

    free_info_file(old_info_file);
    free_info_file(client_info_file);

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


