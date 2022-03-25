#ifndef __IPV6_H_
#define __IPV6_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "protocolstransport.h"
#include "../helpers.h"

struct headerNode {
	byte nextHeader;
	byte headerExtensionLength;
	unsigned headerIndex;
};

struct Ipv6 {
	/* To create the linked list */
	struct linkedList *l;
	
	/* This is the protocol type */
	enum PROTOCOL_TRANSPORT protocolType;
	bool justHeader;

	/* Here I save the protocol object */
	union ProtocolTransport protocolData;

	byte version;
	byte trafficClass;
	byte *flowLabel; /* 3 bytes */
	byte *payloadLength; /* 2 bytes */
	byte nextHeader;
	byte hopLimit;
	byte *sourceAddress; /* 16 bytes */
	byte *destinationAddress; /* 16 bytes */

	int headerIndex; /* This is the index to the */
	bool headerList;

	byte *data; /* The data byte pointer */
	
	/* print: To print the protocol */
	void (*print) (struct Ipv6 *ip);
};

/* Ipv6Package: To create a ipv6 package */
struct Ipv6 *Ipv6Package (byte *data, bool justHeader);

typedef struct Ipv6 Ipv6;

#endif
