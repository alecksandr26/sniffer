/* Standart libraries */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
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
    fprintf(stderr, "To read a packages from the network you will need root privileges\n\n");
    fprintf(stderr, "\tUsage: sudo %s --network\n\n", main);
    fprintf(stderr, "\tOr you can use\n\n");
    fprintf(stderr, "\tUsage: sudo %s -n\n\n", main);
    fprintf(stderr, "To capture a package from the network you will need root privileges\n\n");
    fprintf(stderr, "\tUsage: sudo %s --capture\n\n", main);
    fprintf(stderr, "\tOr you can use\n\n");
    fprintf(stderr, "\tUsage: sudo %s -c\n\n", main);
    fprintf(stderr, "To capture and read a packages from the network you will need root privileges\n\n");
    fprintf(stderr, "\tUsage: sudo %s --network --capture\n\n", main);
    fprintf(stderr, "\tOr you can use\n\n");
    fprintf(stderr, "\tUsage: sudo %s -n -c\n\n", main);
    fprintf(stderr, "To capture a protocol from the layer 2\n\n");
    fprintf(stderr, "\tUsage: sudo %s --protocol-layer-2\n\n", main);
    fprintf(stderr, "\tOr you can use\n\n");
    fprintf(stderr, "\tUsage: sudo %s -pl2\n\n", main);
    exit(EXIT_FAILURE);
}



/* dumpcap: To dump the captured file */
bool dumpcap (byte *data, unsigned len, int numcap)
{
    int fd;
    char name[17];
    
    memset(name, 0, 17);
    sprintf(name, "cap%i.bin", numcap);
    
    puts("capturing package...");
    
    
    if ((fd = open(name, O_WRONLY | O_CREAT | O_TRUNC)) < 0) {
        fprintf(stderr, "Error opening the file \n");
        return false;
    }
    
    /* Now we write the data */
    write(fd, data, len);
    
    close(fd);
    
    printf("package captured as \"%s\"\n", name);
    
    return true;
}


/* capProtocol: To capture an sspecify protocol of layer 2 */
bool capProtocol (byte *data, unsigned len, int numcap)
{
    int fd;
    char name[25];
    
    memset(name, 0, 17);
    sprintf(name, "capprotocol%i.bin", numcap);
    
    puts("capturing protocol layer 2...");
    
    
    if ((fd = open(name, O_WRONLY | O_CREAT | O_TRUNC)) < 0) {
        fprintf(stderr, "Error opening the file \n");
        close(fd);
        return false;
    }
    
    /* Now we write the data */
    write(fd, data, len);
    
    close(fd);
    
    printf("protocol layer 2 captured as \"%s\"\n", name);
    
    return true;
}





int main (int argc, char *argv[])
{
    char ans[100];
    int i, numcap, numprocap;
    bool cappro;

    numprocap = numcap = 0;
    cappro = false;
	if (2 > argc)
		usage(argv[0]);
	else if (((strcmp(argv[1], "--file") == 0) || (strcmp(argv[1], "-f") == 0)) && argc >= 3) { /* To read a file */
        F *f;
        Pak *pack;
        
        if ((strcmp(argv[argc - 1], "-pl2") == 0) || (strcmp(argv[argc - 1], "--protocol-layer-2") == 0))
            cappro = true;
        
        /* If you want to read multiple files */
        for (i = 2; i < ((cappro) ? argc - 1 : argc); ++i) {
            printf("\nfile%i: %s\n", i - 1, argv[i]);
            f = File(argv[i]);
            pack = Package(f->data, f->length);
            
            pack->print(pack);

            if (cappro)
                capProtocol(pack->ether->data, f->length - 14, numprocap++);

            pack->deconstructor(pack);
            f->deconstructor(f);
        }
    } else if ((strcmp(argv[1], "--network") == 0) || (strcmp(argv[1], "-n") == 0)) { /* To read a network package */
        do {
            NetWork *n = NetWorkAnalyze();
        
            /* Now we mount everything to the pack dependency */
            Pak *pack = Package(n->data, n->length);
            pack->print(pack);

            /* if the flags "-c" is active */
            if (argc == 3 && ((strcmp(argv[2], "-c") == 0) || (strcmp(argv[2], "--capture") == 0))) {
                puts("Do you want to capture this package?: (yes/no)");
                printf(">>> ");
                scanf("%s", ans);
                /* To convert the string a lowercase string */
                for (i = 0; ans[i] != 0; ++i)
                    ans[i] = tolower(ans[i]);
                if (strcmp(ans, "yes") == 0)
                    dumpcap(n->data, n->length, numcap++);
            }
            
            /* To capture a protocol bytes */
            if ((strcmp(argv[argc - 1], "-pl2") == 0) || (strcmp(argv[argc - 1], "--protocol-layer-2") == 0))
                capProtocol(pack->ether->data, n->length - 14, numprocap++);
            
            /* To read another package */
            puts("Do you want to catch another package?: (yes/no)");
            printf(">>> ");
            scanf("%s", ans);
            /* To convert the string a lowercase string */
            for (i = 0; ans[i] != 0; ++i)
                ans[i] = tolower(ans[i]);

            /* deconstruct the objects */
            pack->deconstructor(pack);
            n->deconstructor(n);
            
        } while (strcmp("yes", ans) == 0);
        
    } else if ((strcmp(argv[1], "--capture") == 0) || (strcmp(argv[1], "-c") == 0)) {
        /* First we analyze the network */
        NetWork *n = NetWorkAnalyze();
        /* dump the package */
        dumpcap(n->data, n->length, numcap++);
        
        n->deconstructor(n);
    } else
        usage(argv[0]);

    return 0;
}
