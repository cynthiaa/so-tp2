CC = gcc
CFLAGS = -Wall --std=c99

TARGETDIR = bin
SRCDIR = src

all: 
	$(CC) $(CFLAGS) $(SRCDIR)/func.c $(SRCDIR)/front.c -o $(TARGETDIR)/cvs

clean:
	@rm -rf $(TARGETDIR)/*