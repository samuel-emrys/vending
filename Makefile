########################################################################
# Author        : Samuel Dowling
########################################################################

SOURCES=src/ppd_main.c src/ppd_menu.c src/ppd_options.c src/ppd_utility.c \
src/ppd_stock.c src/ppd_coin.c
HEADERS=includes/ppd_main.h includes/ppd_menu.h includes/ppd_options.h \
includes/ppd_utility.h includes/ppd_stock.h includes/ppd_coin.h
README=README.md
MAKEFILE=Makefile
CC=gcc
CFLAGS=-ansi -Wall -pedantic

all:ppd

ppd:
	$(CC) $(CFLAGS) $(SOURCES) -o bin/ppd

debug:
	$(CC) $(CFLAGS) -g $(SOURCES) -o bin/ppd

archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) $(README) $(MAKEFILE)

.PHONY:clean
clean:
	rm -f *.o bin/ppd bin/*
