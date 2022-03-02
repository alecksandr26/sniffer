#ifndef __PROTOCOLS_TRANSPORT_H_
#define __PROTOCOLS_TRANSPORT_H_

#include <stdio.h>

#include "./icmp.h"

enum PROTOCOL_TRANSPORT {
	TCP,
	UDP,
	ICMP
};

union ProtocolTransport {
	Icmp *icmp;
};

char *PROTOCOL_TRANSPORT_STRING[] = { "TCP", "UDP", "ICMP" };

/* printipv4protocoltransport: Prints the protocol inside of ipv4 */
void printIpv4ProtocolTransport(union ProtocolTransport p, enum PROTOCOL_TRANSPORT pt)
{
	switch (pt) {
	case TCP:
		break;
	case UDP:
		break;
	case ICMP:
		p.icmp->print(p.icmp);
		break;
	}
}

/* defineprotocoltransport: To define the protocol */
union ProtocolTransport defineProtocolTransport (enum PROTOCOL_TRANSPORT protocol, byte *data, unsigned short length)
{
	union ProtocolTransport p;
	switch (protocol) {
	case TCP:
		break;
	case UDP:
		break;
	case ICMP:
		p.icmp = IcmpPackage(data, length);
		break;
	}
	
	return p;
}

#endif
