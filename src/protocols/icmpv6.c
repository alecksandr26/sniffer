#include "../../include/protocols/icmpv6.h"

char *TYPE_ICMPV6_STRING[] = {
	"Destination Unreachable",
	"Time Exceeded",
	"Echo request",
	"Echo Reply",
	"Neighbor Advertisement",
	"Router Advertisement",
	"Router Solicitation",
	"Neighbor Solicitation",
	"Redirect Message",
	"Unassigned"
};

/* knowTypeOfIcmpv6: To know the icmpv6 protocol */
enum TYPE_ICMPV6 knowTypeOfIcmpv6 (byte type)
{
	switch (type) {
	case 1:
		return UNREACHABLE_ICMPV6;
	case 3:
		return TIME_EXCEEDED_ICMPV6;
	case 128:
		return ECHO_REQUEST_ICMPV6;
	case 129:
		return ECHO_REPLY_ICMPV6;
	case 136:
		return NEIGHBOR_ADVERTISEMENT_ICMPV6;
	case 134:
		return ROUTER_ADVERTISEMENT_ICMPV6;
	case 133:
		return ROUTER_SOLICITATION_ICMPV6;
	case 135:
		return NEIGHBOR_SOLICITATION_ICMPV6;
	case 137:
		return REDIRECT_MESSAGE_ICMPV6;
	default:
		return UNASSIGNED_ICMPV6;
	}
}

/* neighborAdverstisementReadData: To read the data of the protocol */
struct neighborAdvertisement *neighborAdverstisementReadData (byte *data)
{
	struct neighborAdvertisement *na = (struct neighborAdvertisement *) malloc(sizeof(struct neighborAdvertisement));
	/* flags */
	memcpy(&(na->flags), data, 1);
	data += 4;

	na->targetAddress = (byte *) malloc(16); /* 16 bytes */

	/* The target address */
	memcpy(na->targetAddress, data, 16);
	data += 16;
		
	return na;
}

/* routerAdvertisementReadData: To read the data of the router advertisement header */
struct routerAdvertisement *routerAdvertisementReadData (byte *data)
{
	struct routerAdvertisement *ra = (struct routerAdvertisement *) malloc(sizeof(struct routerAdvertisement));
	
	/* cur hop limit */
	memcpy(&(ra->curHopLimit), data, 1);
	data++;

	/* the flags */
	memcpy(&(ra->flags), data, 1);
	data++;
	
	/* the life time */
	ra->lifetime = (byte *) malloc(sizeof(2));
	memcpy(ra->lifetime, data, 2);
	ra->lifetime = flipData(ra->lifetime, 2);
	data += 2;
	
	/* reachabletime */
	ra->reachableTime = (byte *) malloc(sizeof(4));
	memcpy(ra->reachableTime, data, 4);
	ra->reachableTime = flipData(ra->reachableTime, 4);
	data += 4;
	
	/* retrasTimer */
	ra->retransTimer = (byte *) malloc(sizeof(4));
	memcpy(ra->retransTimer, data, 4);
	ra->retransTimer = flipData(ra->retransTimer, 4);
	data += 4;
	
	return ra;
}

/* neighborSolicitationReadData: To read the header */
struct neighborSolicitation *neighborSolicitationReadData (byte *data)
{
	struct neighborSolicitation *ns = (struct neighborSolicitation *) malloc(sizeof(struct neighborSolicitation));

	ns->targetAddress = (byte *) malloc(sizeof(16));
	
	data += 4;
	
	memcpy(ns->targetAddress, data, 16);
	data += 16;
	
	return ns;
}

struct redirectMessage *redirectMessageReadData (byte *data)
{
	struct redirectMessage *rm = (struct redirectMessage *) malloc(sizeof(struct redirectMessage));

	data += 4;

	rm->destinationAddress = (byte *) malloc(16);
	rm->targetAddress = (byte *) malloc(16);

	memcpy(rm->targetAddress, data, 16);
	data += 16;
	
	memcpy(rm->destinationAddress, data, 16);
	data += 16;
	
	return rm;
}

void knowAndReadNDPProtocol (byte *data, struct Icmpv6 *i)
{
	switch (i->type) {
	case 136:
	    i->ndp.na = neighborAdverstisementReadData(data);
		i->data += 17;
		break;
	case 134:
		i->ndp.ra = routerAdvertisementReadData(data);
		i->data += 12;
		break;
	case 135:
		i->ndp.ns = neighborSolicitationReadData(data);
		i->data += 20;
		break;
	case 137:
		i->ndp.rm = redirectMessageReadData(data);
		i->data += 36;
		break;
	}
}

/* readIcmpv6Package: To read the icmpv6 package */
void readIcmpv6Package (byte *data, struct Icmpv6 *i)
{
	memcpy(&(i->type), data, 1);
	i->typeNum = knowTypeOfIcmpv6(i->type);
	data++;

	memcpy(&(i->code), data, 1);
	data++;

	memcpy(i->checkSum, data, 2);
	data += 2;

	if (i->type > 132 && i->type < 138)
		knowAndReadNDPProtocol(data, i);
	else if (i->type == 128 || i->type == 129) {
		memcpy(i->identifier, data, 2);
		i->identifier = flipData(i->identifier, 2);
		data += 2;
		memcpy(i->sequenceNumber, data, 2);
		i->sequenceNumber = flipData(i->sequenceNumber, 2);
		data += 2;
		i->data = data;
	} else if (i->type == 1) { /* To read the ipv6 package */
		i->extraPackage = true;
		data += 4;
		i->dataIpv6Package = i->Ipv6Package(data, true);
		i->data = data;
	
	} else if (i->type == 3) {
		i->extraPackage = true;
		data += 4;
		i->dataIpv6Package = i->Ipv6Package(data, true);
		i->data = data;
	}
}

/* printCodeIcmpv6: To print the code */
char *stringCodeIcmpv6 (int type, byte code)
{
	switch (type) {
	case UNREACHABLE_ICMPV6:
		switch (code) {
		case 0:
			return "no route to destination";
		case 1:
			return "communication with destination administratively prohibited";
		case 2:
			return "beyond scope of source address";
		case 3:
			return "address unreachable";
		case 4:
			return "port unreachable";
		case 5:
			return "source address failed ingress/egress policy";
		case 6:
			return "reject route to destination";
		case 7:
			return "Error in Source Routing Header";
			
		}
	case TIME_EXCEEDED_ICMPV6:
		switch (code) {
		case 0:
			return "hop limit exceeded in transit";
		case 1:
			return "fragment reassembly time exceeded";
		}
	default:
		return "";
	}
}

/* printNeighborAdvertisement: To print an na protocol */
void printNeighborAdvertisement (struct neighborAdvertisement *na)
{	
	/* Flags */
	puts("Flags: ");
	if (na->flags & 0b10000000)
		puts("\t1. bit 1 Sended by Router");
	else
		puts("\t1. bit 0 Sended by Host");

	if (na->flags & 0b01000000)
		puts("\t2. bit 1 A Neighbor Solicitation");
	else
		puts("\t2. bit 0 Not A Neighbor Solicitation");

	if (na->flags & 0b0010000)
		puts("\t3. bit 1 Override Cache");
	else
		puts("\t3. bit 0 Not Override Cache");

	printIpv6(na->targetAddress, "Target Address");
}

/* printRouterAdverstisement: To print the router advertisement header */
void printRouterAdverstisement (struct routerAdvertisement *ra)
{
    printf("Cur Hop Limit: (%u)\n", ra->curHopLimit);
	puts("Flags:");
	if (ra->flags & 0b1000000)
		puts("\t1. bit 1 Managed Address Configuration");
	else
		puts("\t1. bit 0 Managed Address Configuration");

	if (ra->flags & 0b0100000)
		puts("\t2. bit 1 Other Stateful Configuration");
	else
		puts("\t2. bit 0 Other Stateful Configuration");

	printf("Life Time: (%u) Seconds\n", *((unsigned short *) ra->lifetime));
	printf("Reachable Time: (%u) MilliSeconds\n", *((unsigned *) ra->reachableTime));
	printf("Retrans Timer: (%u) MilliSeconds\n", *((unsigned *) ra->retransTimer));
}

/* printNeighborSolicitation: To print the neighbor solicitation */
void printNeighborSolicitation (struct neighborSolicitation *ns)
{
	printIpv6(ns->targetAddress, "Target Address");
}

/* printRedirectMessage: To print the redirect message */
void printRedirectMessage (struct redirectMessage *rm)
{
	printIpv6(rm->targetAddress, "Target Address");
	printIpv6(rm->destinationAddress, "Destination Address");
}

/* printNDPProtocol: To prin the ndp protocol */
void printNDPProtocol (union NDP ndp, byte type)
{
    switch (type) {
	case 136:
	    printNeighborAdvertisement(ndp.na);
		break;
	case 134:
	    printRouterAdverstisement(ndp.ra);
		break;
	case 135:
		printNeighborSolicitation(ndp.ns);
		break;
	case 137:
		printRedirectMessage(ndp.rm);
		break;
    }
}

/* printIcmpv6: To print the icmp */
void printIcmpv6 (struct Icmpv6 *i)
{
	puts("---------------------------------------");
	puts("| Icmpv6 |\n");
	printf("Type: (%u) (%s)\n", i->type, TYPE_ICMPV6_STRING[i->typeNum]);
	printf("Code: (%u) (%s)\n", i->code, stringCodeIcmpv6(i->typeNum, i->code));
	printf("Check Sum: (");
	printHex(i->checkSum, 1);
	puts(")");
	if (i->type > 132 && i->type < 138)
		printNDPProtocol(i->ndp, i->type);
	else if (i->type == 128 || i->type == 129) {
		printf("Identifier: (%u)\n", *((unsigned short *) i->identifier));
		printf("Sequence Number: (%u)\n", *((unsigned short *) i->sequenceNumber));
	} else if (i->type == 1) {
		
	}
		
	puts("---------------------------------------");
}

/* Icmpv6Packge: To catch the icmpv6 package */
struct Icmpv6 *Icmpv6Package (byte *data, unsigned short length, void *(*Ipv6Package)(byte *data, bool justHeader))
{
	struct Icmpv6 *i = (struct Icmpv6 *) malloc(sizeof(struct Icmpv6));
	
	i->extraPackage = false;
	i->checkSum = (byte *) malloc(2);
	i->identifier = (byte *) malloc(4);
	i->sequenceNumber = (byte *) malloc(4);
	i->Ipv6Package = Ipv6Package;
	
	readIcmpv6Package(data, i);
	
	i->print = &printIcmpv6;
	
	return i;
}


