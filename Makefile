# Copyright 2019 Darius Neatu <neatudarius@gmail.com>

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=archiver

build: $(TARGETS)

archiver: archiver.c
	$(CC) $(CFLAGS) archiver.c -o archiver

pack:
	zip -FSr 314CA_TudoracheBogdanMihai_Tema3.zip README Makefile *.c *.h *.pdf

run: archiver.c
	./archiver

clean:
	rm -f $(TARGETS)
