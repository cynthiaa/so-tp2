#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    run_bash("echo \"$(cd $(eval dirname \"%s\"); pwd)/$(basename \"%s\")\" > %s", name, name, tmp_name);

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


bool find_file_in_parents(char *path, char *file) {

    expand_path(path, file);

    /* TODO */

    return true;
}

