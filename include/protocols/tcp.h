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

	void (*print) (struct Tcp *);
	void (*decontstruct) (struct Tcp *);
};

struct Tcp *TcpPackage (byte *data);

typedef struct Tcp Tcp;

#endif
