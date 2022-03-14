#include "../../include/protocols/ipv4.h"

char *PROTOCOL_TRANSPORT_STRING[] = { "TCP", "UDP", "ICMP" };

/* knowIpv4Protocol: Know which ipv4 protocol we are going to use */
enum PROTOCOL_TRANSPORT knowIpv4Protocol (byte p)
{
	switch (p) {
	    case 1:
			return ICMP;
		case 6:
			return TCP;
		case 17:
			return UDP;
	}
}

/* readDataIpv4: This function will read all the data from a ethernet package */
void readDataIpv4 (struct Ipv4 *i, byte *data)
{
	memcpy(&(i->version), data, 1); /* version */
	i->version &= 0b11110000;
	i->version >>= 4;
	
	memcpy(&(i->IHL), data, 1); /* Header length */
	i->IHL &= 0b00001111;
	data++;

	memcpy(&(i->TOS), data, 1); /* Type of service */
	data++;

	memcpy(i->length, data, 2); /* Total length */
	i->length = flipData(i->length, 2);
	data += 2;

	memcpy(i->id, data, 2); /* identification */
	i->id = flipData(i->id, 2);
	data += 2;

	memcpy(&(i->flags), data, 1); /* The flags of the package */
	i->flags &= 0b11100000;
	i->flags >>= 5;
   
	memcpy(i->offset, data, 2); /* The offset from the package */
	*((unsigned short *) i->offset) &= 0b1111111100011111;
	i->offset = flipData(i->offset, 2);
	data += 2;
	
	memcpy(&(i->TTL), data, 1); /* Time To Live */
	data++;

	memcpy(&(i->protocol), data, 1); /* The protocol ipv4 */
	i->protocolType = knowIpv4Protocol(i->protocol);
	data++;

	memcpy(i->checkSum, data, 2); /* The check sum */
	data += 2;

	memcpy(i->srcIpv4, data, 4); /* The source ipv4 */
	data += 4;

	memcpy(i->desIpv4, data, 4); /* The destination ipv4 */
	data += 4;

	if (i->IHL > 5)
		i->data = data; /* After reading the whole data we put the rest here */
	else
		i->data = data + ((i->IHL - 5) * 4);
}

/* printIpv4flags: To print the flags from the package of ipv4 */
void printIpv4Flags (byte f)
{
	if (!(f & 0b00000001))
		puts("\t0. bit = 0 (Reserved)");
	
	if (f & 0b00000010)
		puts("\t1. bit = 1 (Not divisible)");
	else
		puts("\t1. bit = 0 (Divisible)");
	
	if (f & 0b00000100)
		puts("\t2. bit = 1 (Intermediate Fragment)");
	else
		puts("\t2. bit = 0 (Last Fragment)");
}


/* printIpv4: To prin the ipv4 program */
void printIpv4Protocol (struct Ipv4 *i)
{
	puts("---------------------------------------");
	puts("| IPV4 |\n");
	printf("Version: (%u)\n", i->version);
	printf("Internet Header length: (%u) bytes\n", i->IHL * 4);
	printf("Type Of Service: (%u)\n", i->TOS);
	printf("Total length: (%u) bytes\n", *((unsigned short *) i->length));
	printf("Identification: (%u)\n", *((unsigned short *) i->id));
	puts("\nFlags: ");
	printIpv4Flags(i->flags);
	printf("\nFragment Offset: (%u)\n", *((unsigned short *) i->offset));
	
	printf("Time To Live: (%u)\n", i->TTL);
	printf("Protocol: (%s)\n", PROTOCOL_TRANSPORT_STRING[i->protocolType]);
	printf("Header Checksum: (");
	printHex(i->checkSum, 1);
	puts(")\n");
	printIpv4(i->srcIpv4, "Source");
	printIpv4(i->desIpv4, "Destination");
	puts("---------------------------------------");
	/* Here print the rest of the procotol */
	if (!i->justHeader) {
		printIpv4ProtocolTransport(i->protocolData, i->protocolType);
		/* Here we print the ipv4 package inside of the icmp package */
		if (i->protocolType == ICMP && i->protocolData.icmp->extraIpv4Package)
			printIpv4Protocol((struct Ipv4 *) i->protocolData.icmp->ipv4Package);
	}
}

/* Ipv4package: This function will create the ipv4 object */
struct Ipv4 *Ipv4Package (byte *data, bool justHeader)
{
	struct Ipv4 *i = (struct Ipv4 *) malloc(sizeof(struct Ipv4));
	unsigned short l;

	i->length = (byte *) malloc(2);
	i->id = (byte *) malloc(2);
	i->offset = (byte *) malloc(2);
	i->checkSum = (byte *) malloc(2);
	i->srcIpv4 = (byte *) malloc(4);
	i->desIpv4 = (byte *) malloc(4);
	i->options = (byte *) malloc(40);
	i->justHeader = justHeader; /* if we want to print just the header */
	
	readDataIpv4(i, data);
	if (i->IHL < 6)
		l = (*((unsigned short *) i->length)) - 20;
	else
		l = (*((unsigned short *) i->length)) - (4 * (i->IHL - 5));
	
	/* Here we define the protocol that we have */
	if (!justHeader)
		i->protocolData = defineProtocolTransport(i->protocolType, i->data, l, Ipv4Package);
	
	i->print = &printIpv4Protocol;
	
	return i;
}
