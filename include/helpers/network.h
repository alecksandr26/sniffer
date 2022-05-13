#ifndef __NETWORK_H_
#define __NETWORK_H_

/* To compile -lpcap */
#include <pcap.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1518

struct NetWork {
    unsigned char *data;
    unsigned length;
    
    /* Struct for the list */
    struct pcap_if *dis, *dev;
    
    /* The struct for the session */
    struct pcap_t *session;

    /* Here I create the header */
    struct pcap_pkthdr header;

    /* seeHex: prints the data in hex */
    void (*seeHex) (struct NetWork *network);
    /* deconstructor: To deconstruct the object */
    void (*deconstructor) (struct NetWork *network);

	/* next: read the next package */
	void (*next) (struct NetWork *network);
};

/* This is the object which will analyze the network */
struct NetWork *NetWorkAnalyze ();


typedef struct NetWork NetWork;

#endif


