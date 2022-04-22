#ifndef __HELPERS_H_
#define __HELPERS_H_

/* This is a dependency with some extra usefull functions */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

struct node {
	void *data;
	struct node *next;
	struct node *prev;
};

struct linkedList {
	struct node *head;
	struct node *tail;
	unsigned len;
};

#ifndef __BYTE_DATA_
#define __BYTE_DATA_
/* This is necessary to create the buffers */
typedef unsigned char byte;
#endif

/* initlinkedlist: To create a linked list */
void initLinkedList (struct linkedList *i);

/* pushlinkedlist: To push one element inside of the list */
void pushLinkedList (struct linkedList *i, void *data);

/* deleteElement: To delete one element from the linked list */
void deleteElementLinkedList (struct linkedList *i, void *data);

/* printDataInHex: To print a bunch of data in hex */
void printDataInHex (byte *data, unsigned l);

/*   printHex: This function will print in hex some buffer */
void printHex (byte *hex, int l);

/* printMacAddress: Print the MacAdddress inside of the package */
void printMacAddress (byte *address, bool broadCast, char *macName);

/* flipData: Here we flip the bytes from data types  */
void flipData (byte *data, unsigned l);

/* printIpv4: To print the Ipv4 */
void printIpv4 (byte *ipv4, char *type);

/* printIpv6: To print the ipv6 */
void printIpv6 (byte *ip, char *type);

#endif
