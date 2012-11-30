CC     = gcc
CFLAGS = -std=gnu99 -pedantic -Wall -Wextra -Wno-unused-function

.PHONY = all cvs demo clean

all: cvs demo

cvs: cvs_helpers.c cvs_files.c cvs_core.c cvs.c
	$(CC) $(CFLAGS) -I . -I def_files/ -o cvs cvs_helpers.c cvs_files.c cvs_core.c cvs.c

demo: cvs demo.c
	$(CC) $(CFLAGS) -o demo demo.c

clean:
	@rm -f cvs demo

