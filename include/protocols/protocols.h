


#ifndef __PROTOCOLS_H_
#define __PROTOCOLS_H_

#include "arp.h"
#include "ipv4.h"

/* Here I'm going to use some functions to know which protocol we are going to read */
#include "../ethernet/ethernet.h"


/* This is the union that we are going to use to create our arp */
union Protocols {
	Arp *arp;
	Ipv4 *ipv4;
};


/* defineprotocol: Is a function to define which protocol we are going to use */
union Protocols defineProtocol (Ether *ether, byte *data)
{
	union Protocols p;
	switch (ether->etherType) {
		case ARP_TYPE:
		    p.arp = ArpPackage(data);
			break;
		case IPV4_TYPE:
			p.ipv4 = Ipv4Package(data);
			break;
	}

	return p;
}


#endif

