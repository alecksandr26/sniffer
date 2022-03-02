/* Here I import my header file */
#include "../include/package.h"

/* defineProtocol: Is a function to define which protocol we are going to use */
union Protocol defineProtocol (Ether *ether, byte *data)
{
	union Protocol p;	
	switch (ether->etherType) {
	case ARP_TYPE:
		p.arp = ArpPackage(data);
		break;
	case IPV4_TYPE: /* We want to read the complete package */
		p.ipv4 = Ipv4Package(data, false);
		break;
	}

	return p;
}

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

/* printPackage: This function will all the package */
void printPackage (Pak *p)
{
	p->ether->print(p->ether);
	printProtocol(p);
}

/* Package: This is the constructor we need the data*/
struct Package *Package (byte *data, unsigned short dataLength)
{
	/* Here I create my object */
	struct Package *p = (struct Package *) malloc(sizeof(struct Package));
	
	p->ether = Ethernet(data, dataLength);
	p->protocol = defineProtocol(p->ether, p->ether->data);
	
	p->print = &printPackage;
	
	/* Here I return the object */
	return p;
}
