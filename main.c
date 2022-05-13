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


#define FLAGS_LEN 4

/* flags of the program */
char *SHORT_FLAGS[] = {"-f", "-n", "-c", "-pl2"};
char *LARGE_FLAGS[] = {"--file", "--network", "--capture", "--protocol-layer-2"};


enum FLAGS {
    FILER,
    NETWORK,
    CAPTURE,
    PROTOCOL_LAYER_2,
    NO_FLAG
};



/* getFlag: Get the flag */
enum FLAGS getFlag (char *flag)
{
    unsigned i;

    for (i = 0; i < FLAGS_LEN; ++i) {
        if (strcmp(flag, SHORT_FLAGS[i]) == 0 || strcmp(flag, LARGE_FLAGS[i]) == 0)
            return i;
    }

    return NO_FLAG;
}


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


void getInputLower (char *input)
{
    int i;
    
    /* To convert the string a lowercase string */
    for (i = 0; input[i] != 0; ++i)
        input[i] = tolower(input[i]);
}


/* captureNetowrk: Function that with the active arguments we capture the network */
void captureNetwork (const bool savePackage, const bool pl2)
{
    int numcap;
    char ans[100];
    NetWork *network;
    Pak *pack;

    network = NetWorkAnalyze();

    do {
        /* mount the data and print it */
        pack = Package(network->data, network->length);
        pack->print(pack);
        
        /* If we want to save the package */
        if (savePackage) {
            puts("Do you want to capture this package?: (yes/no)");
            printf(">>> ");
            scanf("%s", ans);
            getInputLower(ans);
            if (strcmp(ans, "yes") == 0)
                dumpcap(network->data, network->length, numcap++);
        }
        /* if we want to capture the protocol for testing purpuse */
        if (pl2) {
            puts("Do you want to capture this protocol layer 2?: (yes/no)");
            printf(">>> ");
            scanf("%s", ans);
            getInputLower(ans);
            if (strcmp(ans, "yes") == 0)
                capProtocol(network->data, network->length, numcap++);
        }
        
        /* To read another package */
        puts("Do you want to catch another package?: (yes/no)");
        printf(">>> ");
        scanf("%s", ans);
        getInputLower(ans);

        /* destroy the package */
        pack->deconstructor(pack);

        /* get antoher package */
        network->next(network);
    } while (strcmp(ans, "yes") == 0);

    network->deconstructor(network);
}





int main (int argc, char *argv[])
{
    char ans[200];
    int i, numcap, numprocap;
    enum FLAGS flag, prev;
    /* flags */
    bool saveP, pl2;

    F *file;
    Pak *pack;

    pl2 = saveP = false;
    
    if (2 > argc)
        usage(argv[0]);
    
    i = 1;
    while (i < argc) {
        flag = getFlag(argv[i]);
        if (flag == FILER && (prev == NETWORK || prev == CAPTURE || prev == PROTOCOL_LAYER_2))
            captureNetwork(saveP, pl2);
        switch (flag) {
        case FILER:
            prev = FILER;
            i++;
            if (i == argc)
                usage(argv[0]);
            break;
            
        case NETWORK:
            prev = NETWORK;
            i++;
            if (i == argc)
                captureNetwork(saveP, pl2);
            
            break;
            
        case CAPTURE:
            saveP = true;
            prev = CAPTURE;
            i++;
            if (i == argc)
                captureNetwork(saveP, pl2);
                
            break;
            
        case PROTOCOL_LAYER_2:
            pl2 = true;
            prev = PROTOCOL_LAYER_2;
            i++;
            if (i == argc)
                captureNetwork(saveP, pl2);
            break;
            
        default: /* no flag */
            
            /* To read a file */
            if (prev == FILER) {
                printf("\nfile: %s\n", argv[i]);
                file = File(argv[i]);
                pack = Package(file->data, file->length);

                /* print the package */
                pack->print(pack);

                /* free all the memory */
                pack->deconstructor(pack);
                file->deconstructor(file);
            } else {
                usage(argv[0]);
            }
            
            /* read the next argument */
            i++;
            break;
        }
    }
    
    return 0;
}

