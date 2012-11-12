#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define CVS_SV_DIR "/var/cvs/"

int cvs_create(char *path);
int check_uri(char *path);
int cvs_checkout(char* remote);
int cvs_add(char* path);
int cvs_remove(char* path);
int cvs_move(char* old, char* new);
int cvs_update();
int cvs_diff_all(int revision);
int cvs_diff_file(int file);
