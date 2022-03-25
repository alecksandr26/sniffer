#ifndef __ICMPV6_H_
#define __ICMPV6_H_

#include <stdlib.h>
#include <string.h>
#include "../helpers.h"

struct neighborAdvertisement {
	byte flags;
	byte *targetAddress; /* 16 bytes */
};

struct routerAdvertisement {
	byte curHopLimit;
	byte flags;
	byte *lifetime; /* 2 bytes | seconds */
	byte *reachableTime; /* 4 bytes | mili seconds */
	byte *retransTimer; /* 4 bytes | mili seconds */
};

struct neighborSolicitation {
	byte *targetAddress; /* 16 bytes */
};

struct redirectMessage {
	byte *targetAddress; /* 16 bytes */
	byte *destinationAddress; /* 16 bytes */
};

union NDP {
	struct neighborAdvertisement *na;
	struct routerAdvertisement *ra;
	struct neighborSolicitation *ns;
	struct redirectMessage *rm;
};

enum TYPE_ICMPV6 {
	UNREACHABLE_ICMPV6,
	TIME_EXCEEDED_ICMPV6,
	ECHO_REQUEST_ICMPV6,
	ECHO_REPLY_ICMPV6,
	NEIGHBOR_ADVERTISEMENT_ICMPV6,
	ROUTER_ADVERTISEMENT_ICMPV6,
	ROUTER_SOLICITATION_ICMPV6,
	NEIGHBOR_SOLICITATION_ICMPV6,
	REDIRECT_MESSAGE_ICMPV6,
    UNASSIGNED_ICMPV6
};

struct Icmpv6 {
	byte type;
	byte code;
	byte *checkSum; /* 2 bytes */
	byte *messageBody; /* The rest of the header */
	byte *data; /* To poin to the next data */

	/* if it is a ping or pong */
	byte *identifier; /* 2 bytes */
	byte *sequenceNumber; /* 2 bytes */

	/* If we need to read again the ipv6 header */
	bool extraPackage;
	void *dataIpv6Package;
	void *(*Ipv6Package) (byte *data, bool justHeader);
	
	enum TYPE_ICMPV6 typeNum;
	union NDP ndp; /* To select the protocol */
	
	void (*print) (struct Icmpv6 *icmpv6);
};


/* Icmpv6Packge: To catch the icmpv6 package */
struct Icmpv6 *Icmpv6Package (byte *data, unsigned short length, void *(*Ipv6Package)(byte *data, bool justHeader));

typedef struct Icmpv6 Icmpv6;

#endif
