#ifndef __UDP_H__
#define __UDP_H__

#include "../../include/helpers/helpers.h"
#include "../../include/protocols/protocolsAplication.h"

#include <string.h>
#include <stdlib.h>

struct Udp {
    unsigned short srcPort;
    unsigned short dstPort;
    unsigned short lenght;
    byte checksum[2];

    /* data poitner to the next prototcols */
    byte *data;

    /* protocol of aplication */
    enum PROTOCOL_APLICATION protocolType;
    union ProtocolAplication p;
    
    void (*print) (struct Udp *);
    void (*deconstruct) (struct Udp *);
};


struct Udp *UdpPackage (byte *data);


typedef struct Udp Udp;


#endif
