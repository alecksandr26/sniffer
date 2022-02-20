

#ifndef __ARP_H_
#define __ARP_H_

/* To use some extra functions that this depency has */
#include "../ethernet/ethernet.h"
#include "../ethernet/pethernet.h"

#include <stdbool.h>
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
	enum ETHER_TYPES protocolType;
	byte *hardwareType;
	byte *protocol;
	byte *byteOfRequestReply;
	byte *ipv4Sender;
	byte *macSender;
	byte *ipv4Target;
	byte *macTarget;

	/* To print the data from the pacakge */
	void (*print) (struct Arp *a);
};


/* AprPackage: This function will create all our arp structure */
struct Arp *ArpPackage (byte *data);

/* Some extra functions */

/* printIpv4: To print a ipv4 */
void printIpv4 (byte *ipv4, char *type);

typedef struct Arp Arp;


#endif

