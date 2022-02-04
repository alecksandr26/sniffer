


#ifndef __FILE_H_
#define __FILE_H_


/* this is the struct file */

struct File {
	char *name;
	unsigned char *data;
	unsigned length;
	
	/* seeHex: prints the info in hex */
	void (*seeHex) (struct File *file);
};



/* File: Here I create the contructor that creates the file */
struct File *File (char *fileName);


/* Here we define the object name */
typedef struct File F;

#endif




