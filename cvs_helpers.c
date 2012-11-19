#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "cvs_helpers.h"
#include "cvs_errors.h"


void cvs_error(char *fmt, ...) {

    va_list vl;

    if (fmt) {
        va_start(vl, fmt);
        vprintf(fmt, vl);
        va_end(vl);
    }

    exit(1);
}


void run_bash(char *fmt, ...) {

    static char cmd[CMD_LENGTH];
    va_list vl;

    va_start(vl, fmt);
    vsprintf(cmd, fmt, vl);
    va_end(vl);

    if (system(cmd) == -1) {

        cvs_error(COMMAND_ERROR);
    }
}


static void vexpand_path(char *name, char *fmt, va_list vl) {

    vsprintf(name, fmt, vl);

    static char tmp_name[FILENAME_MAX];

    tmpnam(tmp_name);

    run_bash("echo \"`eval echo '%s'`\" > %s", name, tmp_name);

    FILE *tmp_file = fopen(tmp_name, "r");

    if (!tmp_file) {

        cvs_error(GENERIC_ERROR);
    }

    fgets(name, FILENAME_MAX, tmp_file);

    fclose(tmp_file);

    char *pos = strchr(name, '\n');

    if (!pos) {

        cvs_error(GENERIC_ERROR);
    }

    *pos = 0;
}


void expand_path(char *name, char *fmt, ...) {

    va_list vl;

    va_start(vl, fmt);

    vexpand_path(name, fmt, vl);

    va_end(vl);
}


FILE* open_file(char *flags, char *fmt, ...) {

    static char name[FILENAME_MAX];
    va_list vl;

    va_start(vl, fmt);
    vexpand_path(name, fmt, vl);
    va_end(vl);

    FILE *file = fopen(name, flags);

    if (!file) {

        cvs_error(GENERIC_ERROR);
    }

    return file;
}


void create_path(char *path) {

    for (char *pos, *last = path + 1; (pos = strchr(last, '/')); last = pos + 1) {

        run_bash("mkdir \"%.*s\"", (int)(pos - path), path);
    }
}


static int read_number_line(FILE *file) {

    int n;

    if (fscanf(file, "%d\n", &n) != 1 || n < 0) {

        cvs_error(CORRUPTED_REPOSITORY);
    }

    return n;
}

static void write_number_line(FILE *file, int n) {

    if (fprintf(file, "%d\n", n) <= 0) {

        cvs_error(WRITE_ERROR);
    }
}


static bool validate_file(struct file *f) {

    f->name[MAX_PATH_LENGTH - 1] = 0;

    int length = strlen(f->name);

    if (length == 0 || length == MAX_PATH_LENGTH -1)
        return false;

    if (f->version < 0)
        return false;

    if (f->id < 0)
        return false;

    return true;
}


static bool validate_modification(struct modification *m) {

    if (!validate_file(&m->file))
        return false;

    if (!m->action || !strchr("ADM", m->action))
        return false;

    if (m->action == MOVE) {

        m->new_name[MAX_PATH_LENGTH - 1] = 0;

        int length = strlen(m->new_name);

        if (length == 0 || length == MAX_PATH_LENGTH -1)
            return false;
    }

    return true;
}


static void read_modification_line(FILE *file, struct modification *m) {

    if (fscanf(file, "%s %d %d %c", m->file.name, &m->file.version, &m->file.id, (char*)&m->action) != 4) {

        cvs_error(CORRUPTED_REPOSITORY);
    }

    if (m->action == MOVE) {

        if (fscanf(file, " %s", m->new_name) != 1) {

             cvs_error(CORRUPTED_REPOSITORY);
        }
    }

    if (fscanf(file, "\n") != 0 || !validate_modification(m)) {

        cvs_error(CORRUPTED_REPOSITORY);
    }
}


static void read_file_line(FILE *file, struct file *f) {

    if (fscanf(file, "%s %d %d\n", f->name, &f->version, &f->id) != 3 || !validate_file(f)) {

        cvs_error(CORRUPTED_REPOSITORY);
    }
}


static void write_modification_line(FILE *file, struct modification *m) {

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


static void write_file_line(FILE *file, struct file *f) {

    if (fprintf(file, "%s %d %d\n", f->name, f->version, f->id) <= 0) {

        cvs_error(WRITE_ERROR);
    }
}


struct client_file* read_client_file(FILE *file) {

    struct client_file *ret = malloc(sizeof(struct client_file));

    if (!ret) {

        cvs_error(MEMORY_ERROR);
    }

    ret->version           = read_number_line(file);
    ret->num_modifications = read_number_line(file);

    ret->modifications = malloc(sizeof(struct modification) * ret->num_modifications);

    if (!ret->modifications) {

        cvs_error(MEMORY_ERROR);
    }

    for (int i = 0; i < ret->num_modifications; i++) {

        read_modification_line(file, ret->modifications + i);
    }

    return ret;
}


void write_client_file(FILE *file, struct client_file *client_file) {

    write_number_line(file, client_file->version);
    write_number_line(file, client_file->num_modifications);

    for (int i = 0; i < client_file->num_modifications; i++) {

        write_modification_line(file, client_file->modifications + i);
    }
}


struct server_file* read_server_file(FILE *file) {

    struct server_file *ret = malloc(sizeof(struct server_file));

    if (!ret) {

        cvs_error(MEMORY_ERROR);
    }

    ret->version           = read_number_line(file);
    ret->next_file_id      = read_number_line(file);
    ret->num_modifications = read_number_line(file);

    ret->modifications = malloc(sizeof(struct modification) * ret->num_modifications);

    if (!ret->modifications) {

        cvs_error(MEMORY_ERROR);
    }

    for (int i = 0; i < ret->num_modifications; i++) {

        read_modification_line(file, ret->modifications + i);
    }

    ret->num_files = read_number_line(file);

    ret->files = malloc(sizeof(struct file) * ret->num_files);

    if (!ret->files) {

        cvs_error(MEMORY_ERROR);
    }

    for (int i = 0; i < ret->num_files; i++) {

        read_file_line(file, ret->files + i);
    }

    return ret;
}


void write_server_file(FILE *file, struct server_file *server_file) {

    write_number_line(file, server_file->version);
    write_number_line(file, server_file->next_file_id);
    write_number_line(file, server_file->num_modifications);

    for (int i = 0; i < server_file->num_modifications; i++) {

        write_modification_line(file, server_file->modifications + i);
    }

    write_number_line(file, server_file->num_files);

    for (int i = 0; i < server_file->num_files; i++) {

        write_file_line(file, server_file->files + i);
    }
}


void free_client_file(struct client_file *client_file) {

    if (!client_file)
        return;

    free(client_file->modifications);
    free(client_file);
}


void free_server_file(struct server_file *server_file){

    if (!server_file)
        return;

    free(server_file->modifications);
    free(server_file->files);
    free(server_file);
}

