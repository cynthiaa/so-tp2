#ifndef _CVS_HELPERS_H_
#define _CVS_HELPERS_H_


/**
 * Máxima longitud de un comando de bash
 */
#define CMD_LENGTH   512


/**
 * Máxima longitud de un path
 */
#define MAX_PATH_LENGTH  512


/**
 * Estructura de una línea de archivo
 */
struct file {

    char name[MAX_PATH_LENGTH];
    int version;
    int id;
};


/**
 * Estructura de una línea de modificación
 */
struct modification {

    struct file file;
    enum {ADD = 'A', DELETE = 'D', MOVE = 'M'} action;
    char new_name[MAX_PATH_LENGTH];
};


/**
 * Estructura de un archivo de versión de cliente
 */
struct client_file {

    int version;
    int num_modifications;
    struct modification *modifications;
};


/**
 * Estructura de un archivo de versión de servidor
 */
struct server_file {

    int version;
    int next_file_id;
    int num_modifications;
    struct modification *modifications;
    int num_files;
    struct file *files;
};


/**
 * Imprime un error y sale de la aplicación
 *
 * @param fmt  mismos parámetros que el printf
 */
void cvs_error(char *fmt, ...);


/**
 * Corre un comando de bash
 *
 * @param fmt  mismos parámetros que el printf
 */
void run_bash(char *fmt, ...);


/**
 * Abre un archivo
 *
 * @param fmt  mismos parámetros que el printf
 */
FILE* open_file(char *flags, char *fmt, ...);


/**
 * Crea las carpetas necesarias para guardar un archivo
 *
 * @param path  nombre del archivo
 */
void create_path(char *path);


/**
 * Lee un archivo de versión del cliente
 *
 * @param file  archivo
 */
struct client_file* read_client_file(FILE *file);


/**
 * Escribe un archivo de versión del cliente
 *
 * @param file         archivo
 * @param client_file  estructura del archivo
 */
void write_client_file(FILE *file, struct client_file *client_file);


/**
 * Lee un archivo de versión del servidor
 *
 * @param file  archivo
 */
struct server_file* read_server_file(FILE *file);


/**
 * Escribe un archivo de versión del servidor
 *
 * @param file         archivo
 * @param server_file  estructura del archivo
 */
void write_server_file(FILE *file, struct server_file *server_file);


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


#endif /* _CVS_HELPERS_H_ */

