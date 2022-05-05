#ifndef __APLICATION_H__
#define __APLICATION_H__

#include <stdlib.h>
#include "../../include/helpers/helpers.h"
#include "../../include/protocols/dns.h"

enum PROTOCOL_APLICATION {
    NIL,
    DNS
};

union ProtocolAplication {
    Dns *dns;
};


/* printLayer5ProtocolAplication: Prints the protocol inside of transport protocol */
void printLayer5ProtocolAplication (union ProtocolAplication p, enum PROTOCOL_APLICATION pa);

/* deconstructProtocolAplication: To destro the protocol aplication selected */
void deconstructProtocolAplication (union ProtocolAplication p, enum PROTOCOL_APLICATION pa);

/* defineProtocolAplication: To define the protocol in the layer of aplication  */
union ProtocolAplication defineProtocolAplication (enum PROTOCOL_APLICATION protocol, byte *data);

#endif




