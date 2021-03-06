#ifndef __ARP_H_
#define __ARP_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../helpers/helpers.h"

/* The amount types */
#define AMOUNT_TYPES 4

/* The types of ethernet */
enum PROTOCOL_TYPES {
	ARP,
    IPV4,
    VLAN,
    IPV6
};

enum REQUEST_REPLY {
	REQUEST,
	REPLY
};

/* struct Arp will contains the data from arp */
struct Arp {
	enum REQUEST_REPLY request;
	enum PROTOCOL_TYPES protocolType;
	byte hardwareLength[1];
	byte protocolLength[1];
	byte hardwareType[2];
	byte protocol[2];
	byte byteOfRequestReply[2];
	byte ipv4Sender[4];
	byte macSender[6];
	byte ipv4Target[4];
	byte macTarget[4];

	/* To print the data from the pacakge */
	void (*print) (struct Arp *a);
    void (*deconstruct) (struct Arp *a);
};

/* AprPackage: This function will create all our arp structure */
struct Arp *ArpPackage (byte *data);

typedef struct Arp Arp;

#endif
