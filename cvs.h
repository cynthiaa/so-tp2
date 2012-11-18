#ifndef _CVS_H_
#define _CVS_H_

#include "cvs_core.h"


/**
 * Información de todos los comandos disponibles
 *
 * @warning tienen que estar ordenados alfabéticamente
 */
struct command {

    char *name;
    int (*handler)(int argc, char **argv);
    char *help;

};


extern const struct command commands[];


/**
 * Función de comparación de `commands` para usar con bsearch, etc
 */
int cmdcmp(const void *name, const void *cmd);


#endif /* _CVS_H_ */

