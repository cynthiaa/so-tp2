#ifndef _CVS_FILES_H_
#define _CVS_FILES_H_

#include "cvs_helpers.h"


/**
 * Directorio del servidor
 *
 * @warning Tiene que terminar en /
 */
#ifndef CVS_DIR
#define CVS_DIR  "~/.cvs/"
#endif


/**
 * Nombre del archivo de informción de versión del cliente
 */
#ifndef CVS_FILE
#define CVS_FILE  ".cvs_info"
#endif


/**
 * Estructura de una línea de archivo
 */
struct file {

    char name[MAX_PATH_LENGTH];
    int version;
    int id;
};


/**
 * Símbolos de acción en las líneas de modificación
 */
enum action {

    ADD    = 'A',
    DELETE = 'D',
    MOVE   = 'M',
};


/**
 * Estructura de una línea de modificación de servidor
 */
struct server_modification {

    struct file file;
    enum action action;
    char new_name[MAX_PATH_LENGTH];
};


/**
 * Estructura de una línea de modificación de servidor
 */
struct client_modification {

    char name[MAX_PATH_LENGTH];
    enum action action;
    char new_name[MAX_PATH_LENGTH];
};


/**
 * Estructura de un archivo de versión de cliente
 */
struct client_file {

    int version;
    int num_modifications;
    struct client_modification *modifications;
};


/**
 * Estructura de un archivo de versión de servidor
 */
struct server_file {

    int version;
    int next_file_id;
    int num_modifications;
    struct server_modification *modifications;
    int num_files;
    struct file *files;
};


/**
 * Lee un archivo de versión del cliente
 *
 * @return estructura del archivo
 */
struct client_file* read_client_file(void);


/**
 * Escribe un archivo de versión del cliente
 *
 * @param client_file  estructura del archivo
 */
void write_client_file(struct client_file *client_file);


/**
 * Lee un archivo de versión del servidor
 *
 * @param version  qué versión abrir, si es -1 abre la última
 *
 * @return estructura del archivo
 */
struct server_file* read_server_file(int version);


/**
 * Escribe un archivo de versión del servidor
 *
 * @param server_file  estructura del archivo
 */
void write_server_file(struct server_file *server_file);


/**
 * Libera recursos usados por client_file
 *
 * @param client_file
 */
void free_client_file(struct client_file *client_file);


/**
 * Libera recursos usados por server_file
 *
 * @param server_file
 */
void free_server_file(struct server_file *server_file);


#endif /* _CVS_FILES_H_ */

