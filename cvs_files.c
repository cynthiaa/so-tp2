#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "cvs_files.h"
#include "cvs_errors.h"


int files_cmp(const void *file1, const void *file2) {

    return strcmp(((struct file*)file1)->name, ((struct file*)file2)->name);
}


int modifications_cmp(const void *mod1, const void *mod2) {

    return strcmp(((struct modification*)mod1)->file.name, ((struct modification*)mod2)->file.name);
}


static int read_number_line(FILE *file) {

    int n;

    if (fscanf(file, "%d\n", &n) != 1 || n < 0) {

        cvs_error(CORRUPT_REPO_ERROR);
    }

    return n;
}


static void write_number_line(FILE *file, int n) {

    if (fprintf(file, "%d\n", n) <= 0) {

        cvs_error(WRITE_ERROR);
    }
}


static bool validate_file(struct file *f, bool server) {

    f->name[MAX_PATH_LENGTH - 1] = 0;

    int length = strlen(f->name);

    if (length == 0 || length == MAX_PATH_LENGTH -1)
        return false;

    if (server && f->version < 0)
        return false;

    if (server && f->id < 0)
        return false;

    return true;
}


static bool validate_modification(struct modification *m, bool server) {

    if (!validate_file(&m->file, server))
        return false;

    if (!m->action || !strchr("ADMN", m->action))
        return false;

    if (m->action == MOVE) {

        m->new_name[MAX_PATH_LENGTH - 1] = 0;

        int length = strlen(m->new_name);

        if (length == 0 || length == MAX_PATH_LENGTH -1)
            return false;
    }

    return true;
}



static void read_server_modification_line(FILE *file, struct modification *m) {

    if (fscanf(file, "%s %d %d %c", m->file.name, &m->file.version, &m->file.id, (char*)&m->action) != 4) {

        cvs_error(CORRUPT_REPO_ERROR);
    }

    if (m->action == MOVE) {

        if (fscanf(file, " %s", m->new_name) != 1) {

             cvs_error(CORRUPT_REPO_ERROR);
        }
    }

    if (fscanf(file, "\n") != 0 || !validate_modification(m, true)) {

        cvs_error(CORRUPT_REPO_ERROR);
    }
}


static void read_client_modification_line(FILE *file, struct modification *m) {

    if (fscanf(file, "%s %c", m->file.name, (char*)&m->action) != 2) {

        cvs_error(CORRUPT_REPO_ERROR);
    }

    if (m->action == MOVE) {

        if (fscanf(file, " %s", m->new_name) != 1) {

             cvs_error(CORRUPT_REPO_ERROR);
        }
    }

    if (fscanf(file, "\n") != 0 || !validate_modification(m, false)) {

        cvs_error(CORRUPT_REPO_ERROR);
    }
}


static void read_server_file_line(FILE *file, struct file *f) {

    if (fscanf(file, "%s %d %d\n", f->name, &f->version, &f->id) != 3 || !validate_file(f, true)) {

        cvs_error(CORRUPT_REPO_ERROR);
    }
}


static void read_client_file_line(FILE *file, struct file *f) {

    if (fscanf(file, "%s\n", f->name) != 1 || !validate_file(f, false)) {

        cvs_error(CORRUPT_REPO_ERROR);
    }
}


static void write_server_modification_line(FILE *file, struct modification *m) {

    if (fprintf(file, "%s %d %d %c", m->file.name, m->file.version, m->file.id, m->action) <= 0) {

        cvs_error(WRITE_ERROR);
    }

    if (m->action == MOVE && fprintf(file, " %s", m->new_name) <= 0) {

        cvs_error(WRITE_ERROR);
    }

    if (fprintf(file, "\n") <= 0) {

        cvs_error(WRITE_ERROR);
    }
}


static void write_client_modification_line(FILE *file, struct modification *m) {

    if (fprintf(file, "%s %c", m->file.name, m->action) <= 0) {

        cvs_error(WRITE_ERROR);
    }

    if (m->action == MOVE && fprintf(file, " %s", m->new_name) <= 0) {

        cvs_error(WRITE_ERROR);
    }

    if (fprintf(file, "\n") <= 0) {

        cvs_error(WRITE_ERROR);
    }
}


static void write_server_file_line(FILE *file, struct file *f) {

    if (fprintf(file, "%s %d %d\n", f->name, f->version, f->id) <= 0) {

        cvs_error(WRITE_ERROR);
    }
}


static void write_client_file_line(FILE *file, struct file *f) {

    if (fprintf(file, "%s\n", f->name) <= 0) {

        cvs_error(WRITE_ERROR);
    }
}


#define ACTOR client
#include "file_io.def"


#define ACTOR server
#include "file_io.def"


struct info_file* read_client_file(void) {

    char path[MAX_PATH_LENGTH];

    if (!find_file_in_parents(path, CVS_FILE)) {

        cvs_error(REPO_MISSING_ERROR);
    }

    return __read_client_file(path);
}


void write_client_file(struct info_file *info_file) {

    char path[MAX_PATH_LENGTH];

    if (!find_file_in_parents(path, CVS_FILE)) {

        cvs_error(REPO_MISSING_ERROR);
    }

    __write_client_file(info_file, path);
}


struct info_file* read_server_file(int version) {

    char path[MAX_PATH_LENGTH];

    sprintf(path, version == -1 ? "%sinfo/current" : "%sinfo/%d", CVS_DIR, version);

    return __read_server_file(path);
}


void write_server_file(struct info_file *info_file) {

    if (file_exists("%sinfo/current", CVS_DIR)) {

        struct info_file* cur = read_server_file(-1);
        info_file->version = cur->version + 1;
    }

    char path[MAX_PATH_LENGTH];

    sprintf(path, "%sinfo/%d", CVS_DIR, info_file->version);

    __write_server_file(info_file, path);

    char base[MAX_PATH_LENGTH];

    expand_path(base, CVS_DIR);

    run_bash("ln %s/info/%d %s/info/current", base, info_file->version, base);
}


void free_info_file(struct info_file *info_file) {

    if (!info_file)
        return;

    free(info_file->modifications);
    free(info_file->files);
    free(info_file);
}

