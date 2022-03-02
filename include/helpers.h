#ifndef __HELPERS_H_
#define __HELPERS_H_

/* This is a dependency with some extra usefull functions */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


#ifndef __BYTE_DATA_
#define __BYTE_DATA_
/* This is necessary to create the buffers */
typedef unsigned char byte;
#endif

/*   printHex: This function will print in hex some buffer */
void printHex (byte *hex, int l);

/* printMacAddress: Print the MacAdddress inside of the package */
void printMacAddress (byte *address, bool broadCast, char *macName);

/* flipData: Here we flip the bytes from data types  */
byte *flipData (byte *data, unsigned l);

/* printIpv4: To print the Ipv4 */
void printIpv4 (byte *ipv4, char *type);

#endif
