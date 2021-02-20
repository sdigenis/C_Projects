CC = gcc
COMMENTS = 	-Wall -g

all:	archive	dirlist	main	unarchive	encrypt

archive:	archive.c	mysystem_check.c
	$(CC) $(COMMENTS) archive.c -o archive

dirlist:	dirlist.c	mysystem_check.c
	$(CC) $(COMMENTS) dirlist.c -o dirlist

main:	main.c	mysystem_check.c
	$(CC) $(COMMENTS) main.c -o main

unarchive:	unarchive.c	mysystem_check.c
	$(CC) $(COMMENTS) unarchive.c -o unarchive

encrypt:	encrypt.c mysystem_check.c
	$(CC) $(COMMENTS) encrypt.c -o encrypt

clean:
	rm encrypt	main	archive	unarchive	dirlist


