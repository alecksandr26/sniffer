/* Standart libraries */
#include <stdio.h>
#include <ctype.h> 


#include "include/package.h"
#include "include/helpers/file.h"
#include "include/helpers/network.h"

int main (int argc, char *argv[])
{
    char ans[100];
    int i;
    
	if (3 != argc && argc != 2) {
		fprintf(stderr, "Error: wrong arguments\n");
        fprintf(stderr, "To read a file\n");
		fprintf(stderr, "\tUsage: %s file <file-name>\n", argv[0]);
        fprintf(stderr, "To read a package from the network\n");
        fprintf(stderr, "\tUsage: %s network\n", argv[0]);
		exit(EXIT_FAILURE);
	} else if ((strcmp(argv[1], "--file") == 0) || (strcmp(argv[1], "-f") == 0)) { /* To read a file */
        F *f = File(argv[1]);
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
    } else {
        fprintf(stderr, "%s: Error: wrong arguments\n", argv[0]);
        fprintf(stderr, "To read a file\n");
		fprintf(stderr, "\tUsage: %s file <file-name>\n", argv[0]);
        fprintf(stderr, "To read a package from the network\n");
        fprintf(stderr, "\tUsage: %s network\n", argv[0]);
		exit(EXIT_FAILURE);
    }
    return 0;
}
