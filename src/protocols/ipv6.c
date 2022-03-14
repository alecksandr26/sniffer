#include "../../include/protocols/ipv6.h"

/* readIpv6protocol: To read all the data from the package */
void readIpv6Protocol (struct Ipv6 *i, byte *data)
{
	
}

/* printIpv6protocol: To print the the protocol */
void printIpv6Protocol (struct Ipv6 *i)
{
	printf("Hello how are you?\n");
}


/* Ipv6Package: To create ipv6 package */
struct Ipv6 *Ipv6Package (byte *data, bool justHeader)
{
	struct Ipv6 *i = (struct Ipv6 *) malloc(sizeof(struct Ipv6));

	
	i->justHeader = justHeader;
	i->print = &printIpv6Protocol;
	
	return i;
}
