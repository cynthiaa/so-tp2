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
 * Símbolos de acción en las líneas de modificación
 */
enum action {

    ALTER  = 'A',
    DELETE = 'D',
    MOVE   = 'M',
    NEW    = 'N',
};


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
    enum action action;
    char new_name[MAX_PATH_LENGTH];
};


/**
 * Estructura de un archivo de versión
 */
struct info_file {

    int version;
    int next_file_id;

    int num_modifications;
    struct modification *modifications;

    int num_files;
    struct file *files;
};


/**
 * Devuelve el path completo de CVS_DIR
 */
char* base_path(void);


/**
 * Devuelve el path completo del repositorio
 */
char* repo_path(void);


/**
 * Función de comparación de `files` para usar con bsearch, etc
 */
int files_cmp(const void *file1, const void *file2);


/**
 * Función de conparación de `files` y `subfiles` (archivos hijos)
 */
int sub_files_cmp(const void *name, const void *file);


/**
 * Función de comparación de `modifications` para usar con bsearch, etc
 */
int modifications_cmp(const void *mod1, const void *mod2);


/**
 * Lee un archivo de versión del cliente
 *
 * @return estructura del archivo
 */
struct info_file* read_client_file(void);


/**
 * Escribe un archivo de versión del cliente
 *
 * @param info_file  estructura del archivo
 */
void write_client_file(struct info_file *info_file);


/**
 * Lee un archivo de versión del servidor
 *
 * @param version  qué versión abrir, si es -1 abre la última
 *
 * @return estructura del archivo
 */
struct info_file* read_server_file(int version);


/**
 * Escribe un archivo de versión del servidor
 *
 * @param info_file  estructura del archivo
 */
void write_server_file(struct info_file *info_file);


/**
 * Libera recursos usados por un info_file
 *
 * @param info_file
 */
void free_info_file(struct info_file *info_file);


#endif /* _CVS_FILES_H_ */

