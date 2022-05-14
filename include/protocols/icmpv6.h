#ifndef __ICMPV6_H_
#define __ICMPV6_H_

#include <stdlib.h>
#include <string.h>
#include "../helpers/helpers.h"

struct Option {
	byte type;
	int typeOption;
	byte len;
	struct Option *next;
	byte macAddress[6]; /* 6 bytes */
	byte mtu[4]; /* 4 bytes */

	/* for the prefix information */
	byte prefixLength;
	byte prefixFlags;
	unsigned validLifeTime;
	unsigned preferredLifeTime;
	byte ipv6[16];
	
};

enum TYPE_OPTIONS_ICMPV6 {
	NONCE_ICMPV6,
	SOURCE_LINK_LAYER_ADDRESS_ICMPV6,
	TARGET_LINK_LAYER_ADDRESS_ICMPV6,
	PREFIX_INFORMATION_ICMPV6,
	REDIRECT_HEADER_ICMPV6,
	REDIRECT_MTU_ICMPV6,
	RECURSIVE_DNS_SERVER_ICMPV6
};

struct NDP {
	/* neighborAdvertisement */
	byte flags;
	byte targetAddress[16]; /* 16 bytes */
	
	/* routerAdvertisement */
	byte curHopLimit;
	byte lifetime[2]; /* 2 bytes | seconds */
	byte reachableTime[2]; /* 4 bytes | mili seconds */
	byte retransTimer[2]; /* 4 bytes | mili seconds */

	/* redirectMessage */
	byte destinationAddress[16]; /* 16 bytes */
    
    /* This is lnked list */
	struct Option *tail;
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
	VERSION_2_MULTICAST_ICMPV6,
    UNASSIGNED_ICMPV6
};

struct Icmpv6 {
	byte type;
	byte code;
	byte checkSum[2]; /* 2 bytes */
	byte *messageBody; /* The rest of the header */
	byte *data; /* To poin to the next data */

	/* if it is a ping or pong */
	byte identifier[2]; /* 2 bytes */
	byte sequenceNumber[4]; /* 4 bytes */

	/* If we need to read again the ipv6 header */
	bool extraPackage;
	void *dataIpv6Package;
	void *(*Ipv6Package) (byte *data, bool justHeader);
	
	enum TYPE_ICMPV6 typeNum;
	struct NDP *ndp; /* To select the protocol */
	
	void (*print) (struct Icmpv6 *icmpv6);
    void (*deconstruct) (struct Icmpv6 *icmpv6);
};


/* Icmpv6Packge: To catch the icmpv6 package */
struct Icmpv6 *Icmpv6Package (byte *data, unsigned short length, void *(*Ipv6Package)(byte *data, bool justHeader));

typedef struct Icmpv6 Icmpv6;

#endif
