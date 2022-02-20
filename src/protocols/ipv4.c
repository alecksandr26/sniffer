
#include "../../include/protocols/ipv4.h"

/* flipData: Here we flip the bytes from data types  */
byte *flipData (byte *data, unsigned l)
{
	int i; /* index */
	byte *temp = (byte *) malloc(l);

	for (i = 0; i < l; ++i)
		*(temp + (l - 1) - i) = *(data + i);

	free(data);
   
	return temp;
}

/* knowIpv4Protocol: Know which ipv4 protocol we are going to use */
enum IPV4_PROTOCOL knowIpv4Protocol (byte p)
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
}

/* printIpv4flags: To print the flags from the package of ipv4 */
void printIpv4Flags (byte f)
{
	switch (f) {
		case 1:
			puts("\tReserved");
			break;
		case 2:
			puts("\tDon't Fragment");
			break;
		case 4:
			puts("\tMore Fragments");
			break;
	}
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
	printf("\nFlag: (0x0%x)\n", i->flags);
	printIpv4Flags(i->flags);
	printf("\nFragment Offset: (%u)\n", *((unsigned short *) i->offset));
	
	printf("Time To Live: (%u)\n", i->TTL);
	printf("Protocol: (%s)\n", IPV4_PROTOCOLS[i->protocolType]);
	printf("Header Checksum: (");
	printHex(i->checkSum, 1);
	puts(")\n");
	printIpv4(i->srcIpv4, "Source");
	printIpv4(i->desIpv4, "Destination");
	puts("---------------------------------------");
}

/* Ipv4package: This function will create the ipv4 object */
struct Ipv4 *Ipv4Package (byte *data)
{
	struct Ipv4 *i = (struct Ipv4 *) malloc(sizeof(struct Ipv4));

	i->length = (byte *) malloc(2);
	i->id = (byte *) malloc(2);
	i->offset = (byte *) malloc(2);
	i->checkSum = (byte *) malloc(2);
	i->srcIpv4 = (byte *) malloc(4);
	i->desIpv4 = (byte *) malloc(4);
	i->options = (byte *) malloc(40);

	readDataIpv4(i, data);
	
	i->print = &printIpv4Protocol;
	
	return i;
}


