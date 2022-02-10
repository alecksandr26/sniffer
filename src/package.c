
/* Here I import my header file */
#include "../include/package.h"


/* WE DON'T USE THIS FUNCITON */
/* flipData: Here we flip the bytes from data types  */
byte *flipData (byte *data, unsigned l)
{
	int i; /* index */
	byte *temp = (byte *) malloc(l);

	for (i = 0; i < l; ++i)
		*(temp + (l - 1) - i) = *(data + i);

	free(data);
   
	return temp;
}



/* Package: This is the constructor we need the data*/
struct Package *Package (byte *data, unsigned short dataLength)
{
	/* Here I create my object */
	struct Package *p = (struct Package *) malloc(sizeof(struct Package));
	
	p->ether = Ethernet(data, dataLength);
	p->protocol = defineProtocol(p->ether, p->ether->data);
	
	/* Here I return the object */
	return p;
}
