#ifndef __PROTOCOLS_TRANSPORT_H_
#define __PROTOCOLS_TRANSPORT_H_

#include <stdio.h>

#include "icmp.h"
#include "icmpv6.h"
#include "udp.h"
#include "tcp.h"

enum PROTOCOL_TRANSPORT {
	TCP,
	UDP,
	ICMP,
	ICMPV6
};

union ProtocolTransport {
	Tcp *tcp;
	Icmp *icmp;
	Icmpv6 *icmpv6;
    Udp *udp;
};

/* printLayer4ProtocolTransport: Prints the protocol inside of ipv4 */
void printLayer4ProtocolTransport (union ProtocolTransport p, enum PROTOCOL_TRANSPORT pt);

void deconstructProtocolTransport (union ProtocolTransport p, enum PROTOCOL_TRANSPORT pt);

/* defineprotocoltransport: To define the protocol */
union ProtocolTransport defineProtocolTransport (enum PROTOCOL_TRANSPORT protocol, byte *data, unsigned short length, void *(*IpvPackage)(byte *data, bool justHeader));

#endif
