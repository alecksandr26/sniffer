#include "../../include/helpers/network.h"

/* selectDevice: To select the device */
struct pcap_if *selectDevice (struct pcap_if *list)
{
    struct pcap_if *head;
    int num, temp, total;

    head = list;
    total = num = 0;
    
    while (head != NULL) {
        printf("[%i] %s.\n", num++, head->name);
        head = head->next;
        total++;
    }
    
    do { /* Select a valid num */
        printf("Select a device: ");
        scanf("%i", &num);

        temp = 0;
        while (list != NULL) {
            if (temp == num)
                return list;
            list = list->next;
            temp++;
        }
    } while (num > total || num < 0);

    /* If there is an error */
    return NULL;
}


/* NetWorkAnalyzeSeeHex: print the data in hex */
void NetWorkAnalyzeSeeHex (struct NetWork *n)
{
    
	int i; /* index */

	for (i = 0; i < n->length; ++i)
		printf("%u. | hex: %x | uns: %u\n", i+1, *(n->data + i), *(n->data + i));
}

/* NetWorkAnalyzeDeconstruct: To deconstruct the object */
void NetWorkAnalyzeDeconstruct (struct NetWork *n)
{
    /* To destroy the list */
    pcap_freealldevs(n->dis);
    
    /* To close the session */
    pcap_close(n->session);

    /* And here I delete the object */
    free(n);
}


/* nextNetWorkPackage: to read another package  */
void nextNetWorkPackage (struct NetWork *n)
{
    /* Now we need to catch the data */
    n->data = pcap_next(n->session, &n->header);
    n->length = n->header.len;
}


/* This is the object which will analyze the network */
struct NetWork *NetWorkAnalyze ()
{
    struct NetWork *n = (struct NetWork *) malloc(sizeof(struct NetWork));
    struct pcap_pkthdr header;
    int num;
    char errbuf[PCAP_ERRBUF_SIZE];

    
    /* Lets find all the devs n->dis is a list of elements */
    num = pcap_findalldevs(&n->dis, NULL);
    if (num < 0) {
        fprintf(stderr, "Error: There are not any device\n");
        exit(EXIT_FAILURE);
    }

    /* Here we select the device */
    n->dev = selectDevice(n->dis);

    /* To get a package */
    printf("\nSelecting: %s ...\n\n", n->dev->name);
    if ((n->session = pcap_open_live(n->dev->name, 8192, 1, 0, errbuf)) == NULL) {
        fprintf(stderr, "Error: %s\n", errbuf);
        exit(EXIT_FAILURE);
    } else if (pcap_datalink(n->session) != DLT_EN10MB) {
        fprintf(stderr, "Error: The package is not ethernet\n");
        exit(EXIT_FAILURE);
    }

    /* Now we need to catch the data */
    n->data = pcap_next(n->session, &n->header);
    n->length = n->header.len;

    n->seeHex = &NetWorkAnalyzeSeeHex;
    n->deconstructor = &NetWorkAnalyzeDeconstruct;
    n->next = &nextNetWorkPackage;
    
    return n;
}


