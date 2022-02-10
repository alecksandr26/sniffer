
/* This is an small module to have just a few extra functions */


#ifndef __PETHERNET_H_
#define __PETHERNET_H_

#include <stdbool.h>
#include <stdio.h>

#ifndef __BYTE_DATA_
#define __BYTE_DATA_
/* This is necessary to create the buffers */
typedef unsigned char byte;
#endif


/*   printHex: This function will print in hex some buffer */
void printHex (byte *hex, int l);


/* printMacAddress: Print the MacAdddress inside of the package */
void printMacAddress (byte *address, bool broadCast, char *macName);


#endif



