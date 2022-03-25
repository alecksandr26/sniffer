#ifndef __PROTOCOLS_TRANSPORT_H_
#define __PROTOCOLS_TRANSPORT_H_

#include <stdio.h>

#include "icmp.h"
#include "icmpv6.h"

enum PROTOCOL_TRANSPORT {
	TCP,
	UDP,
	ICMP,
	ICMPV6
};

union ProtocolTransport {
	Icmp *icmp;
	Icmpv6 *icmpv6;
};

/* printipv4protocoltransport: Prints the protocol inside of ipv4 */
void printIpv4ProtocolTransport (union ProtocolTransport p, enum PROTOCOL_TRANSPORT pt);

/* defineprotocoltransport: To define the protocol */
union ProtocolTransport defineProtocolTransport (enum PROTOCOL_TRANSPORT protocol, byte *data, unsigned short length, void *(*IpvPackage)(byte *data, bool justHeader));

#endif
