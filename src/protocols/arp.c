
/* Here I include this thing */
#include "../../include/protocols/arp.h"

/* To include the macaddress */
#include "../../include/ethernet/ethernet.h"
/* To include the print functions */
#include "../../include/ethernet/pethernet.h"

/* defineOperationrequest: To know which operation the arp package is doing */
enum REQUEST_REPLY defineOperationRequest (byte *request)
{
	switch (*((short *) request)) {
		case 256:
			return REQUEST;
		case 512:
			return REPLY;
	}
}


/* readDataArp: This data will give you our data */
void readDataArp (byte *data, struct Arp *a)
{
	memcpy(a->hardwareType, data, 2);
	data += 6;

	memcpy(a->byteOfRequestReply, data, 2);
	data += 8;
	
	memcpy(a->ipv4Source, data, 4);
	data += 10;

	memcpy(a->ipv4Destination, data, 4);
}

/* printHardWareType: To print which hardware type we have */
void printHardWareType (byte *type)
{
	printf("Hardware type: ");
    printHex(type, 1);
	printf(" (Ethernet)\n");
}

/* printArprequesttype: To print which type of arp was */
void printArpRequestType (enum REQUEST_REPLY r)
{
	printf("Operation: ARP ");
	switch (r) {
		case REQUEST:
			printf("Request (1)\n");
			break;
		case REPLY:
			printf("Reply (2)\n");
			break;
	}
}


/* printIpv4: To print the Ipv4 */
void printIpv4 (byte *ipv4, char *type)
{
	int i; /* index */
	
	printf("IP Address %s: ", type);
	for (i = 0; i < 4; ++i) {
		if (i == 3)
			printf("%u\n", *(ipv4 + i));
		else
			printf("%u.", *(ipv4 + i));
	}
}

/* printArpdata: This function will print all the data */
void printArpData (struct Arp *a)
{
	puts("---------------------------------------");
	printHardWareType(a->hardwareType);
	printf("Ethernet Type: %s (", ETHER_TYPE_STRING[a->e->etherType]);
	printHex(a->e->ethernetTypeBytes, 1);
	puts(")");
	printArpRequestType(a->request);
	printMacAddress(a->macDestination, a->e->broadCastDes, "Dest");
	printIpv4(a->ipv4Destination, "Dest");
	printMacAddress(a->macSource, a->e->broadCastDes, "Src");
	printIpv4(a->ipv4Source, "Src");
	puts("---------------------------------------");
	
}




/* AprPackage: This function will create all our arp structure  */
struct Arp *ArpPackage (byte *data, Ether *e)
{
	struct Arp *a = (struct Arp *) malloc(sizeof(struct Arp));

	a->e = e; /* Here we get the object that we need */
	a->hardwareType = (byte *) malloc(2);
	a->byteOfRequestReply = (byte *) malloc(2);
	a->ipv4Destination = (byte *) malloc(4);
	a->macSource = e->macaddressSor; /* Catch the mac address */
	a->ipv4Source = (byte *) malloc(4);
	a->macDestination = e->macaddressDes; /* Catch the mac address */

	readDataArp(data, a);

	a->request = defineOperationRequest(a->byteOfRequestReply);

	a->print = &printArpData;
	
	return a;
}




