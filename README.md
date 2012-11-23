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
* _A_: alter file
* _D_: delete file
* _M_: move file
* _N_: new file

Move (_M_) lleva como parámetro el nuevo nombre.

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

Además, hay una carpeta llamada _info_ que contiene un archivo con la información de versión de nombre el número de versión. Finalmente, hay un link simbólico llamado _current_ apuntando al último archivo de información de versión.

