#ifndef __ETHERNET_H_
#define __ETHERNET_H_

/* Std librearies */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "./helpers/helpers.h"


/* The different types of enums */
enum UNI_MULTI {
    UNI_CAST,
    MULTI_CAST
};

enum GLOB_LOCA {
    GLOBALLY_UNIQUE,
    LOCALLY_ADMIN
};

/* The amount ehter types */
#define AMOUNT_ETHER_TYPES 4

/* The types of ethernet */
enum ETHER_TYPES {
	ARP_TYPE,
    IPV4_TYPE,
    VLAN_TYPE,
    IPV6_TYPE
};

/* This is the struct to save the ether data */
struct EtherPackage {
	byte ethernetTypeBytes[2];    /* Ethernet data type | 2 bytes */
	enum ETHER_TYPES etherType;
	
    byte macaddressDes[6];   /* Mac address Destination | 6 bytes */
    /* The data of the first mac address */
	enum UNI_MULTI lastbitDes;
    enum GLOB_LOCA penultimatebitDes;
	bool broadCastDes;
	
    byte macaddressSor[6];   /* Mac address Source | 6 bytes */
	/* The data of the second mac address */
	enum UNI_MULTI lastbitSor;
    enum GLOB_LOCA penultimatebitSor;
	bool broadCastSor;
	
	byte *data; /* The pointer of the data left */

	unsigned short length; /* the amount of data */
    
    byte frameCheck[4];    /* Frame check | 4 bytes */
	bool isFrameCheck; /* To know if we really have the frame check or not */
	
	/* print: We are going to print the data as ethernet */
	void (*print) (struct EtherPackage *etherdata);
    void (*deconstructor) (struct EtherPackage *etherdata);
};

/* EtherPackage: Now here I load the data of the ethernet package */
struct EtherPackage *Ethernet (byte *data, unsigned short length);

/* Some extra functions */

/* knowEthertype: To know which ethernet type we have in the package */
enum ETHER_TYPES knowEtherType (byte *data);

typedef struct EtherPackage Ether;

#endif
