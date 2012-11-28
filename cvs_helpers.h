#ifndef _CVS_HELPERS_H_
#define _CVS_HELPERS_H_

#include <stdlib.h>
#include <stdbool.h>


/**
 * Máxima longitud de un path
 */
#ifndef MAX_PATH_LENGTH
#define MAX_PATH_LENGTH  512
#endif


/**
 * Imprime un error y sale de la aplicación
 *
 * @param fmt  mismos parámetros que el printf
 */
void cvs_error(char *fmt, ...);


/**
 * Aloca memoria, chequeando errores
 *
 * @param size  tamaño de memoria pedido
 *
 * @return puntero a la memoria
 */
void* cvs_malloc(size_t size);


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
 *
 * @return el archivo
 */
FILE* open_file(char *flags, char *fmt, ...);


/**
 * Crea las carpetas necesarias para guardar un archivo
 *
 * @param fmt  mismos parámetros que el printf
 */
void create_path(char *fmt, ...);


/**
 * Busca un archivo en el directorio actual o alguno de sus ancestros
 *
 * @param path  buffer de salida
 * @param file  nombre del archivo
 *
 * @return true si encontró el archivo, false sino
 */
bool find_file_in_parents(char *path, char *file);


/**
 * Chequea la existencia de un archivo
 *
 * @param fmt  mismos parámetros que el printf
 *
 * @return true si el archivo existe, false sino
 */
bool file_exists(char *fmt, ...);


/**
 * Chequea si un path dado pertenece a un directorio
 *
 * @param file  nombre del archivo
 *
 * @return true si el path es un directorio existente, false sino
 */
bool is_directory(char *file);


/**
 * Extraer el nombre del archivo/carpeta de un path
 *
 * @param path  path del archivo/carpeta
 *
 * @return nombre del archivo/carpeta
 */
char* file_name(char *path);


#endif /* _CVS_HELPERS_H_ */

