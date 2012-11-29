#include <stdio.h>
#include <stdlib.h>


int main(void) {

    // FILE* file= open_file("w", "~/salida_demo");

    // system("chmod  777 ~/salida_demo");

    // fprintf(file,"\n\nDemostración de comandos implementados para el cvs\n");
    // fprintf(file,"---------------------------------------------------------\n");

    // fprintf(file,"\n\nCreando un nuevo repositorio....\n");

    // system("./cvs format");
    // system("./cvs checkout");

    // printf("\nArchivos que se encuentran en el directorio inicialmete:\n\n");

    // system("find ~/.cvs | sed -e 's;[^/]*/;|-;g;s;--|; |;g' > ~/salida_demo");

    // fprintf(file,"---------------------------------------------------------\n");

    // system("touch Archivo1");
    // system("mkdir Carpeta");
    // system("./cvs add Archivo1");
    // system("./cvs add Carpeta");

    // fprintf(file,"Se han agregado el archivo Archivo1 y la carpeta Carpeta al cvs\n");

    // system("find ~/.cvs | sed -e 's;[^/]*/;|-;g;s;--|; |;g' > ~/salida_demo");


    printf("\n\nDemostración de comandos implementados para el cvs\n");
    printf("---------------------------------------------------------\n");

    printf("\n\nCreando un nuevo repositorio....\n");


    system("./cvs format");
    system("./cvs checkout");

    printf("\nArchivos que se encuentran en el repositorio inicialmete:\n\n");

    system("find ~/.cvs | sed -e 's;[^/]*/;|-;g;s;--|; |;g' ");

    printf("---------------------------------------------------------\n");

    printf("\nCreando el archivo Archivo1 en la carpeta Repo ......\n");


    system("mkdir Repo");
    system("touch ./Repo/Archivo1");

    system("./cvs add Repo");
    system("./cvs add Repo/Archivo1");
    system("./cvs commit");


    printf("\n\n Se han agregado el archivo Archivo1 y la carpeta Repo al cvs\n\n");

    system("find ./Repo | sed -e 's;[^/]*/;|-;g;s;--|; |;g'");

    printf("\n\n El repositorio ya tiene su primer versión\n\n");

    system("find ~/.cvs | sed -e 's;[^/]*/;|-;g;s;--|; |;g' ");

    system("touch ./Repo/Archivo2");
    system("./cvs add Repo/Archivo2");
    system("./cvs commit");

    printf("\n\n Se han agregado el archivo Archivo2 a la carpeta Repo y se ha commiteado\n\n");

    system("find ./Repo | sed -e 's;[^/]*/;|-;g;s;--|; |;g'");

    printf("\n\n El repositorio ya tiene su segunda versión\n\n");

    system("find ~/.cvs | sed -e 's;[^/]*/;|-;g;s;--|; |;g' ");

    return 0;
}

