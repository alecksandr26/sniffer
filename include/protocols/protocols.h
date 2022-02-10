


#ifndef __PROTOCOLS_H_
#define __PROTOCOLS_H_

#include "arp.h"

/* Here I'm going to use some functions to know which protocol we are going to read */
#include "../ethernet/ethernet.h"


/* This is the union that we are going to use to create our arp */
union Protocols {
	Arp *arp;
};


/* defineprotocol: Is a function to define which protocol we are going to use */
union Protocols defineProtocol (Ether *ether, byte *data)
{
	union Protocols p;
	switch (ether->etherType) {
		case ARP_TYPE:
		    p.arp = ArpPackage(data, ether);
			break;
	}

	return p;
}


#endif

