/* Standart libraries */
#include <stdio.h>

/* Import my of my dependencies */
#include "include/package.h"
#include "include/file.h"

void main (int argc, char *argv[])
{
	if (argc > 2 || argc < 2) {
		fprintf(stderr, "%s: Error: we need a file\n", argv[0]);
		fprintf(stderr, "Usage: %s <file-name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	F *f = File(argv[1]);

	/* To read the data fast */
	// f->seeHex(f);

	/* basically we load the package */
	Pak *pack = Package(f->data, f->length - 18);

	/* To free al the memory */
	f->deconstructor(f);
	
	/* To print the protocol */
	pack->printEthernet(pack->ether);
	pack->print(pack);
}
