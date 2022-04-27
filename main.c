/* Standart libraries */
#include <stdio.h>
#include <ctype.h> 


#include "include/package.h"
#include "include/helpers/file.h"
#include "include/helpers/network.h"



void usage (char *main)
{
    fprintf(stderr, "Error: wrong arguments\n\n");
    fprintf(stderr, "To read a file\n\n");
    fprintf(stderr, "\tUsage: %s --file <file-name>\n\n", main);
    fprintf(stderr, "\tOr you can use\n\n");
    fprintf(stderr, "\tUsage: %s -f <file-name>\n\n", main);
    fprintf(stderr, "To read a package from the network\n\n");
    fprintf(stderr, "\tUsage: %s --network\n\n", main);
    fprintf(stderr, "\tOr you can use\n\n");
    fprintf(stderr, "\tUsage: %s -n\n\n", main);
    exit(EXIT_FAILURE);
}


int main (int argc, char *argv[])
{
    char ans[100];
    int i;
    
	if (3 != argc && argc != 2)
		usage(argv[0]);
	else if ((strcmp(argv[1], "--file") == 0) || (strcmp(argv[1], "-f") == 0)) { /* To read a file */
        F *f = File(argv[2]);
        Pak *pack = Package(f->data, f->length);
        pack->print(pack);
        pack->deconstructor(pack);
        f->deconstructor(f);
    } else if ((strcmp(argv[1], "--network") == 0) || (strcmp(argv[1], "-n") == 0)) { /* To read a network package */
        do {
            NetWork *n = NetWorkAnalyze();
        
            /* Now we mount everything to the pack dependency */
            Pak *pack = Package(n->data, n->length);
            pack->print(pack);
            pack->deconstructor(pack);
            n->deconstructor(n);
            /* To read another package */
            puts("Do you want to read another package?: (yes/no)");
            printf(">>> ");
            scanf("%s", ans);
            /* To convert the string a lowercase string */
            for (i = 0; ans[i] != 0; ++i)
                ans[i] = tolower(ans[i]);
        } while (strcmp("yes", ans) == 0);
    } else
        usage(argv[0]);

    return 0;
}
