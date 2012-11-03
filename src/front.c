#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

int router(int argc, const char *argv[])
{
    if (strcmp(argv[1], "checkout") == 0) {
        char *remote = (char *)argv[2];

        if (cvs_checkout(remote) == -1) {
            printf("Unable to checkout repository\n");
            return -1;
        }
    } else if (strcmp(argv[1], "add") == 0) {
        char *path = (char *)argv[2];

        if (cvs_add(path) == -1) {
            printf("Unable to move file\n");
            return -1;
        }
    } else if (strcmp(argv[1], "rm") == 0) {
        char *path = (char *)argv[2];

        if (cvs_remove(path) == -1) {
            printf("Unable to remove file\n");
            return -1;
        }
    } else if (strcmp(argv[1], "mv") == 0) {
        char *old = (char *)argv[2];
        char *new = (char *)argv[3];

        if (cvs_move(old, new) == -1) {
            printf("Unable to move file\n");
            return -1;
        }
    } else if (strcmp(argv[1], "update") == 0) {
        if (cvs_update() == -1) {
            printf("Unable to update repository\n");
            return -1;
        }
    } else if (strcmp(argv[1], "diff") == 0) {
        /* TODO: check to see if revision was passed */
        int revision = (int)strtol(argv[2], (char **)NULL, 10);

        if (cvs_diff_all(revision) == -1) {
            /* code */
        }
        if (cvs_diff_file(revision) == -1) {
            /* code */
        }
    }

}

int main(int argc, const char *argv[])
{
    return 0;
}
