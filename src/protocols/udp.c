#include "../../include/protocols/udp.h"


/* defineProtocolAplicationType: To define the protocol type of aplication */
enum PROTOCOL_APLICATION defineProtocolAplicationType (unsigned short port)
{
    switch (port) {
    case 53: /* port number dns */
        return DNS;
        break;
        
    default:
        return NIL;
        break;
    }
}

/* readUpdPackage: Lets read the udp package */
void readUdpPackage (struct Udp *u, byte *data)
{
    memcpy(&(u->srcPort), data, 2); /* 2 bytes */
    data += 2;
    flipData(&(u->srcPort), 2); /* fip data */

    /* after reading the src port we define the protocol of aplication */
    u->protocolType = defineProtocolAplicationType(u->srcPort);


    memcpy(&(u->dstPort), data, 2); /* 2 bytes */
    data += 2;
    flipData(&(u->dstPort), 2);

    /* If the port is from client we try with the dst port */
    if (u->protocolType == NIL)
        u->protocolType = defineProtocolAplicationType(u->dstPort);

    memcpy(&(u->lenght), data, 2);
    data += 2;
    flipData(&(u->lenght), 2);

    memcpy(u->checksum, data, 2);
    data += 2;

    u->data = data;
}

/* stringProtocolAplication: returns the port protocol */
char *stringProtocolAplication (int port)
{
    switch (port) {
    case 53:
        return "DNS";
    default:
        return "";
    }
}


/* printUdpProtocol: To print the protocol */
void printUdpProtocol (struct Udp *u)
{
    
	puts("---------------------------------------");
    puts("| UDP |\n");
    printf("Src Port: %u %s\n", u->srcPort, stringProtocolAplication(u->srcPort));
    printf("Dst Port: %u %s\n", u->dstPort, stringProtocolAplication(u->dstPort));
    printf("Lenght: %u\n", u->lenght);
    printf("ChecKSum: ");
    printHex(u->checksum, 1);
    puts("");
	puts("---------------------------------------");

    /* Now we print the protocol */
    printLayer5ProtocolAplication(u->p, u->protocolType);
        
}


/* deconstructUdpProtocol: To delete the package */
void deconstructUdpProtocol (struct Udp *u)
{
    free(u);

    /* here We deconstruct the object */
    deconstructProtocolAplication(u->p, u->protocolType);
}


/* UdpPackage: to load a udp package */
struct Udp *UdpPackage (byte *data)
{
    struct Udp *u = (struct Udp *) malloc(sizeof(struct Udp));

    readUdpPackage(u, data);

    /* here I start the protrocol of aplication */
    u->p = defineProtocolAplication(u->protocolType, u->data);
    
    u->print = &printUdpProtocol;
    u->deconstruct = &deconstructUdpProtocol;
    
    
    return u;
}
