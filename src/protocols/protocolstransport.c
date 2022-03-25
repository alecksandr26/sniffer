#include "../../include/protocols/protocolstransport.h"

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
		
	case ICMPV6:
		p.icmpv6->print(p.icmpv6);
		break;
	}
}

/* defineprotocoltransport: To define the protocol */
union ProtocolTransport defineProtocolTransport (enum PROTOCOL_TRANSPORT protocol, byte *data, unsigned short length, void *(*IpvPackage)(byte *data, bool justHeader))
{
	union ProtocolTransport p;
	
	switch (protocol) {
	case TCP:
		break;
	case UDP:
		break;
	case ICMP: /* Here we pass the function to create an ipv4 package */
		p.icmp = IcmpPackage(data, length, IpvPackage);
		break;
	case ICMPV6:
		p.icmpv6 = Icmpv6Package(data, length, IpvPackage);
		break;
	}
	
	return p;
}
