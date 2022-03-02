#ifndef __PACKAGE_H_
#define __PACKAGE_H_

/* std Libreries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "protocols/arp.h"
#include "protocols/ipv4.h"

#include "ethernet.h"
#include "helpers.h"

/* This is the union that we are going to use to define the type of protocol */
union Protocol {
	Arp *arp;
	Ipv4 *ipv4;
};

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




