#ifndef _CVS_CORE_HELPERS_H_
#define _CVS_CORE_HELPERS_H_


#define UNUSED(x) ((void)(x))


static void cvs_move_alter(struct info_file *info_file, struct modification *mod, struct file *file) {

    file->version = mod->file.version = info_file->version;

    run_bash("cp %s/%s %s/%d/%d", repo_path(), mod->file.name, base_path(), mod->file.id, mod->file.version);
}


static void cvs_move_delete(struct info_file *info_file, struct modification *mod, struct file *file) {

    UNUSED(mod);

    remove_files(info_file, file - info_file->files);
}


static void cvs_move_move(struct info_file *info_file, struct modification *mod, struct file *file) {

    struct file new_file;

    if (!file) {

        mod->file.id = new_file.id = info_file->next_file_id++;
        mod->file.version = new_file.version = info_file->version;

    } else {

        mod->file.id = new_file.id = file->id;
        mod->file.version = new_file.version = file->version;

        remove_files(info_file, file - info_file->files);
    }

    strcpy(new_file.name, mod->new_name);

    add_files(info_file, &new_file);
}


static void cvs_move_new(struct info_file *info_file, struct modification *mod, struct file *file) {

    if (!file) {

        mod->file.id = info_file->next_file_id++;
        mod->file.version = info_file->version;

        add_files(info_file, &mod->file);
    }

    create_path("%s/%d/%d", base_path(), mod->file.id, mod->file.version);

    run_bash("cp %s/%s %s/%d/%d", repo_path(), mod->file.name, base_path(), mod->file.id, mod->file.version);
}


static void cvs_update_alter(struct info_file *info_file, struct modification *mod, struct file *file) {

    if (!file) {

        mod->action = NEW;

        add_files(info_file, &mod->file);

        create_path("%s/%s", base_path(), mod->file.name);

        run_bash("mv %s/.cvs/%s %s/%s", repo_path(), mod->file.name, repo_path(), mod->file.name);

        return;
    }

    mod->action = ALTER;

    if (!run_bash("diff -q %s/%s %s/.cvs/%s", repo_path(), mod->file.name, repo_path(), mod->file.name)) {

        return;
    }

    printf( "Conflict in file `%s/%s`.\nSolve it and run `cvs resolved %s/%s`.\n",
            repo_path(), mod->file.name, repo_path(), mod->file.name );

    run_bash( "diff -u %s/%s %s/.cvs/%s > %s/%s", repo_path(), mod->file.name,
              repo_path(), mod->file.name, repo_path(), mod->file.name );
}


static void cvs_update_delete(struct info_file *info_file, struct modification *mod, struct file *file) {

    if (!file) {

        return;
    }

    remove_files(info_file, file - info_file->files);

    run_bash("rm %s/%s 2> /dev/null", repo_path(), mod->file.name);
}


static void cvs_update_move(struct info_file *info_file, struct modification *mod, struct file *file) {

    if (!file) {

        cvs_error(TREE_CONFLICT_ERROR);
    }

    remove_files(info_file, file - info_file->files);

    struct file new_file;

    strcpy(new_file.name, mod->new_name);

    if (!(file = find_files(info_file, &new_file))) {

        add_files(info_file, &new_file);
    }

    run_bash("mv %s/%s %s/%s", repo_path(), mod->file.name, repo_path(), mod->new_name);
}


static void cvs_update_new(struct info_file *info_file, struct modification *mod, struct file *file) {

    cvs_update_alter(info_file, mod, file);
}


static struct {

    void (*move_helper)(struct info_file*, struct modification*, struct file*);

    void (*update_helper)(struct info_file*, struct modification*, struct file*);

} action_helpers[] = {

    ['A'] = {cvs_move_alter,  cvs_update_alter },
    ['D'] = {cvs_move_delete, cvs_update_delete},
    ['M'] = {cvs_move_move,   cvs_update_move  },
    ['N'] = {cvs_move_new,    cvs_update_new   },
};


#endif /* _CVS_CORE_HELPERS_H_ */

