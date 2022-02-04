/*
  This is a module which reads a ethernet package and we get info from it 
*/


#ifndef __PACKAGE_H_
#define __PACKAGE_H_

/* std Libreries */
#include <stdbool.h>


/* The different types of enums */
enum UNI_MULTI {
	UNI_CAST,
	MULTI_CAST
};

enum GLOB_LOCA {
	GLOBALLY_UNIQUE,
	LOCALLY_ADMIN
};


/* The types of ethernet */
enum ethertype {
	IPV4_TYPE,
	ARP_TYPE,
	VLAN_TYPE,
	IPV6_TYPE
};



/* This is necessary to create the buffers */
typedef unsigned char byte;


/* This class will mantine the data */
struct Package {
	enum ethertype etherType;
	
	byte *macaddressDes;   /* Mac address Destination | 6 bytes */
	enum UNI_MULTI lastbitDes;
	enum GLOB_LOCA penultimatebitDes;
	bool broadCastDes;
	
	byte *macaddressSor;   /* Mac address Source | 6 bytes */
	enum UNI_MULTI lastbitSor;
	enum GLOB_LOCA penultimatebitSor;
    bool broadCastSor;
	
	unsigned short *ethernetTypeBytes;    /* Ethernet data type | 2 bytes */

	byte *data; /* data */
	unsigned short length; /* the amount of data */
	
	unsigned *frameCheck;    /* Frame check | 4 bytes */
	
	
	/* printData: Is the method which prints the data */
	void (*printData)(struct Package *package);
};


/* Package: This is the constructor we need the data*/
struct Package *Package (byte *data, unsigned short length);


typedef struct Package Pak;

#endif




