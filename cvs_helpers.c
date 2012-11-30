#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
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


void* cvs_malloc(size_t size) {

    void *tmp = malloc(size);

    if (!tmp) {

        cvs_error(MEMORY_ERROR);
    }

    return tmp;
}


int run_bash(char *fmt, ...) {

    static char cmd[MAX_PATH_LENGTH];
    va_list vl;

    va_start(vl, fmt);
    vsprintf(cmd, fmt, vl);
    va_end(vl);

    int ret = system(cmd);

    if (ret == -1) {

        cvs_error(COMMAND_ERROR);
    }

    return ret;
}


static void vexpand_path(char *path, char *fmt, va_list vl) {

    char name[MAX_PATH_LENGTH], tmp[MAX_PATH_LENGTH];

    vsprintf(name, fmt, vl);

    if (name[0] == '~' && (!name[1] || name[1] == '/')) {

        sprintf(path, "%s%s", getenv("HOME"), name + 1);
        strcpy(name, path);
    }

    char *pos = strchr(name + 1, '/');

    for ( ; pos; pos = strchr(pos + 1, '/')) {

        sprintf(tmp, "%.*s", (int)(pos - name), name);

        if (access(tmp, R_OK)) {

            break;
        }
    }

    if (!pos) {

        realpath(name, path);

        return;
    }

    sprintf(tmp, "%.*s", (int)(pos - name), name);

    realpath(name, path);

    strcpy(path + strlen(path), pos);
}


void expand_path(char *name, char *fmt, ...) {

    va_list vl;

    va_start(vl, fmt);
    vexpand_path(name, fmt, vl);
    va_end(vl);
}


FILE* open_file(char *flags, char *fmt, ...) {

    static char name[MAX_PATH_LENGTH];
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


void create_path(char *fmt, ...) {

    static char path[MAX_PATH_LENGTH];
    va_list vl;

    va_start(vl, fmt);
    vsprintf(path, fmt, vl);
    va_end(vl);

    for (char *pos, *last = path + 1; (pos = strchr(last, '/')); last = pos + 1) {

        run_bash("mkdir \"%.*s\" 2> /dev/null", (int)(pos - path), path);
    }
}


bool find_file_in_parents(char *path, char *file) {

    char tmp[MAX_PATH_LENGTH], *old, *new = tmp;

    expand_path(path, file);

    while (!file_exists(path) && strchr(path + strspn(path, "/"), '/')){

        strcpy(old = new, "../");
        strcpy(new += strlen("../"), file);
        expand_path(path, tmp);
    }

    return file_exists(path);
}


bool file_exists(char *fmt, ...) {

    static char path[MAX_PATH_LENGTH];
    va_list vl;

    va_start(vl, fmt);
    vexpand_path(path, fmt, vl);
    va_end(vl);

    return !access(path, R_OK);
}


bool is_directory(char *file) {

    struct stat buf;

    return !stat(file, &buf) && (buf.st_mode & S_IFDIR);
}


bool is_empty(char *directory) {

    struct dirent *dp;
    DIR *dir = opendir(directory);

    while ((dp = readdir(dir))) {

        if (!strcmp(dp->d_name,".") || !strcmp(dp->d_name,"..")) {

            continue;
        }

        closedir(dir);
        return false;
    }

    closedir(dir);
    return true;
}


char* file_name(char *path) {

    int pos = strlen(path);

    for ( ; pos >= 0 && path[pos] == '/'; pos--) ;
    for ( ; pos >= 0 && path[pos] != '/'; pos--) ;

    return path + (pos + 1);
}

