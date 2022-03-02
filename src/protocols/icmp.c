#include "../../include/protocols/icmp.h"

char *TYPE_ICMP_STRING[] = {
	"Echo Reply",
	"Unreachable",
	"Redirect Message",
	"Echo Request"
};

/* knowTypeOfIcmp: To know the type of icmp we have */
enum TYPE_ICMP knowTypeOfIcmp (byte type)
{
	printf("%u\n", type);
	switch (type) {
	case 0:
		return ECHO_REPLY;
	case 3:
		return UNREACHABLE;
	case 5:
		return REDIRECT_MESSAGE;
	case 8:
		return ECHO_REQUEST;
	}
}

/* readDataIcmp: To read the whole data of the protocol */
void readDataIcmp (struct Icmp *i, byte *data)
{
	memcpy(&(i->type), data, 1);
	data++;
	
    memcpy(&(i->code), data, 1);
	data++;

	memcpy(i->checkSum, data, 2);
	data += 2;
	
	memcpy(i->identifier, data, 2);
	data += 2;
	
	memcpy(i->sequenceNumber, data, 2);
	data += 2;

	
	if (i->typeI == ECHO_REPLY || i->typeI == ECHO_REQUEST)
		return;
}

/* printCodeIcmp: To print the type of code that we have */
char *codeIcmp (struct Icmp *i)
{
	switch (i->typeI) {
	case ECHO_REPLY:
	    return "Echo Reply (used to ping)";
	case UNREACHABLE:
		switch (i->code) {
		case 0:
			return "Destination network unreachable";
		case 1:
			return "Destination host unreachable";
		case 2:
			return "Destination protocol unreachable";
		case 3:
			return "Destination port unreachable";
		case 4:
			return "Fragmentation required, and DF flag set";
		case 5:
			return "Source route failed";
		case 6:
			return "Destination network unknown";
		case 7:
			return "Destination host unknown";
		case 8:
			return "Source host isolated";
		case 9:
			return "Network administratively prohibited";
		case 10:
			return "Host administratively prohibited";
		case 11:
			return "Network unreachable for ToS";
		case 12:
			return "Host unreachable for ToS";
		case 13:
			return "Communication administratively prohibited";
		case 14:
			return "Host Precedence Violation";
		case 15:
			return "Precedence cutoff in effect";
		}
		break;
	case REDIRECT_MESSAGE:
		switch (i->code) {
		case 0:
			return "Redirect Datagram for the Network";
		case 1:
			return "Redirect Datagram for the Host";
		case 2:
			return "Redirect Datagram for the ToS & network";
		case 3:
			return "Redirect Datagram for the ToS & host";
		}
		break;
	case ECHO_REQUEST:
		return "Echo request (used to ping)";
	};
}

/* printIcmp: Prints the protocol of icmp */
void printIcmp (struct Icmp *i)
{
	int j;
	puts("---------------------------------------");
	puts("| ICMP |\n");
	
	printf("Type: %s (%u)\n", TYPE_ICMP_STRING[i->typeI], i->type);
	printf("Code: %s (%u)\n", codeIcmp(i), i->code);
	printf("Header Checksum: (");
	printHex(i->checkSum, 1);
	puts(")");
	
	printf("Identifier: (%u)\n", (*(unsigned short *) flipData(i->identifier, 2)));
	printf("Sequence Number: (%u)\n", (*(unsigned short *) flipData(i->sequenceNumber, 2)));
	printf("Data: ");
	if (i->typeI == ECHO_REPLY || i->typeI == ECHO_REQUEST) {
		for (j = 0; j < i->length; j++) {
			if (*(i->data + j) <= 15)
				printf("0");
			printf("%x", *(i->data + j));
		}
	}
	puts("");
	puts("---------------------------------------");
}

/* Icmppackage: Build an icmp object data */
struct Icmp *IcmpPackage (byte *data, unsigned short length)
{
	struct Icmp *i = (struct Icmp *) malloc(sizeof(struct Icmp));

	i->checkSum = (byte *) malloc(2);
	i->identifier = (byte *) malloc(2);
	i->sequenceNumber = (byte *) malloc(2);
	i->ipv4 = (byte *) malloc(2);
	i->length = length;
	readDataIcmp(i, data);
	i->data = data;
	
	i->typeI = knowTypeOfIcmp(i->type);
	
	i->print = &printIcmp;
	
	return i;
}


