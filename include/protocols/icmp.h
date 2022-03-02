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
	ECHO_REQUEST,
	TIME_EXCEEDED
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
	byte *nextHopMTU; /* 4 bytes */
	byte *ipv4Data; /* 20 bytes */
	void *ipv4Package; /* 20 bytes */
	
	/* print: To print this protocol */
	void (*print) (struct Icmp *icmp);
};

/* IcmpPackage: To create the icmp object */
struct Icmp *IcmpPackage (byte *data, unsigned short lenght, void *(*Ipv4Package)(byte *data, bool justHeader));

typedef struct Icmp Icmp;

#endif
