#include <string.h>
#include <string.h>

static char* error;

int check_uri(char *path)
{
	char* base_uri = "cvs://";
	for (int i = 0; i < strlen(base_uri) ; ++i) {
			if (path[i] != base_uri[i]) {
				error = "unknown remote directory";
				return -1;
			}
	}
}



int cvs_checkout(char* remote)
{
	check_uri(remote);

}



int cvs_add(char* path)
{



}
   

int cvs_remove(char* path)
{

}


int cvs_move(char* old, char* new)
{

}

int cvs_update()
{

}

int cvs_diff_all(int revision)
{

}