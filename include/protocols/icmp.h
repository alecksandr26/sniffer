#ifndef __ICMP_H_
#define __ICMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../helpers.h"

enum TYPE_ICMP {
	ECHO_REPLY,
	UNREACHABLE,
	REDIRECT_MESSAGE,
	ECHO_REQUEST
};

struct Icmp {
	unsigned short length;
	byte type;
	enum TYPE_ICMP typeI;
	byte code;
	byte *checkSum; /* 2 bytes */
	byte *identifier; /* 2 bytes */
	byte *sequenceNumber; /* 2 bytes */
	byte *ipv4; /* 4 bytes */
	byte *data;
	bool extraIpv4Package;
	
	/* print: To print this protocol */
	void (*print) (struct Icmp *icmp);
};

/* IcmpPackage: To create the icmp object */
struct Icmp *IcmpPackage (byte *data, unsigned short lenght);

typedef struct Icmp Icmp;

#endif
