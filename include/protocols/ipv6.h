#ifndef __IPV6_H_
#define __IPV6_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "protocolstransport.h"
#include "../helpers.h"

struct Ipv6 {
	/* This is the protocol type */
	enum PROTOCOL_TRANSPORT protocolType;
	bool justHeader;

	/* Here I save the protocol object */
	union ProtocolTransport protocolData;

	/* print: To print the protocol */
	void (*print) (struct Ipv6 *ip);
};

/* Ipv6Package: To create a ipv6 package */
struct Ipv6 *Ipv6Package (byte *data, bool justHeader);

typedef struct Ipv6 Ipv6;

#endif
