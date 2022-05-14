#include "../../include/protocols/tcp.h"

void readTcpPackage (struct Tcp *t, byte *data)
{
	memcpy(&(t->srcPort), data, 2);
	flipData(&(t->srcPort), 2);
	data += 2;

	memcpy(&(t->dstPort), data, 2);
	flipData(&(t->dstPort), 2);
	data += 2;

	memcpy(&(t->sequenceNum), data, 4);
	flipData(&(t->sequenceNum), 4);
	data += 4;

	memcpy(&(t->ackNum), data, 4);
	flipData(&(t->ackNum), 4);
	data += 4;

    memcpy(&(t->dataOffset), data, 1);
    t->dataOffset >>= 4;
    data++;

    memcpy(&(t->flags), data, 1);
    data++;

    memcpy(&(t->windowSize), data, 2);
    flipData(&(t->windowSize), 2);
    data += 2;

    memcpy(t->checkSum, data, 2);
    data += 2;

    memcpy(&(t->urgentPointer), data, 2);
    flipData(&(t->urgentPointer), 2);
    data += 2;

    if (t->dataOffset > 5) { /* here we jump the options */
        t->options = (t->dataOffset - 5) * 4;
        data += t->options;
    }

    t->data = data;
}


void printFlagsTcpPackage (byte byte1, byte byte2)
{
    if (byte1 & 0b00000001)
        puts("\t1 bit 1 NS");
    else
        puts("\t1 bit 0 NS");
            
    if (byte2 & 0b10000000)
        puts("\t2 bit 1 CWR");
    else
        puts("\t2 bit 0 CWR");

    if (byte2 & 0b01000000)
        puts("\t3 bit 1 ECE");
    else
        puts("\t3 bit 0 ECE");

    if (byte2 & 0b00100000)
        puts("\t4 bit 1 URG");
    else
        puts("\t4 bit 0 URG");

    if (byte2 & 0b00010000)
        puts("\t5 bit 1 ACK");
    else
        puts("\t5 bit 0 ACK");

    if (byte2 & 0b00001000)
        puts("\t6 bit 1 PSH");
    else
        puts("\t6 bit 0 PSH");

    if (byte2 & 0b00000100)
        puts("\t7 bit 1 RST");
    else
        puts("\t7 bit 0 RST");

    if (byte2 & 0b00000010)
        puts("\t8 bit 1 SYN");
    else
        puts("\t8 bit 0 SYN");

    if (byte2 & 0b00000001)
        puts("\t9 bit 1 FIN");
    else
        puts("\t9 bit 0 FIN");
}


void printTcpPackage (struct Tcp *t)
{
	puts("---------------------------------------");
	puts("| TCP |\n");
	printf("Source Port: (%u) %s\n", t->srcPort, stringProtocolAplication(t->srcPort));
	printf("Destination Port: (%u) %s\n", t->dstPort, stringProtocolAplication(t->dstPort));
    printf("Sequence Number: (%u)\n", t->sequenceNum);
    printf("Acknowledgment Number: (%u)\n", t->ackNum);
    printf("Data Offset: (%u)\n", t->dataOffset);
    puts("Flags:");
    printFlagsTcpPackage(t->dataOffset, t->flags);
    printf("Window Size: (%u) bytes\n", t->windowSize);
    printf("Check Sum: (");
    printHex(t->checkSum, 1);
    puts(")");
    printf("Urgent Pointer: (%u)\n", t->urgentPointer);
    printf("Options: (%u) bytes\n", t->options);
    
	puts("---------------------------------------");
}


void deconstructTcpPackage (struct Tcp *t)
{
	free(t); 
}

struct Tcp *TcpPackage (byte *data)
{
	struct Tcp *t = (struct Tcp *) malloc(sizeof(struct Tcp));

	readTcpPackage(t, data);
	
	t->print = &printTcpPackage;
	t->decontstruct = &deconstructTcpPackage;
	
	return t;
}




