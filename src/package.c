
/* Here I import my header file */
#include "../include/package.h"


/* printProtocol: This function will print the protocol */
void printProtocol (Pak *package)
{
	switch (package->ether->etherType) {
		case ARP_TYPE: /* To print arp protocol */
			package->protocol.arp->print(package->protocol.arp);
			break;
	    case IPV4_TYPE: /* To prin ipv4 prototcol */
			package->protocol.ipv4->print(package->protocol.ipv4);
			break;
	}
}


/* Package: This is the constructor we need the data*/
struct Package *Package (byte *data, unsigned short dataLength)
{
	/* Here I create my object */
	struct Package *p = (struct Package *) malloc(sizeof(struct Package));
	
	p->ether = Ethernet(data, dataLength);
	p->protocol = defineProtocol(p->ether, p->ether->data);
	
	p->print = &printProtocol;
	p->printEthernet = p->ether->print;
	
	/* Here I return the object */
	return p;
}
