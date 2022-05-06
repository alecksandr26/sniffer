#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#include "../include/helpers/helpers.h"
#include "../include/protocols/ipv6.h"




char PAYLOAD[] = {
    "\x60\x05\xfe\x4a\x00\x4b\x06\x40\x28\x06\x02\xf0\x51\xc1\xde\x77\xbe\xc9\x51\xb1\xc0\xd4\x3b\x81\x26\x00\x19\x01\x00\x01\x0c\xa7\x00\x00\x00\x00\x00\x00\x00\x00\xab\x02\x01\xbb\xa7\xe8\x69\x80\xde\x7c\x1a\x68\x80\x18\x42\xdc\xb3\xfa\x00\x00\x01\x01\x08\x0a\xc6\x05\xe1\xb3\x01\xb7\xce\x23\x17\x03\x03\x00\x26\xcb\x9d\x06\x14\x12\x53\xcc\x4f\xe9\x19\x9c\x29\x63\x52\x5b\xa6\xad\xb5\xa6\x07\xec\x7e\xa5\x5d\x56\x4d\x09\xf8\xeb\x0c\x86\x09\x3c\x77\x44\xc8\x64\x9d"
};


int main (int argc, char *argv[])
{
    int fd;
    Ipv6 *i;
    byte buffer[129];

    int *e;
    
    /* Here I use file as payload */
    if (argc == 2) {
        if ((fd = open(argv[1], O_RDONLY)) < 0) {
            puts("Test fail");
            exit(EXIT_FAILURE);
        }
        
        read(fd, buffer, 129);
        close(fd);

        i = Ipv6Package(buffer, false);
        i->print(i);
    
        i->deconstruct(i);
        
        i = Ipv6Package(buffer, false);
        i->print(i);
    
        i->deconstruct(i);

        i = Ipv6Package(buffer, false);
        i->print(i);
    
        i->deconstruct(i);

        i = Ipv6Package(buffer, false);
        i->print(i);
    
        i->deconstruct(i);
        
    } else {
        i = Ipv6Package(PAYLOAD, false);

        i->print(i);
    
        i->deconstruct(i);
    }


    e = malloc(sizeof(int));
    free(e);
    
    return 0;
}


