#ifndef __IPV4_H_
#define __IPV4_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "protocolstransport.h"
#include "../helpers/helpers.h"

struct Ipv4 { /* 20 bytes */
	byte version; /* 4 bits */
	byte IHL; /* 4 bits */
	byte TOS;
	byte length[2]; 
	byte id[2]; /* identification 2 bytes */
	byte flags; /* 3 bits */
	byte offset[2]; /* 13 bits */
	byte TTL;
	byte protocol;
	enum PROTOCOL_TRANSPORT protocolType;
	byte checkSum[2];
	
	byte srcIpv4[4];
	byte desIpv4[4];
	
	byte options[40];
	byte *data; /* will points to the rest of bytes */
	bool justHeader;

	union ProtocolTransport protocolData;
	
	/* print: To print all the protocol */
	void (*print) (struct Ipv4 *ip);
    void (*deconstruct) (struct Ipv4 *ip);
};

/* justHeader If only we want to print the header or all the package */
struct Ipv4 *Ipv4Package (byte *data, bool justHeader);

typedef struct Ipv4 Ipv4;

#endif
