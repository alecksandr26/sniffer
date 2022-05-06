#ifndef __FILE_H_
#define __FILE_H_

/* std file */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* this is the struct file */

/* The maximun size of a ethernet package */
#define BUFSIZE 1518


struct File {
    unsigned char data[BUFSIZE];
	char name[100];
	unsigned length;
	
	/* seeHex: prints the info in hex */
	void (*seeHex) (struct File *file);

	/* deconstructor: To free all the memory */
	void (*deconstructor) (struct File *file);
};



/* File: Here I create the contructor that creates the file */
struct File *File (char *fileName);


/* Here we define the object name */
typedef struct File F;

#endif




