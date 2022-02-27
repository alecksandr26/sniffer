#include "../../include/ethernet/pethernet.h"

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
	printf("\nMAC Address %s: ", macName);
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
