#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cvs_back.h"

#define CMD_LENGTH      256
#define MAX_PATH_LENGTH 512

static struct file {
    char name[MAX_PATH_LENGTH];
    int version;
    int id;
};

static struct modification {
    struct file file;
    enum {ADD = 'A', DELETE = 'D', MOVE = 'M'} action;
    char new_name[MAX_PATH_LENGTH];
};

static int create_path(char *path) {

    char cmd[CMD_LENGTH];

    for (char *pos, *last = path; pos = strchr(last, '\'); last = pos + 1) {

        sprintf(cmd, "mkdir %*s", pos - path, path);

        if (system(cmd) == -1)
            return 1;
    }

    return 0;
}

static void read_line(FILE *file) {

    int c;

    do {
        c = fgetc(file);
    } while (c != EOF && c != '\n');
}

static int read_number_line(FILE *file) {

    int n;

    fscanf(file, "%d\n", &n);

    return n;
}

static struct modification read_modification_line(FILE *file) {

    struct modification ret;

    fscanf(file, "%s %d %d %c", &ret.file.name, &ret.file.version, &ret.file.id, &ret.action);

    if (ret.action == MOVE) {

        fscanf(file, "%s", &ret.new_name);
    }

    scanf(file, "\n");

    return ret;
}

static struct file read_file_line(FILE *file) {

    struct file ret;

    fscanf(file, "%s %d %d\n", &ret.name, &ret.version, &ret.id);

    return ret;
}

int cvs_checkout(int argc, char **argv) {

    char *cmd[CMD_LENGTH];

    sprintf(cmd, "%sinfo/current", CVS_DIR);
    FILE *file = fopen(cmd, "r");

    if (!file)
        return 1;

    int version = read_number(file);
    read_line(file);

    int lines = read_number(file);

    while (lines--)
        read_line(file);

    lines = read_number(file);

    while (lines--) {

        struct file file_line = read_file(file);

        if (create_path(file_line.name))
            return 1;

        sprintf(cmd, "cp %s%d/%d %s", CVS_DIR, file_line.id, file_line.version, file_line.name);

        if (system(cmd) == -1)
            return 1;
    }

    fclose(file);

    file = fopen(".cvs_info", "w");

    if (!file)
        return 1;

    fprintf(file, "%d\n0\n", version);

    fclose(file);

    return 0;
}

int cvs_add(int argc, char **argv) {


    return 0;
}

int cvs_delete(int argc, char **argv) {

    return 0;
}

int cvs_update(int argc, char **argv) {

    return 0;
}

int cvs_commit(int argc, char **argv) {

    return 0;
}

int cvs_diff(int argc, char **argv) {

    return 0;
}

int cvs_version(int argc, char **argv) {

    return 0;
}


int cvs_revert(int argc, char **argv) {

    return 0;
}

int cvs_mv(int argc, char **argv) {

    return 0;
}

int cvs_format(int argc, char **argv) {

    char cmd[CMD_LENGTH];

    sprintf(cmd, "cd %s && rm -rf *", CVS_DIR);
    if (system(cmd) == -1)
        return 1;

    sprintf(cmd, "cd %s && mkdir info", CVS_DIR);
    if (system(cmd) == -1)
        return 1;

    sprintf(cmd, "%sinfo/0", CVS_DIR);
    FILE *file = fopen(cmd, "w");
    if (!file)
        return 1;

    fprintf(file, "0\n0\n0\n0\n");
    fclose(file);

    sprintf(cmd, "ln %sinfo/current %sinfo/0", CVS_DIR, CVS_DIR);
    if (system(cmd) == -1)
        return 1;

    return 0;
}
