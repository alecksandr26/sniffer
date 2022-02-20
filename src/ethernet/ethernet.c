

/* Here we are going to include ethernet.h */
#include "../../include/ethernet/ethernet.h"
#include "../../include/ethernet/pethernet.h"

/* And these are the print functions */

/* mulOrUni: To know if it is multicast or unicast mac address */
enum UNI_MULTI mulOrUni (byte *data)
{
	if (*(data + 0) % 2 == 0)
		return UNI_CAST;

	return MULTI_CAST;
}

/* globOrLoca: To know if it is a gloabl or local mac address */
enum GLOB_LOCA globOrLoca (byte *data)
{
	if (*(data + 0) % 4 == 0)
		return GLOBALLY_UNIQUE;

	return LOCALLY_ADMIN;
}

/* printTypeOfMacAddress: Here we are going to print what type of mac adress is */
void printTypeOfMacAddress (enum UNI_MULTI uniMul, enum GLOB_LOCA globLo)
{
	switch (uniMul) {
		case UNI_CAST:
			printf("\tUnicast\n");
			break;
		case MULTI_CAST:
			printf("\tMulticast\n");
			break;
	}

	switch (globLo) {
		case GLOBALLY_UNIQUE:
			printf("\tGlobal and unique\n");
			break;
		case LOCALLY_ADMIN:
			printf("\tLocal and administrate\n");
			break;
	}
}

/* printDataEthernet: Prints the data of the package using the whole functions of print */
void printDataEthernet (struct EtherPackage *e)
{
	puts("---------------------------------------");
	puts("| ETHERNET |\n");
	
	/* print the ether type */
	printf("Ethernet Type: ");
	printHex(e->ethernetTypeBytes, 1);
	printf(" (%s)\n", ETHER_TYPE_STRING[e->etherType]);
	
	printMacAddress(e->macaddressDes, e->broadCastDes, "Dest");
	/* To know what type of mac address is */
	printTypeOfMacAddress(mulOrUni(e->macaddressDes),
						  globOrLoca(e->macaddressDes));

	printMacAddress(e->macaddressSor, e->broadCastSor, "Src");
	/* To know what type of mac address is */
	printTypeOfMacAddress(mulOrUni(e->macaddressSor),
						  globOrLoca(e->macaddressSor));
	
	printf("\nPackage Size: (%i)\n", e->length);

	/* Here we print the frame check */
	printf("Frame Check Secure: (");
	printHex(e->frameCheck, 3);
	puts(")");
	puts("---------------------------------------");
}

/* These are the core functions */

/* isBroadCast: If the mac address is broadCast */
bool isBroadCast (byte *data)
{
	int i; /* index */
	unsigned sum;

	sum = 0;
	for (i = 0; i < 6; ++i)
		sum += *(data + i);

	return (sum == (255 * 6)) ? true : false;
}

/* readDataEthernet: To load all the data to the ethernet package */
void readDataEthernet (struct EtherPackage *e, byte *data)
{
	/* Read the mac address dest */
    memcpy(e->macaddressDes, data, sizeof(byte) * 6);
	data += 6; /* move 6 steps */

	/* Read the mac address source */
    memcpy(e->macaddressSor, data, sizeof(byte) * 6);
	data += 6; /* move 6 steps */
	
	/* Read the ethernet type */
    memcpy(e->ethernetTypeBytes, data, sizeof(byte) * 2);
	data += 2; /* move 2 steps */

	/* Read the data */
	memcpy(e->data, data, sizeof(byte) * e->length);
	data += e->length;
	
	/* Read the secure check */
	memcpy(e->frameCheck, data, sizeof(byte) * 4);


	/* Try to know if the mac address is broadcast */
	e->broadCastDes = isBroadCast(e->macaddressDes);
	
	/* Try to know if the mac address is broadcast */
	e->broadCastSor = isBroadCast(e->macaddressSor);
}

/* knowEtherType: Is a function to know the ether type */
enum ETHER_TYPES knowEtherType (byte *data)
{
	int i; /* index */
	
	for (i = 0; i < AMOUNT_ETHER_TYPES; ++i)
		if (*((unsigned short *) data) == ETHER_TYPE_DECMIAL[i])
			return i;
			
	puts("Error: Uknow ethernet type");
	exit(EXIT_FAILURE);
}

/* EtherPackage: Now here I load the data of the ethernet package */
struct EtherPackage *Ethernet (byte *data, unsigned short length)
{
	struct EtherPackage *e = (struct EtherPackage *) malloc(sizeof(struct EtherPackage));

	/* Here I load the data */
	e->length = length;
	e->ethernetTypeBytes = (byte *) malloc(2);
	e->macaddressDes = (byte *) malloc(6);
	e->macaddressSor = (byte *) malloc(6);
	e->frameCheck = (byte *) malloc(4);
	e->data = (byte *) malloc(length);

	readDataEthernet(e, data);

	/* To know which package we were receiving */
	e->etherType = knowEtherType(e->ethernetTypeBytes);
	
	/* printDataEthernet: We are going to print the data as ethernet */
    e->print = &printDataEthernet;
	
	/* return ether package */
	return e;
}
