#ifndef __IPV4_H_
#define __IPV4_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "./protocolstransport.h"
#include "../helpers.h"

struct Ipv4 { /* 20 bytes */
	byte version; /* 4 bits */
	byte IHL; /* 4 bits */
	byte TOS;
	byte *length; 
	byte *id; /* 2 bytes */
	byte flags; /* 3 bits */
	byte *offset; /* 13 bits */
	byte TTL;
	byte protocol;
	enum PROTOCOL_TRANSPORT protocolType;
	byte *checkSum;
	
	byte *srcIpv4;
	byte *desIpv4;
	
	byte *options;
	byte *data; /* will points to the rest of bytes */

	union ProtocolTransport protocolData;
	
	/* print: To print all the protocol */
	void (*print) (struct Ipv4 *ip);
};

struct Ipv4 *Ipv4Package (byte *data);

typedef struct Ipv4 Ipv4;

#endif
