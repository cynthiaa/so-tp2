Formato de los archivos de información de versión del servidor:
---------------------------------------------------------------

```
número_de_versión
id_del_próximo_archivo_a_crear
cantidad_de_líneas_de_modificaciones
modificación_1
...
modifcación_n
cantidad_de_archivos
archivo_1
...
archivo_n
```

Las líneas de modificación son de la forma:

```
nombre_del_archivo última_versión id operación [opciones]
```

donde las operaciones son:
* `A`: alter file
* `D`: delete file
* `M`: move file
* `N`: new file

Move (`M`) lleva como parámetro el nuevo nombre.

Las líneas de archivo son de la forma:

```
nombre_del_archivo última_versión id
```


Formato de los archivos de información de versión del cliente:
--------------------------------------------------------------

```
número_de_versión
cantidad_de_líneas_de_modificaciones
modificación_1
...
modifcación_n
cantidad_de_archivos
archivo_1
...
archivo_n
```

Las líneas de modificación son de la forma:

```
nombre_del_archivo operación [opciones]
```

donde las operaciones son las mismas que para el servidor.

Las líneas de archivo son de la forma:

```
nombre_del_archivo
```


Estructura de datos en el servidor:
-----------------------------------

Por cada archivo hay una carpeta con nombre igual al id del archivo, que contiene la versión del archivo, cada vez que fue modificado, con nombre el número de versión.

Además, hay una carpeta llamada `info` que contiene un archivo con la información de versión de nombre el número de versión. Finalmente, hay un link simbólico llamado `current` apuntando al último archivo de información de versión.


Detalles:
---------

* Al hacer un revert, se pierde la historia de ese archivo, es decir, `cvs version archivo` no mostrará las versiones anteriores si el archivo fue borrado o movido.

* Al igual que la mayoría de los CVSs, solo se almacenan archivos, es decir, las carpetas vacias son tratadas como inexistentes.

* El `cvs add`, `cvs delete`, `cvs mv` son recursivos, es decir, aceptan carpetas. El resto de los comandos no lo es.

* La lista de archivos de cada archivo de versión tiene (implícitamente) el árbol de archivos del file system: cada archivo es una hoja, y su path es el camino de la raíz a la hoja (root-to-leaf path).

* El `cvs update` no soporta "tree conflicts".
