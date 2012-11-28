#ifndef _CVS_CORE_H_
#define _CVS_CORE_H_


/**
 * Formatea el servidor
 */
int cvs_format(int argc, char **argv);


/**
 * Crea una copia local de la última versión del repositorio
 */
int cvs_checkout(int argc, char **argv);


/**
 * Marca archivos o carpetas para agregar en el próximo commit
 *
 * @param file1  nombre del archivo o carpeta
 * @param file2  ...
 *               ...
 */
int cvs_add(int argc, char **argv);


/**
 * Marca archivos o carpetas para borrar en el próximo commit
 *
 * @param file1  nombre del archivo o carpeta
 * @param file2  ...
 *               ...
 */
int cvs_delete(int argc, char **argv);


/**
 * Marca archivos o carpetas para renombrar en el próximo commit
 *
 * @param old_path1  viejo nombre
 * @param old_path2  ...
 *                   ...
 * @param new_path   nuevo nombre (si hay más de un old_path, debe ser una carpeta)
 */
int cvs_mv(int argc, char **argv);


/**
 * Hace un commit del repositorio local al servidor
 */
int cvs_commit(int argc, char **argv);


/**
 * Actualiza el repositorio local
 */
int cvs_update(int argc, char **argv);


/**
 * Muestra en qué versiones fue modificado un archivo
 *
 * @param file  nombre del archivo
 */
int cvs_version(int argc, char **argv);


/**
 * Muestra el diff de un archivo con una versión previa del mismo
 *
 * @param file     nombre del archivo
 * @param version  número de versión
 */
int cvs_diff(int argc, char **argv);


/**
 * Reemplaza un archivo con una versión previa del mismo
 *
 * @param file     nombre del archivo
 * @param version  número de versión
 */
int cvs_revert(int argc, char **argv);


/**
 * Muestra los archivos borrados desde una versión dada
 *
 * @param version  número de versión
 */
int cvs_erased(int argc, char **argv);


/**
 * Imprime ayuda
 *
 * @param comando  nombre del comando [opcional]
 */
int cvs_help(int argc, char **argv);


#endif /* _CVS_CORE_H_ */

