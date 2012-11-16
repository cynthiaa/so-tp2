#ifndef _CVS_BACK_H_
#define _CVS_BACK_H_

/* Tiene que terminar en / */
#define CVS_DIR "/var/cvs/"

int cvs_checkout(int argc, char **argv);
int cvs_add     (int argc, char **argv);
int cvs_delete  (int argc, char **argv);
int cvs_update  (int argc, char **argv);
int cvs_commit  (int argc, char **argv);
int cvs_diff    (int argc, char **argv);
int cvs_version (int argc, char **argv);
int cvs_revert  (int argc, char **argv);
int cvs_mv      (int argc, char **argv);
int cvs_format  (int argc, char **argv);

/* Faltaria un comando para ver archivos eliminados */

#endif /* _CVS_BACK_H_ */
