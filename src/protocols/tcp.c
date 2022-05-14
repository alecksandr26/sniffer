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
}


void printTcpPackage (struct Tcp *t)
{
	puts("---------------------------------------");
	puts("| TCP |\n");
	printf("Src Port: (%u)\n", t->srcPort);
	printf("Dst Port: (%u)\n", t->dstPort);
	
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




