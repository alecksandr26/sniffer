#ifndef __ARP_H_
#define __ARP_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __BYTE_DATA_
#define __BYTE_DATA_
/* This is necessary to create the buffers */
typedef unsigned char byte;
#endif

/* The amount types */
#define AMOUNT_TYPES 4

unsigned short PROTOCOL_TYPES_DECIMAL[AMOUNT_TYPES] = { 1544, 8, 129,	56710 };
char *PROTOCOL_TYPES_STRING[AMOUNT_TYPES] = { "ARP", "IPV4", "VLAN", "IPV6" };

/* The types of ethernet */
enum PROTOCOL_TYPES {
	ARP,
    IPV4,
    VLAN,
    IPV6
};

enum REQUEST_REPLY {
	REQUEST,
	REPLY
};

/* struct Arp will contains the data from arp */
struct Arp {
	enum REQUEST_REPLY request;
	enum PROTOCOL_TYPES protocolType;
	byte *hardwareLength;
	byte *protocolLength;
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

typedef struct Arp Arp;

#endif
