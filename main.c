
/* Import my of my dependencies */
#include "include/package.h"
#include "include/file.h"


/* Standart libraries */
#include <stdio.h>



void main ()
{
	char file[20];
	
	printf("Put a file: ");
	scanf("%s", file);
	
	F *f = File(file);

	/* To read the data fast */
	// f->seeHex(f);

	/* basically we load the package */
	Pak *pack = Package(f->data, f->length - 18);

	pack->printData(pack);
}
