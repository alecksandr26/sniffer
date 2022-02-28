#ifndef __IPV4_H_
#define __IPV4_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef __BYTE_DATA_
#define __BYTE_DATA_
/* This is necessary to create the buffers */
typedef unsigned char byte;
#endif

enum IPV4_PROTOCOL {
	TCP,
	UDP,
	ICMP
};

char *IPV4_PROTOCOLS[] = { "TCP", "UDP", "ICMP" };

struct Ipv4 {
	byte version; /* 4 bits */
	byte IHL; /* 4 bits */
	byte TOS;
	byte *length; 
	byte *id; /* 2 bytes */
	byte flags; /* 3 bits */
	byte *offset; /* 13 bits */
	byte TTL;
	byte protocol;
	enum IPV4_PROTOCOL protocolType;
	bool extraProtocol;
	byte *checkSum;
	
	byte *srcIpv4;
	byte *desIpv4;
	
	byte *options;
	byte *data; /* will points to the rest of bytes */

	/* print: To print all the protocol */
	void (*print) (struct Ipv4 *ip);
};

struct Ipv4 *Ipv4Package (byte *data);

/* printIpv4: To print a ipv4 */
void printIpv4 (byte *ipv4, char *type);

typedef struct Ipv4 Ipv4;

#endif
