#ifndef __TCP_H_
#define __TCP_H_

#include <stdlib.h>
#include <string.h>

#include "../../include/helpers/helpers.h"


/* to read the tcp package */
struct Tcp {
	unsigned short srcPort;
	unsigned short dstPort;
	unsigned sequenceNum;
	unsigned ackNum;
    byte dataOffset;
    byte flags;
    unsigned short windowSize;
    byte checkSum[2];
    unsigned short urgentPointer;
    byte options;

    byte *data;
    
	void (*print) (struct Tcp *);
	void (*decontstruct) (struct Tcp *);
};

struct Tcp *TcpPackage (byte *data);

typedef struct Tcp Tcp;

#endif
