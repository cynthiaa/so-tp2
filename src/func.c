#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define CVS_SV_DIR "/var/cvs/"

static char* error;
int cvs_create(char *path) 
{
	//create folders
	char *mkdir, *s;
	s = CVS_SV_DIR;
	//@TODO check permission to use var folder
	sprintf(mkdir, "mkdir -p %s \"%s\"", s, path);
	system(mkdir);
	//create hidden config file
	sprintf(s, "touch %s/.cvs_config", mkdir);
	system(s);
	//@TODO actually config the config file
	return 0;
}

int check_uri(char *path)
{
	//char* base_uri = "cvs://";
	return 0;
}



int cvs_checkout(char* remote)
{
	check_uri(remote);
	return 0;
}



int cvs_add(char* path)
{
	return 0;
}
   

int cvs_remove(char* path)
{
	//tomar parametros, meterlos en un unico string y pasarlo como parámetro. Evitar espacios de mas

	char* cmd;
	sprintf(cmd, "rm \"%s\" ", path);

	//system(cmd);
	return 0;
}


int cvs_move(char* old, char* new)
{
	return 0;
}

int cvs_update()
{
	return 0;
}

int cvs_diff_all(int revision)
{
	return 0;
}

int cvs_diff_file(int file)
{
	return 0;
}