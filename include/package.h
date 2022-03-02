#ifndef __PACKAGE_H_
#define __PACKAGE_H_

/* std Libreries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "protocols/protocols.h"
#include "ethernet.h"

#ifndef __BYTE_DATA_
#define __BYTE_DATA_
/* This is necessary to create the buffers */
typedef unsigned char byte;

#endif

/* This class will mantine the data */
struct Package {
	/* To catch the protocol */
	union Protocol protocol;
	
	/* The ethernet object */
	Ether *ether;
	
	/* print: To print the protocol that we receive */
	void (*print) (struct Package *package);	
};

/* Package: This is the constructor we need the data*/
struct Package *Package (byte *data, unsigned short length);

typedef struct Package Pak;

#endif




