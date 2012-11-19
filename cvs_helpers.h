#ifndef _CVS_HELPERS_H_
#define _CVS_HELPERS_H_

#include <stdbool.h>


/**
 * Máxima longitud de un comando de bash
 */
#define CMD_LENGTH   512


/**
 * Máxima longitud de un path
 */
#define MAX_PATH_LENGTH  512


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
 * Expande con bash un path dado
 *
 * @param name  buffer de salida
 * @param fmt   mismos parámetros que el printf
 */
void expand_path(char *name, char *fmt, ...);


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
 * Busca un archivo en el directorio actual o alguno de sus ancestros
 *
 * @param path  buffer de salida
 * @param file  nombre del archivo
 *
 * @return true si encontró el archivo, false sino
 */
bool find_file_in_parents(char *path, char *file);


#endif /* _CVS_HELPERS_H_ */

