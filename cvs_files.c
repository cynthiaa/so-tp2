#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "cvs_files.h"
#include "cvs_helpers.h"
#include "cvs_errors.h"


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


static bool validate_server_modification(struct server_modification *m) {

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


static bool validate_client_modification(struct client_modification *m) {

    m->name[MAX_PATH_LENGTH - 1] = 0;

    int length = strlen(m->name);

    if (length == 0 || length == MAX_PATH_LENGTH -1)
        return false;

    if (!m->action || !strchr("ADM", m->action))
        return false;

    if (m->action == MOVE) {

        m->new_name[MAX_PATH_LENGTH - 1] = 0;

        length = strlen(m->new_name);

        if (length == 0 || length == MAX_PATH_LENGTH -1)
            return false;
    }

    return true;
}


static void read_server_modification_line(FILE *file, struct server_modification *m) {

    if (fscanf(file, "%s %d %d %c", m->file.name, &m->file.version, &m->file.id, (char*)&m->action) != 4) {

        cvs_error(CORRUPT_REPO_ERROR);
    }

    if (m->action == MOVE) {

        if (fscanf(file, " %s", m->new_name) != 1) {

             cvs_error(CORRUPT_REPO_ERROR);
        }
    }

    if (fscanf(file, "\n") != 0 || !validate_server_modification(m)) {

        cvs_error(CORRUPT_REPO_ERROR);
    }
}


static void read_client_modification_line(FILE *file, struct client_modification *m) {

    if (fscanf(file, "%s %c", m->name, (char*)&m->action) != 2) {

        cvs_error(CORRUPT_REPO_ERROR);
    }

    if (m->action == MOVE) {

        if (fscanf(file, " %s", m->new_name) != 1) {

             cvs_error(CORRUPT_REPO_ERROR);
        }
    }

    if (fscanf(file, "\n") != 0 || !validate_client_modification(m)) {

        cvs_error(CORRUPT_REPO_ERROR);
    }
}


static void read_file_line(FILE *file, struct file *f) {

    if (fscanf(file, "%s %d %d\n", f->name, &f->version, &f->id) != 3 || !validate_file(f)) {

        cvs_error(CORRUPT_REPO_ERROR);
    }
}


static void write_server_modification_line(FILE *file, struct server_modification *m) {

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


static void write_client_modification_line(FILE *file, struct client_modification *m) {

    if (fprintf(file, "%s %c", m->name, m->action) <= 0) {

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

    ret->modifications = malloc(sizeof(struct client_modification) * ret->num_modifications);

    if (!ret->modifications) {

        cvs_error(MEMORY_ERROR);
    }

    for (int i = 0; i < ret->num_modifications; i++) {

        read_client_modification_line(file, ret->modifications + i);
    }

    return ret;
}


void write_client_file(FILE *file, struct client_file *client_file) {

    write_number_line(file, client_file->version);
    write_number_line(file, client_file->num_modifications);

    for (int i = 0; i < client_file->num_modifications; i++) {

        write_client_modification_line(file, client_file->modifications + i);
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

    ret->modifications = malloc(sizeof(struct server_modification) * ret->num_modifications);

    if (!ret->modifications) {

        cvs_error(MEMORY_ERROR);
    }

    for (int i = 0; i < ret->num_modifications; i++) {

        read_server_modification_line(file, ret->modifications + i);
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

        write_server_modification_line(file, server_file->modifications + i);
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
