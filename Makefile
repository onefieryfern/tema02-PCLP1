# Copyright 2025 Andrei N. FLOREA
# andrei.florea1311@stud.acs.pub.ro

CC=gcc
CFLAGS=-std=c99 -lc -Wall -Wextra -Werror

all: build

build: battleships

battleships: battleships.c logging.c memory.c types.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f battleships

pack:
	zip -FSr 313CA_FloreaAndrei_Tema2.zip README Makefile *.c *.h

.PHONY: clean pack
