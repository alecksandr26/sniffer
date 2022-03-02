#include "../include/helpers.h"

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
/*   printHex: This function will print in hex some buffer */
void printHex (byte *hex, int l)
{
	int i; /* index */
	
	printf("0x");
	for (i = 0; i < l; ++i) {
		if (*(hex + i) < 16)
			printf("0");
		printf("%x", *(hex + i));
	}
    if (*(hex + i) < 16)
		printf("0");
	printf("%x", *(hex + i));
}

/* printMacAddress: Print the MacAdddress inside of the package */
void printMacAddress (byte *address, bool broadCast, char *macName)
{
	int i; /* index */
	
	/* Print mac address */
	printf("MAC Address %s: ", macName);
	for (i = 0; i < 5; ++i) {
		if (*(address + i) < 16)
			printf("0");
		printf("%x:", *(address + i));
	}

	if (broadCast) {
		if (*(address + i) < 16)
			printf("0");
		printf("%x", *(address + i));
		printf(" Broadcast\n");
	}
	else {
		if (*(address + i) < 16)
			printf("0");
		printf("%x\n", *(address + i));
	}
}
