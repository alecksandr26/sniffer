#include "../../include/protocols/protocolsAplication.h"

/* printLayer5ProtocolAplication: Prints the protocol inside of transport protocol */
void printLayer5ProtocolAplication (union ProtocolAplication p, enum PROTOCOL_APLICATION pa)
{
    switch (pa) {
    case DNS:
        p.dns->print(p.dns);
        break;
    }
}


/* deconstructProtocolAplication: To destro the protocol aplication selected */
void deconstructProtocolAplication (union ProtocolAplication p, enum PROTOCOL_APLICATION pa)
{
    switch (pa) {
    case DNS:
        p.dns->deconstruct(p.dns);
        break;
    }
}

/* defineProtocolAplication: To define the protocol in the layer of aplication  */
union ProtocolAplication defineProtocolAplication (enum PROTOCOL_APLICATION protocol, byte *data)
{

    union ProtocolAplication p;
    switch (protocol) {
    case DNS:
        p.dns = DnsPackage(data);
        break;
    }

    return p;
}


