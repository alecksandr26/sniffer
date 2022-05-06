#include "../../include/protocols/ipv6.h"

unsigned headerList[] = {0, 6, 17, 43, 44, 50, 51, 58, 60};
char *headerListString[] = {"Hop-by-Hop", "TCP", "UDP", "Routing",
	"Fragment", "Encapsulating Security Payload", "Authentication Header", "IPv6-ICMP",
	"Destination Options"};

/* isProtocolIpv6: To know if it is a protocol */
bool isProtocolIpv6 (int p)
{
	switch (p) {

    case 6: /* tcp */
        return true;
        break;
        
    case 17: /* udp */
        return true;
        break;
        
	case 58: /* icmpv6 */
		return true;
		break;
		
	default:
		return false;
		break;
	}
}

/* defineNextHeader: To define the next header of the protocol */
int defineNextHeader (int next)
{
	int i;

	for (i = 0; i < 9; ++i)
		if (headerList[i] == next)
			return i;
	
	return -1;
}

/* defineIpv6ProtocolType: To know the protocol that we are going to use */
enum PROTOCOL_TRANSPORT defineIpv6ProtocolType (byte p)
{
	switch (p) {
    case 6:
        return TCP;
        break;
    case 17:
        return UDP;
        break;
	case 58: /* If it is an icmpv6 protocol */
		return ICMPV6;
		break;
	}
}

/* readNextHeader: To read the headers from the package */
void readNextHeader (struct Ipv6 *i, byte *data)
{
	struct headerNode *h;
	byte nextHeader;

	nextHeader = i->nextHeader;
	while (!isProtocolIpv6(nextHeader)) {
		h = (struct headerNode *) malloc(sizeof(struct headerNode));
	   
		memcpy(&(nextHeader), data, 1);
		data++;
		
		memcpy(&(h->headerExtensionLength), data , 1);
		data += (h->headerExtensionLength) + 1;

		h->headerIndex = defineNextHeader(nextHeader);
		h->nextHeader = nextHeader;
		
		/* Here I pusht the item to the list */
		pushLinkedList(&(i->l), h);

        
        if (h->headerIndex == -1)
            break;
	}

	i->protocolType = defineIpv6ProtocolType(nextHeader);
	i->data = data; /* To catch the pointer */
}

/* readIpv6protocol: To read all the data from the package */
void readIpv6Protocol (struct Ipv6 *i, byte *data)
{
	byte temp; /* A temporal byte */
	
	/* To get the version of the ipv package */
	memcpy(&(i->version), data, 1);
	i->version &= 0b11110000;
	i->version >>= 4;

	/* To get the simple traffic class */
	memcpy(&(temp), data, 1);
	temp &= 0b00001111;
	temp <<= 4;
	i->trafficClass |= temp;
	data++;
	
	memcpy(&(temp), data, 1);
	temp &= 0b11110000;
	temp >>= 4;
	i->trafficClass |= temp;

	/* To get the flow label */
	memcpy(&(temp), data, 1);
	temp &= 0b00001111;
	*(i->flowLabel + 2) = temp;
	data++;
	
	memcpy(&(temp), data, 1);
	temp &= 0b11110000;
	*(i->flowLabel + 2) |= temp;

	memcpy(&(temp), data, 1);
	temp &= 0b00001111;
	*(i->flowLabel + 1) = temp;
	data++;

	memcpy(&(temp), data, 1);
	temp &= 0b11110000;
	*(i->flowLabel + 1) |= temp;

	memcpy(&(temp), data, 1);
	temp &= 0b00001111;
	*(i->flowLabel) |= temp;
	data++;

	/* payload length */
	memcpy(i->payloadLength, data, 2);
    flipData(i->payloadLength, 2);
	data += 2;

	/* Next header */
	memcpy(&(i->nextHeader), data, 1);
	i->headerIndex = defineNextHeader(i->nextHeader);
	i->headerList = !isProtocolIpv6(i->nextHeader);
	data++;

	/* Hop limit */
	memcpy(&(i->hopLimit), data, 1);
	data++;

	/* Source Address */
	memcpy(i->sourceAddress, data, 16);
	data += 16;

	/* Destination Address */
	memcpy(i->destinationAddress, data, 16);
	data += 16;

	/* Here we are going to read the next header */
	readNextHeader(i, data);
}

/* printNextHeader: To print the next headers */
void printNextHeader (struct Ipv6 *i)
{
	struct node *aux;
	struct headerNode *node;

	aux = i->l.tail;
	while (aux != NULL) {
		node = (struct headerNode *) aux->data;
        if (node->headerIndex != -1) {
            printf("Next header: (%s)\n", headerListString[node->headerIndex]);
            printf("Header extension lenght: (%u)\n", node->headerExtensionLength);
        } else {
            puts("Next header: nil");
            puts("Header extesion lenght: (nil) ");
        }
		aux = aux->next;
	}
}

/* printIpv6protocol: To print the the protocol */
void printIpv6Protocol (struct Ipv6 *i)
{
	puts("---------------------------------------");
	puts("| IPV6 |\n");
	printf("Version: (%u)\n", i->version);
	printf("Traffic Class: (%u)\n", i->trafficClass);
	printf("Flow Label: (%u)\n", *((unsigned *) i->flowLabel));
	printf("Payload Length: (%u)\n", *((unsigned short *) i->payloadLength));
	printf("Next Header: (%s)\n", headerListString[i->headerIndex]);
	printf("Hop Limit: (%u)\n", i->hopLimit);
	printIpv6(i->sourceAddress, "Source Address");
	printIpv6(i->destinationAddress, "Destination Address");
    
	if (i->headerList && !i->justHeader) {
		puts("\nExtension headers: ");
		printNextHeader(i);
	}
	puts("---------------------------------------");
    
    
    
	if (!i->justHeader) {
        printLayer4ProtocolTransport(i->protocolData, i->protocolType);
		if (i->protocolType == ICMP && i->protocolData.icmpv6->extraPackage)
			i->print(i->protocolData.icmpv6->dataIpv6Package);
	}
    
}

/* Ipv6PackageDeconstructor: To destroy the object */
void Ipv6PackageDeconstructor (struct Ipv6 *i)
{
    struct Ipv6 *extra;
    struct node *n, *aux;
    
    /* First we are going to free all the memory from the linked list */
    n = i->l.head;
    while (n != NULL) {
        aux = n;
        n = n->next;
        free((struct headerNode *) aux->data);
        free(aux);
    }

    /* After that we need to free the icmpv6 package */
    if (!i->justHeader) {
        if (i->protocolType == ICMPV6 && i->protocolData.icmpv6->extraPackage) {
            extra = (struct Ipv6 *) i->protocolData.icmpv6->dataIpv6Package;
            extra->deconstruct(extra);
        }

        deconstructProtocolTransport(i->protocolData, i->protocolType);
    }
    
    free(i);
}


/* Ipv6Package: To create ipv6 package */
struct Ipv6 *Ipv6Package (byte *data, bool justHeader)
{
	struct Ipv6 *i = (struct Ipv6 *) malloc(sizeof(struct Ipv6));

	i->justHeader = justHeader;
	initLinkedList(&(i->l));
	i->headerIndex = -1;

	/* Here we read the protocol */
	readIpv6Protocol(i, data);
	
	if (!justHeader)
		i->protocolData = defineProtocolTransport(i->protocolType, i->data, i->payloadLength, Ipv6Package);
	
	i->print = &printIpv6Protocol;
    i->deconstruct = &Ipv6PackageDeconstructor;
	
	return i;
}
