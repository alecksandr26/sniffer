/* Here I include this thing */
#include "../../include/protocols/arp.h"



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
	data += 2;

	memcpy(a->protocol, data, 2);
	data += 4;
	
	memcpy(a->byteOfRequestReply, data, 2);
	data += 2;

	memcpy(a->macSender, data, 6);
	data += 6;
	
	memcpy(a->ipv4Sender, data, 4);
	data += 4;

	memcpy(a->macTarget, data, 6);
	data += 6;

	memcpy(a->ipv4Target, data, 4);
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
	
	printf("%s IP Address: ", type);
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
	puts("| ARP |\n");
	printHardWareType(a->hardwareType);
	printf("Protocol Type: ");
	printHex(a->protocol, 1);
	printf(" (%s)\n", ETHER_TYPE_STRING[a->protocolType]);
	
	printArpRequestType(a->request);
	printMacAddress(a->macTarget, false, "Target");
	printIpv4(a->ipv4Target, "Target");
	printMacAddress(a->macSender, false, "Sender");
	printIpv4(a->ipv4Sender, "Sender");
	puts("---------------------------------------");
	
}




/* AprPackage: This function will create all our arp structure  */
struct Arp *ArpPackage (byte *data)
{
	struct Arp *a = (struct Arp *) malloc(sizeof(struct Arp));

	a->hardwareType = (byte *) malloc(2);
	a->protocol = (byte *) malloc(2);
	a->byteOfRequestReply = (byte *) malloc(2);
	
	
	/* Here we commit the error */
	a->macSender = (byte *) malloc(6); /* Catch the mac address */
	a->ipv4Sender = (byte *) malloc(4);
	
	/* Here we commit the error */
	a->macTarget = (byte *) malloc(6); /* Catch the mac address */
	a->ipv4Target = (byte *) malloc(4);
	
	readDataArp(data, a);
   
	a->protocolType = knowEtherType(a->protocol);
	
	a->request = defineOperationRequest(a->byteOfRequestReply);

	a->print = &printArpData;
	
	return a;
}




