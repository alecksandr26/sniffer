
/* Here I import my header file */
#include "../include/package.h"


/* std libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


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




/* readData: Here I read the data from the object and fill all the attributes */
void readData (struct Package *p, byte *data)
{
	/* Read the mac address dest */
    memcpy(p->macaddressDes, data, sizeof(byte) * 6);
	data += 6; /* move 6 steps */

	/* Try to know if the mac address is broadcast */
	p->broadCastDes = isBroadCast(p->macaddressDes);
	
	/* Read the mac address source */
    memcpy(p->macaddressSor, data, sizeof(byte) * 6);
	data += 6; /* move 6 steps */

	/* Try to know if the mac address is broadcast */
	p->broadCastSor = isBroadCast(p->macaddressSor);
	
	/* Read the ethernet type */
    memcpy((byte *) p->ethernetTypeBytes, data, sizeof(byte) * 2);
	p->ethernetTypeBytes = (unsigned short *) flipData((byte *) p->ethernetTypeBytes, 2);
	data += 2; /* move 2 steps */

	/* Read the data */
	memcpy(p->data, data, sizeof(byte) * p->length);
	data += p->length;
	
	/* Read the secure check */
	memcpy((byte *) p->frameCheck, data, sizeof(byte) * 4);
	p->frameCheck = (unsigned *) flipData((byte *) p->frameCheck, 4);

	/* With this we finish to read the data */
}



/* knowEtherType: Is a function to know which package we were receiving */
void knowEtherType (struct Package *p)
{
	switch (*p->ethernetTypeBytes) {
		case 2054:
			p->etherType = ARP_TYPE;
			break;
		case 2048:
			p->etherType = IPV4_TYPE;
			break;
	   	case 33024:
			p->etherType = VLAN_TYPE;
			break;
		case 34525:
			p->etherType = IPV6_TYPE;
			break;
	}
}


/* mulOrUni: To know if it is multicast or unicast */
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


/* PRINT FUNCTIONS */

/* printEtherType: Print the Ether type */
void printEtherType (struct Package *p)
{
   	/* Ethernet type */
	switch (p->etherType) {
		case ARP_TYPE:
			printf("Ethernet Type: ARP \n");
			break;
			
		case IPV4_TYPE:
			printf("Ethernet Type: IPV4 \n");
			break;
			
		case VLAN_TYPE:
			printf("Ethernet Type: VLAN \n");
			break;
			
	    case IPV6_TYPE:
			printf("Ethernet Type: IPV6 \n");
			break;
	}
}



/* printTypeOfMacAddress: Here we are going to print what type of mac adress is */
void printTypeOfMacAddress (enum UNI_MULTI uniMul, enum GLOB_LOCA globLo)
{
	switch (uniMul) {
		case UNI_CAST:
			printf("\tUnicast MacAddress\n");
			break;
		case MULTI_CAST:
			printf("\tMulticast MacAddress\n");
			break;
	}

	switch (globLo) {
		case GLOBALLY_UNIQUE:
			printf("\tGlobal and unique MacAddress\n");
			break;
		case LOCALLY_ADMIN:
			printf("\tLocal and administrate MacAddress\n");
			break;
	}
}



/* printMacAddress: Print the MacAdddress inside of the package */
void printMacAddress (byte *address, bool broadCast, char *macName)
{
	int i; /* index */
	
	enum UNI_MULTI uniMul;
	enum GLOB_LOCA globLo;
	
	/* Print mac address */
	printf("\nMacAddress %s: ", macName);
	for (i = 0; i < 5; ++i)
		printf("%x;", *(address + i));
	if (broadCast)
		printf("%x BroadCast\n", *(address));
	else
		printf("%x\n", *(address));
	
	uniMul = mulOrUni(address);
	globLo = globOrLoca(address);

	/* To know what type of mac address is */
	printTypeOfMacAddress(uniMul, globLo);
}




/* printData: Prints the data of the package using the whole functions of print */
void printData (struct Package *p)
{
	
	puts("---------------------------------------");
	
	printEtherType(p);
	printMacAddress(p->macaddressDes, p->broadCastDes, "Destination");
	printMacAddress(p->macaddressSor, p->broadCastSor, "Source");
	
	printf("\nPackage Size: %i\n", p->length);
	printf("Frame Check Secure: %u\n", *p->frameCheck);
	
	puts("---------------------------------------");
}




/* Package: This is the constructor we need the data*/
struct Package *Package (byte *data, unsigned short length)
{
	/* Here I create my object */
	struct Package *p = (struct Package *) malloc(sizeof(struct Package));

	/* Here I pass the method */
	p->printData = &printData;
	
	/* Here I pass the amount of data */
	p->length = length;

	/* Here I give memory to the attributes */
	p->macaddressDes = (byte *) malloc(6);
	p->macaddressSor = (byte *) malloc(6);
	p->ethernetTypeBytes = (unsigned short *) malloc(2);
	p->data = (byte *) malloc(p->length);
	p->frameCheck = (unsigned *) malloc(4);

	/* Here we read the pack of data */
	readData(p, data);

	/* To know which package we were receiving */
	knowEtherType(p);

	/* Here I return the object */
	return p;
}
