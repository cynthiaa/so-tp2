#ifndef _CVS_H_
#define _CVS_H_

#include <stdlib.h>
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

extern const size_t num_commands;


/**
 * Función de comparación de `commands` para usar con bsearch, etc
 */
int command_cmp(const void *name, const void *cmd);


/**
 * Busca un comando
 *
 * @param command  nombre del comando a buscar
 *
 * @return puntero al comando, o NULL si no fue encontrado
 */
static inline struct command* find_command(char *command) {

    return bsearch(command, commands, num_commands, sizeof(struct command), command_cmp);
}


#endif /* _CVS_H_ */

