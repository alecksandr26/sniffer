/* Here we include the header file */
#include "../include/file.h"

/* std file */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* The maximun size of a ethernet package */
#define BUFSIZE 1518

/* readFile: This function will read the whole file */
void readFile (struct File *f)
{
	int fd, n;    /* file descriptor | transferred bytes */

	/* Give memory to data */
	f->data = (char *) malloc(BUFSIZE);
	
	/* Open the file */
	if ((fd = open(f->name, O_RDONLY, 0)) == -1) {
		fprintf(stderr, "file: error opening the file %s: %s\n", f->name, strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Read the file */
	if ((n = read(fd, f->data, BUFSIZE)) < 0) {
		fprintf(stderr, "file: error reading the file %s: %s\n", f->name, strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Copy the amount of transferred bytes */
	f->length = n;
}

/* seeHex: prints the info in hex | unsiged */
void seeHex (struct File *f)
{
	int i; /* index */

	for (i = 0; i < f->length; ++i)
		printf("%u. | hex: %x | uns: %u\n", i+1, *(f->data + i), *(f->data + i));
}

/* deconstructor: To free all the memory */
void deconstructor (struct File *f)
{
    free(f->data);
	free(f);
}

/* File: Here I create the contructor that creates the file */
struct File *File (char *fileName)
{
	struct File *f = (struct File *) malloc(sizeof(struct File));

	/* Here I declare the attributes */
	f->name = fileName;

	/* Here link the method */
	f->seeHex = &seeHex;
	f->deconstructor = &deconstructor;
	
	readFile(f);

	return f;
}



