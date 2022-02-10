

#ifndef __ARP_H_
#define __ARP_H_

/* To be able to pass the ethernet package */
#include "../ethernet/ethernet.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __BYTE_DATA_
#define __BYTE_DATA_
/* This is necessary to create the buffers */
typedef unsigned char byte;
#endif


enum REQUEST_REPLY {
	REQUEST,
	REPLY
};


/* struct Arp will contains the data from arp */
struct Arp {
	enum REQUEST_REPLY request;
	Ether *e;
	byte *hardwareType;
	byte *byteOfRequestReply;
	byte *ipv4Source;
	byte *macSource;
	byte *ipv4Destination;
	byte *macDestination;

	/* To print the data from the pacakge */
	void (*print) (struct Arp *a);
};


/* AprPackage: This function will create all our arp structure */
struct Arp *ArpPackage (byte *data, Ether *ethernet);


typedef struct Arp Arp;


#endif

