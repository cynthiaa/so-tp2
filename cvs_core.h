#ifndef _CVS_CORE_H_
#define _CVS_CORE_H_

/**
 * Directorio del servidor
 *
 * @warning Tiene que terminar en /
 */
#define CVS_DIR  "/var/cvs/"


/**
 * Formatea el servidor
 */
int cvs_format(int argc, char **argv);


/**
 * Crea una copia local de la última versión del repositorio
 */
int cvs_checkout(int argc, char **argv);


/**
 * Marca un archivo para agregar en el próximo commit
 *
 * @param file  nombre del archivo
 */
int cvs_add(int argc, char **argv);


/**
 * Marca un archivo para borrar en el próximo commit
 *
 * @param file  nombre del archivo
 */
int cvs_delete(int argc, char **argv);


/**
 * Marca un archivo para renombrar en el próximo commit
 *
 * @param old_path  viejo nombre
 * @param new_patg  nuevo nombre
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
 * Muestra en qué versiones fue modificado el archivo
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
 * Imprime ayuda
 *
 * @param comando  nombre del comando [opcional]
 */
int cvs_help(int argc, char **argv);

/* TODO: Faltaria un comando para ver archivos eliminados */


#endif /* _CVS_CORE_H_ */

