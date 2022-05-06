#include "../../include/helpers/helpers.h"

/* initlinkedlist: To create a linked list */
void initLinkedList (struct linkedList *l)
{
	l->len = 0;
	l->head = NULL;
	l->tail = NULL;
}

/* pushlinkedlist: To push one element inside of the list */
void pushLinkedList (struct linkedList *i, void *data)
{
	struct node *n = (struct node *) malloc(sizeof(struct node));
	n->prev = NULL;
	n->next = NULL;
	n->data = data;
	
	if (i->len == 0) {
		i->head = n;
		i->tail = n;
	} else {
		i->head->next = n;
		n->prev = i->head;
		i->head = n;
	}

	i->len++;
}

/* deleteElement: To delete one element from the linked list */
void deleteElementLinkedList (struct linkedList *i, void *data)
{
	struct node *aux;

	aux = i->tail;
	while (aux != NULL) {
		if (aux->data == data) {
			if (aux->next != NULL)
				aux->next->prev = aux->prev;
			if (aux->prev != NULL)
				aux->prev->next = aux->next;
			free(aux->data);
			free(aux);
			return;
		}
		aux = aux->next;
	}
}

/* printIpv6: To print the ipv6 */
void printIpv6 (byte *ip, char *type)
{
	/* The address was */
	char addr[INET6_ADDRSTRLEN];
    
    memset(addr, 0, INET6_ADDRSTRLEN);
    
	/* Here we catch the ipv6 */
	inet_ntop(AF_INET6, ip, addr, INET6_ADDRSTRLEN);

	printf("%s: %s\n", type, addr);
}

/* printDataInHex: To print a bunch of data in hex */
void printDataInHex (byte *data, unsigned l)
{
	int j;
	
	printf("Data: ");
	for (j = 0; j < l; j++) {
		if (*(data + j) <= 15)
			printf("0");
		printf("%x", *(data + j));
	}
	printf("\n[Length: %u]\n", l);
}

/* flipData: Here we flip the bytes from data types  */
void flipData (byte *data, unsigned l)
{
	int i; /* index */
	byte temp[l];

    memcpy(temp, data, l);
    
	for (i = 0; i < l; ++i)
        data[(l - 1) - i] = temp[i];
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
