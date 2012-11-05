#include <string.h>
#include <string.h>

static char* error;

int cvs_checkout(char* remote)
{
	char* base_uri = "cvs://";
	for (int i = 0; i < strlen(base_uri) ; ++i) {
			if (remote[i] != base_uri[i]) {
				error = "unknown remote directory";
				return -1;
			}
	}


}