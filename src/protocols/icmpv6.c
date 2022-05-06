#include "../../include/protocols/icmpv6.h"

char *TYPE_ICMPV6_OPTIONS_STRING[] = {
	"Nonce",
	"Source Link Layer Address",
	"Target Link Layer Address",
	"Prefix Information",
	"Redirect Header",
	"Redirect Mtu",
	"Recursive Dns Server"
};

char *TYPE_ICMPV6_STRING[] = {
	"Destination Unreachable",
	"Time Exceeded",
	"Echo request",
	"Echo Reply",
	"Neighbor Advertisement",
	"Router Advertisement",
	"Router Solicitation",
	"Neighbor Solicitation",
	"Redirect Message",
	"Unassigned"
};

/* knowTypeOfOptionIcmpv6: to know the type of the option */
int knowTypeOfOptionIcmpv6 (byte type)
{
	switch (type) {
	case 1:
		return SOURCE_LINK_LAYER_ADDRESS_ICMPV6;
	case 2:
		return TARGET_LINK_LAYER_ADDRESS_ICMPV6;
	case 3:
		return PREFIX_INFORMATION_ICMPV6;
	case 4:
		return REDIRECT_HEADER_ICMPV6;
	case 5:
		return REDIRECT_MTU_ICMPV6;
	case 17:
		return NONCE_ICMPV6;
	case 25:
		return RECURSIVE_DNS_SERVER_ICMPV6;
	default:
		return -1;
	}
}
	

/* knowTypeOfIcmpv6: To know the icmpv6 protocol */
enum TYPE_ICMPV6 knowTypeOfIcmpv6 (byte type)
{
	switch (type) {
	case 1:
		return UNREACHABLE_ICMPV6;
	case 3:
		return TIME_EXCEEDED_ICMPV6;
	case 128:
		return ECHO_REQUEST_ICMPV6;
	case 129:
		return ECHO_REPLY_ICMPV6;
	case 136:
		return NEIGHBOR_ADVERTISEMENT_ICMPV6;
	case 134:
		return ROUTER_ADVERTISEMENT_ICMPV6;
	case 133:
		return ROUTER_SOLICITATION_ICMPV6;
	case 135:
		return NEIGHBOR_SOLICITATION_ICMPV6;
	case 137:
		return REDIRECT_MESSAGE_ICMPV6;
	default:
		return UNASSIGNED_ICMPV6;
	}
}

/* neighborAdverstisementReadData: To read the data of the protocol */
struct NDP *neighborAdverstisementReadData (byte *data, struct NDP *na)
{
	/* flags */
	memcpy(&(na->flags), data, 1);
	data += 4;

	/* The target address */
	memcpy(na->targetAddress, data, 16);
	data += 16;
		
	return na;
}

/* routerAdvertisementReadData: To read the data of the router advertisement header */
struct NDP *routerAdvertisementReadData (byte *data, struct NDP *ra)
{	
	/* cur hop limit */
	memcpy(&(ra->curHopLimit), data, 1);
	data++;

	/* the flags */
	memcpy(&(ra->flags), data, 1);
	data++;
	
	/* the life time */
	memcpy(ra->lifetime, data, 2);
    flipData(ra->lifetime, 2);
	data += 2;
	
	/* reachabletime */
	memcpy(ra->reachableTime, data, 4);
    flipData(ra->reachableTime, 4);
	data += 4;
	
	/* retrasTimer */
	memcpy(ra->retransTimer, data, 4);
    flipData(ra->retransTimer, 4);
	data += 4;
	
	return ra;
}

/* neighborSolicitationReadData: To read the header */
struct NDP *neighborSolicitationReadData (byte *data, struct NDP *ns)
{
	data += 4;
	memcpy(ns->targetAddress, data, 16);
	data += 16;
	
	return ns;
}

/* redirectMessageReadData: To read the header of the redirect message */
struct NDP *redirectMessageReadData (byte *data, struct NDP *rm)
{
	data += 4;
	memcpy(rm->targetAddress, data, 16);
	data += 16;
	
	memcpy(rm->destinationAddress, data, 16);
	data += 16;
	
	return rm;
}

/* readOption: To read the option */
void readOption (byte *data, enum TYPE_OPTIONS_ICMPV6 type, struct Option *op)
{
	switch (type) {
	case SOURCE_LINK_LAYER_ADDRESS_ICMPV6: case TARGET_LINK_LAYER_ADDRESS_ICMPV6:
		memcpy(op->macAddress, data, 6);
		break;
	case REDIRECT_MTU_ICMPV6:
		memcpy(op->mtu, data, 4);
	    flipData(op->mtu, 3);
		break;
	}
}

/* readOptions: To read the options and the dumpt it into the struct */
unsigned readOptions (byte *data, struct NDP *ndp)
{
	int typeOption;
	unsigned num;
	byte type;
	struct Option *op, *prev;

	type = *data; /* The first byte and the first type of options */
	data++;
	prev = NULL;
	while ((typeOption = knowTypeOfOptionIcmpv6(type)) != -1) {
		
		op = (struct Option *) malloc(sizeof(struct Option));
		op->next = NULL;
		if (prev != NULL)
			prev->next = op;
		else {
			ndp->tail = op;
			num = 1;
		}
		op->typeOption = typeOption;
		op->type = type;
		
		/* read the option */
		readOption(data + 1, typeOption, op);
		
		op->len = *data * 8;
		data += op->len - 1; /* we move the amount of bytes plus 1 */
		num += op->len - 1;
		
		type = *data; /* Here we take the new byte */
		data++;
		num++;
		
		/* Here we catch the previos one */
		prev = op;
	}

	return num;
}

/* knowAndreadNDPProtocol: To know which type of the ndp protocol we were using and read it*/
void knowAndReadNDPProtocol (byte *data, struct Icmpv6 *i)
{
	i->ndp = (struct NDP *) malloc(sizeof(struct NDP));
	
	switch (i->type) {
	case 136:
	    i->ndp = neighborAdverstisementReadData(data, i->ndp);
		data += 20;
		break;
	case 134:
		i->ndp = routerAdvertisementReadData(data, i->ndp);
		data += 12;
		break;
	case 135:
		i->ndp = neighborSolicitationReadData(data, i->ndp);
		data += 20;
		break;
	case 137:
		i->ndp = redirectMessageReadData(data, i->ndp);
		data += 36;
		break;
	}
	i->data = data;
	i->data += readOptions(data, i->ndp);
}

/* readIcmpv6Package: To read the icmpv6 package */
void readIcmpv6Package (byte *data, struct Icmpv6 *i)
{
	memcpy(&(i->type), data, 1);
	i->typeNum = knowTypeOfIcmpv6(i->type);
	data++;

	memcpy(&(i->code), data, 1);
	data++;

	memcpy(i->checkSum, data, 2);
	data += 2;

	if (i->type > 132 && i->type < 138)
		knowAndReadNDPProtocol(data, i);
	else if (i->type == 128 || i->type == 129) {
		memcpy(i->identifier, data, 2);
	    flipData(i->identifier, 2);
		data += 2;
		memcpy(i->sequenceNumber, data, 2);
	    flipData(i->sequenceNumber, 2);
		data += 2;
		i->data = data;
	} else if (i->type == 1) { /* To read the ipv6 package */
		i->extraPackage = true;
		data += 4;
		i->dataIpv6Package = i->Ipv6Package(data, true);
		i->data = data;
	
	} else if (i->type == 3) {
		i->extraPackage = true;
		data += 4;
		i->dataIpv6Package = i->Ipv6Package(data, true);
		i->data = data;
	}
}

/* printCodeIcmpv6: To print the code */
char *stringCodeIcmpv6 (int type, byte code)
{
	switch (type) {
	case UNREACHABLE_ICMPV6:
		switch (code) {
		case 0:
			return "no route to destination";
		case 1:
			return "communication with destination administratively prohibited";
		case 2:
			return "beyond scope of source address";
		case 3:
			return "address unreachable";
		case 4:
			return "port unreachable";
		case 5:
			return "source address failed ingress/egress policy";
		case 6:
			return "reject route to destination";
		case 7:
			return "Error in Source Routing Header";
			
		}
	case TIME_EXCEEDED_ICMPV6:
		switch (code) {
		case 0:
			return "hop limit exceeded in transit";
		case 1:
			return "fragment reassembly time exceeded";
		}
	default:
		return "";
	}
}

/* printNeighborAdvertisement: To print an na protocol */
void printNeighborAdvertisement (struct NDP *na)
{	
	/* Flags */
	puts("Flags: ");
	if (na->flags & 0b10000000)
		puts("\t1. bit 1 Sended by Router");
	else
		puts("\t1. bit 0 Sended by Host");

	if (na->flags & 0b01000000)
		puts("\t2. bit 1 A Neighbor Solicitation");
	else
		puts("\t2. bit 0 Not A Neighbor Solicitation");

	if (na->flags & 0b0010000)
		puts("\t3. bit 1 Override Cache");
	else
		puts("\t3. bit 0 Not Override Cache");

	printIpv6(na->targetAddress, "Target Address");
}

/* printRouterAdverstisement: To print the router advertisement header */
void printRouterAdverstisement (struct NDP *ra)
{
    printf("Cur Hop Limit: (%u)\n", ra->curHopLimit);
	puts("Flags:");
	if (ra->flags & 0b1000000)
		puts("\t1. bit 1 Managed Address Configuration");
	else
		puts("\t1. bit 0 Managed Address Configuration");

	if (ra->flags & 0b0100000)
		puts("\t2. bit 1 Other Stateful Configuration");
	else
		puts("\t2. bit 0 Other Stateful Configuration");

	printf("Life Time: (%u) Seconds\n", *((unsigned short *) ra->lifetime));
	printf("Reachable Time: (%u) MilliSeconds\n", *((unsigned *) ra->reachableTime));
	printf("Retrans Timer: (%u) MilliSeconds\n", *((unsigned *) ra->retransTimer));
}

/* printNeighborSolicitation: To print the neighbor solicitation */
void printNeighborSolicitation (struct NDP *ns)
{
	printIpv6(ns->targetAddress, "Target Address");
}

/* printRedirectMessage: To print the redirect message */
void printRedirectMessage (struct NDP *rm)
{
	printIpv6(rm->targetAddress, "Target Address");
	printIpv6(rm->destinationAddress, "Destination Address");
}

/* printNDPOptions: To print the options */
void printNDPOptions (struct Option *op)
{
	if (op != NULL)
		puts("Options:");
	while (op != NULL) {
		printf("Option: (%u) (%s)\n", op->type, TYPE_ICMPV6_OPTIONS_STRING[op->typeOption]);
		printf("Length: (%u) bytes\n", op->len);
		switch (op->typeOption) {
		case SOURCE_LINK_LAYER_ADDRESS_ICMPV6:
			printMacAddress(op->macAddress, false, "Source");
			break;
		case TARGET_LINK_LAYER_ADDRESS_ICMPV6:
			printMacAddress(op->macAddress, false, "Target");
			break;
		case REDIRECT_MTU_ICMPV6:
			printf("Mtu: (%u) bytes\n", *((unsigned *) op->mtu));
			break;
		}
		
		op = op->next;
	}
}

/* printNDPProtocol: To prin the ndp protocol */
void printNDPProtocol (struct NDP *ndp, byte type)
{
    switch (type) {
	case 136:
	    printNeighborAdvertisement(ndp);
		break;
	case 134:
	    printRouterAdverstisement(ndp);
		break;
	case 135:
		printNeighborSolicitation(ndp);
		break;
	case 137:
		printRedirectMessage(ndp);
		break;
    }

	printNDPOptions(ndp->tail);
}

/* printIcmpv6: To print the icmp */
void printIcmpv6 (struct Icmpv6 *i)
{
	puts("---------------------------------------");
	puts("| Icmpv6 |\n");
	printf("Type: (%u) (%s)\n", i->type, TYPE_ICMPV6_STRING[i->typeNum]);
	printf("Code: (%u) (%s)\n", i->code, stringCodeIcmpv6(i->typeNum, i->code));
	printf("Check Sum: (");
	printHex(i->checkSum, 1);
	puts(")");
	if (i->type > 132 && i->type < 138)
		printNDPProtocol(i->ndp, i->type);
	else if (i->type == 128 || i->type == 129) {
		printf("Identifier: (%u)\n", *((unsigned short *) i->identifier));
		printf("Sequence Number: (%u)\n", *((unsigned short *) i->sequenceNumber));
	}
		
	puts("---------------------------------------");
}

void Icmpv6PackageDeconstructor (struct Icmpv6 *i)
{
    struct Option *op, *aux;
    
    if (i->ndp != NULL) {
        op = i->ndp->tail;
        
        while (op != NULL) {
            aux = op;
            op = op->next;
            free(aux);
        }
    }
    
    free(i->ndp);
    free(i);
}


/* Icmpv6Packge: To catch the icmpv6 package */
struct Icmpv6 *Icmpv6Package (byte *data, unsigned short length, void *(*Ipv6Package)(byte *data, bool justHeader))
{
	struct Icmpv6 *i = (struct Icmpv6 *) malloc(sizeof(struct Icmpv6));
	
	i->extraPackage = false;
    
	i->Ipv6Package = Ipv6Package;
	
	readIcmpv6Package(data, i);
	
	i->print = &printIcmpv6;
    i->deconstruct = &Icmpv6PackageDeconstructor;
	
	return i;
}


