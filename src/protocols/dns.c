#include "../../include/protocols/dns.h"


/* getTypeOfDNsSec: Return an string if it is type 0x0001 or 0x0002 etc */
char *getTypeOfDnsSec (unsigned short type)
{
    switch (type) {
    case 1: /* ipv4 version */
        return "A (Ipv4 Address)";
        break;
    case 2: /* name server record */
        return "NS (Name Server Record)";
        break;
    case 5:
        return "CNAME (Canonical Nama for Alias)";
        break;
    case 6:
        return "SOA (Start of a Zone of Athority)";
        break;
    case 15:
        return "MX (Mail Exchange Record)";
        break;
    case 16:
        return "TXT (Text)";
        break;
    case 28: /* ipv6 version */
        return "AAAA (Ipv6 Address)";
        break;
    case 41:
        return "OPT (Option)";
        break;
    default:
        return "NULL";
        break;
    }
}



/* getClassOfDnsSec: get string with the data of the dns section */
void printClassOfDnsSec (unsigned short class, byte *classBytes)
{
    printf("class: ");
    switch (class) {
    case 1:
        printf("IN (");
        
        break;
    default:
        printf("NULL (");
        
        break;
    }
    printHex(classBytes, 1);
    puts(")");
}



/* readNameDnsPackage: Is double pointer to the data pointer */
byte *readNameDnsPackage (char *domainName, unsigned length, byte *data, byte *bsp, struct linkedList *l)
{
    byte len;
    const byte dot = '.';

    memset(domainName, 0, length);

    len = 1;
    /* while len be diferent to 0 */
    while (len != 0) {
        memcpy(&len, data, 1);
        data++;

        /* the end of string */
        if (len == 0) {
            /* there is not pointer */
            if (l->len == 0)
                break;
            else {/* get the last pointer */
                while (l->len > 0)
                    data = (byte *) popDataLinkedList(l);
                break;
            }
        }
        
        if (len == 0xc0) { /* pointer */
            /* push the diretion save the direction */
            
            pushLinkedList(l, data + 1);
            /* get he new pointer */
            data = bsp + *data;
            continue; /* jump to the next iteration */

        } else {
            memcpy(domainName, data, len);
            data += len; /* move the pointer */
            domainName += len;
            
        }
        if (*(data ) != 0) { /* To add the . to the domain name */
            memcpy(domainName++, &dot, 1);
        }
    }

    return data;
}


void printQuestionDnsPackage (const struct QuestionsDns *qd)
{
    puts("\nQuestion: ");
    printf("name: %s\n", qd->domainName);
    printf("type: %s (%u)\n", getTypeOfDnsSec(qd->type), qd->type);
    printClassOfDnsSec(qd->class, qd->classBytes);
}


/* To readn and return the questions dns structure */
struct QuestionsDns *QuestionsDnsPackage (byte *data, byte *bsp)
{
     struct QuestionsDns *qd = (struct QuestionsDns *) malloc(sizeof(struct QuestionsDns));
     
     /* Here I create the linked list */
     struct linkedList l;
     initLinkedList(&l);

     /* first read the name */
     data = readNameDnsPackage(qd->domainName, 253, data, bsp, &l);
     

     /* now read the type */
     memcpy(&(qd->type), data, 2);
     data += 2;
     flipData(&(qd->type), 2);

     /* now read the class */
     memcpy(&(qd->class), data, 2);
     memcpy(qd->classBytes, data, 2);
     data += 2;
     flipData(&(qd->class), 2);

     qd->data = data;
     qd->print = printQuestionDnsPackage;
     
     return qd;
}





/* printAnswerDnsPackage: To print the answer */
void printAnswerDnsPackage (const struct AnswerDns *ad)
{
    int i;
    puts("\nAnswer: ");
    printf("name: %s\n", ad->domainName);
    printf("type: %s (%u)\n", getTypeOfDnsSec(ad->type), ad->type);
    printClassOfDnsSec(ad->class, ad->classBytes);
    printf("ttl: %u\n", ad->ttl);
    printf("rdlenght: %u\n", ad->rdLenght);

    if (ad->type == 5){
        printf("%s\n", ad->rddata);
    } else if (ad->rdLenght == 4 && ad->type == 1) {
        printIpv4(ad->rddata, "A");
    } else if (ad->rdLenght == 16 && ad->type == 28) {
        printIpv6(ad->rddata, "AAAA");
    } else if (ad->type == 15) {
        printf("preface: %u\n", ad->preference);
        printf("exchange: %s\n", ad->name);
    }

    /* after printing the data */
}


/* AnswerDnsPackage:  */
struct AnswerDns *AnswerDnsPackage (byte *data, byte *bsp)
{
    struct AnswerDns *ad = (struct AnswerDns *) malloc(sizeof(struct AnswerDns));
    
    /* first read the name */
    
    /* Here I create the linked list */
    struct linkedList l;
    initLinkedList(&l);
    
    data = readNameDnsPackage(ad->domainName, 253, data, bsp, &l);

    /* now read the type */
    memcpy(&(ad->type), data, 2);
    data += 2;
    flipData(&(ad->type), 2);

    /* now read the class */
    memcpy(&(ad->class), data, 2);
    memcpy(ad->classBytes, data, 2);
    data += 2;
    flipData(&(ad->class), 2);

    /* read the ttl */
    memcpy(&(ad->ttl), data, 4);
    data += 4;
    flipData(&(ad->ttl), 4);

    /* read the rdLenght */
    memcpy(&(ad->rdLenght), data, 2);
    data += 2;
    flipData(&(ad->rdLenght), 2);

    /* read the data */
    ad->rddata = (byte *) malloc(ad->rdLenght);
    memcpy(ad->rddata, data, ad->rdLenght);
    
    /* if the rddata its a name we read it */
    if (ad->type == 5) {
        memset(ad->rddata, 0, ad->rdLenght);
        readNameDnsPackage(ad->rddata, ad->rdLenght, data, bsp, &l);
    } else if (ad->type == 15) {
        memcpy(&(ad->preference), data, 2);
        readNameDnsPackage(ad->name, 253, data + 2, bsp, &l);
    }
    
    data += ad->rdLenght;

    ad->print = &printAnswerDnsPackage;

    /* Track the pointer */
    ad->data = data;
    
    return ad;
}



/* printAuthoritativeDnsPackage: to print a authority section  */
void printAuthoritativeDnsPackage (struct AuthoritativeDns *ahd, byte *bsp)
{
    puts("\nAuthority: ");
    printf("name: %s\n", ahd->name);
    printf("type: %s (%u)\n", getTypeOfDnsSec(ahd->type), ahd->type);
    printClassOfDnsSec(ahd->class, ahd->classBytes);
    printf("ttl: %u\n", ahd->ttl);
    printf("data length: %u \n", ahd->dataLength);

    if (ahd->type == 1) {
        if (ahd->dataLength == 4)
            printIpv4(ahd->address, "A");
        else if (ahd->dataLength == 16)
            printIpv6(ahd->address, "AAAA");

    } else if (ahd->type == 6) {
        printf("primary name server: %s\n", ahd->primaryNameSever);
        printf("responsible authority mailbox: %s\n", ahd->authorityMailBox);
        printf("serial number: %u\n", ahd->serialNum);
        printf("refresh interval: %u\n", ahd->refreshInterval);
        printf("retry interval: %u\n", ahd->retryInterval);
        printf("expire limit: %u\n", ahd->expireLimit);
        printf("minimum ttl: %u\n", ahd->minimumTtl);
    } else if (ahd->type == 2) {
        printf("ns name: %s\n", ahd->primaryNameSever);
    }
    return;
}



/* AuthoritativeDnsPackage: to read the authoritative Dns package  */
struct AuthoritativeDns *AuthoritativeDnsPackage (byte *data, byte *bsp)
{
    struct AuthoritativeDns *ahd = (struct AuthoritativeDns *) malloc(sizeof(struct AuthoritativeDns));
    struct linkedList l;
    
    initLinkedList(&l);
    
    data = readNameDnsPackage(ahd->name, 253, data, bsp, &l);
    if (strcmp(ahd->name, "") == 0)
        data++;

    memcpy(&(ahd->type), data, 2);
    flipData(&(ahd->type), 2);
    data += 2;

    memcpy(&(ahd->class), data, 2);
    flipData(&(ahd->class), 2);
    memcpy(ahd->classBytes, data, 2);
    data += 2;

    memcpy(&(ahd->ttl), data, 4);
    flipData(&(ahd->ttl), 4);
    data += 4;

    memcpy(&(ahd->dataLength), data, 2);
    flipData(&(ahd->dataLength), 2);
    data += 2;


    if (ahd->type == 6) {
        data = readNameDnsPackage(ahd->primaryNameSever, 253, data, bsp, &l);
    

        data = readNameDnsPackage(ahd->authorityMailBox, 253, data, bsp, &l);

        memcpy(&(ahd->serialNum), data, 4);
        flipData(&(ahd->serialNum), 4);
        data += 4;
        
        memcpy(&(ahd->refreshInterval), data, 4);
        flipData(&(ahd->refreshInterval), 4);
        data += 4;
        
        memcpy(&(ahd->retryInterval), data, 4);
        flipData(&(ahd->retryInterval), 4);
        data += 4;

        memcpy(&(ahd->expireLimit), data, 4);
        flipData(&(ahd->expireLimit), 4);
        data += 4;
        
        memcpy(&(ahd->minimumTtl), data, 4);
        flipData(&(ahd->minimumTtl), 4);
        data += 4;
        
    } else if (ahd->type == 1) {
        memcpy(ahd->address, data, ahd->dataLength);
        data += ahd->dataLength;
    } else if (ahd->type == 2) {
        data = readNameDnsPackage(ahd->primaryNameSever, 253, data, bsp, &l);
    }

    
    ahd->print = &printAuthoritativeDnsPackage;    
    ahd->data = data;

    return ahd;
}



void printAdditionalDnsPackage (struct AdditionalDns *add)
{
    puts("\nAdditional:");
    printf("name: %s\n", (strcmp(add->name, "") == 0) ? "Root" : add->name);
    printf("type: %s (%u)\n", getTypeOfDnsSec(add->type), add->type);
    printf("UDP payload size: %u\n", add->udpLength);
    printf("length: %u\n", add->dataLength);
    if (add->type == 1)
        printIpv4(add->address, "A");
    else if (add->type == 28)
        printIpv6(add->address, "AAAA");
}




/* AdditionLDnsPackage: IS the section of the additional package dns */
struct AdditionalDns *AdditionalDnsPackage (byte *data, byte *bsp)
{
    struct AdditionalDns *add = (struct AdditionalDns *) malloc(sizeof(struct AdditionalDns));

    struct linkedList l;
    initLinkedList(&l);

    data = readNameDnsPackage(add->name, 253, data, bsp, &l);

    memcpy(&(add->type), data, 2);
    flipData(&(add->type), 2);
    data += 2;

    memcpy(&(add->udpLength), data, 2);
    flipData(&(add->udpLength), 2);
    data += 6;

    memcpy(&(add->dataLength), data, 2);
    flipData(&(add->dataLength), 2);
    data += 2;

    if (add->type == 1 || add->type == 28) {
        memcpy(add->address, data, add->dataLength);
        data += add->dataLength;
    }
    
    add->print = &printAdditionalDnsPackage;
    
    add->data = data;
    
    return add;
}


/* readDnsPackage: To read the protocol dns */
void readDnsPackage (struct Dns *d, byte *data)
{
    byte len;
    unsigned short l;
    int index;

    d->amountPayload = 0;
    
    /* First We read the id transaction */
    memcpy(d->idTransaction, data, 2);
    data += 2;

    /* Now read the flags */
    memcpy(d->flags, data, 2);
    data += 2;

    /* Now we read the numbers */
    memcpy(&(d->questionCount), data, 2);
    data += 2;
    flipData(&(d->questionCount), 2);
    d->amountPayload += d->questionCount;

    memcpy(&(d->answerCount), data, 2);
    data += 2;
    flipData(&(d->answerCount), 2);
    d->amountPayload += d->answerCount;

    memcpy(&(d->authorizationCount), data, 2);
    data += 2;
    flipData(&(d->authorizationCount), 2);
    d->amountPayload += d->authorizationCount;

    memcpy(&(d->additionalCount), data, 2);
    data += 2;
    flipData(&(d->additionalCount), 2);
    d->amountPayload += d->additionalCount;

    /* Here I save the pointer */
    d->data = data;

    /* Now I create an array */
    d->payload = (union DnsPayload *) malloc(sizeof(union DnsPayload) * d->amountPayload);
    d->types = (enum PAYLOADS_TYPES *) malloc(sizeof(enum PAYLOADS_TYPES) * d->amountPayload);
    memset(d->types, 0, sizeof(enum PAYLOADS_TYPES) * d->amountPayload);

    index = 0;
    
    /* Now read the data of the questions */
    l = d->questionCount;
    while (l--) { /* l > 0 */
        d->payload[index++].questionsDns = QuestionsDnsPackage(data, d->bsp);
        data = d->payload[index - 1].questionsDns->data;
        d->types[index - 1] = QUESTIONS_DNS; /* Here We put a question */
    }

    /* Now read the data of the answers */
    l = d->answerCount;
    while (l--) {
        d->payload[index++].answerDns = AnswerDnsPackage(data, d->bsp);
        data = d->payload[index - 1].answerDns->data;
        d->types[index - 1] = ANSWER_DNS;
    }

    l = d->authorizationCount;
    while (l--) {
        d->payload[index++].authoritativeDns = AuthoritativeDnsPackage(data, d->bsp);
        data = d->payload[index - 1].authoritativeDns->data;
        d->types[index - 1] = AUTHORIZATION_DNS;
    }

    l = d->additionalCount;
    while (l--) {
        d->payload[index++].additionDns = AdditionalDnsPackage(data, d->bsp);
        data = d->payload[index - 1].additionDns->data;
        d->types[index - 1] = ADDITIONAL_DNS;
    }
    
}


void printDnsFlags (const struct Dns *d)
{
    printf("\nFlags: \n");
    /* Now print the flags */
    if (d->flags[1] & 0b10000000)
        puts("\t1 bit 0 response");
    else
        puts("\t1 bit 0 query");
    
    if (d->flags[1] & 0b00100000)
        puts("\t2-5 bits 0100 Inverse ");
    else if (d->flags[1] & 0b00010000 || d->flags[1] & 0b0001000)
        puts("\t2-5 bits 0010 or 0001 not used ");
    else
        puts("\t2-5 bits 0000 Standart query");
    if (d->flags[1] & 0b000000100)
        puts("\t6 bit 1 authoritative DNS answer");
    else
        puts("\t6 bit 0 non-authoritative DNS answer");

    if (d->flags[1] & 0b00000010)
        puts("\t7 bit 1 Message truncated");
    else
        puts("\t7 bit 0 Message is not truncated ");

    if (d->flags[1] & 0b00000001)
        puts("\t8 bit 1 Recursive available");
    else
        puts("\t8 bit 0 Non-recursive query");

    if (d->flags[2] & 0b10000000)
        puts("\t9 bit 1 Recursion available");
    else
        puts("\t9 bit 0 Recursion not available");
    
    if (d->flags[2] & 0b00100000)
        puts("\t11 bit 1 Answer / authority position was authenticated by the server");
    else
        puts("\t11 bit 0 Answer / authority position was not authenticated by the server");
    if (d->flags[2] & 0b00000100)
        puts("\t13-16 bits 0100 Format error in query");
    else if (d->flags[2] & 0b0000010)
        puts("\t13-16 bits 0010 Server Failure");
    else if (d->flags[2] & 0b0000001)
        puts("\t13-16 bits 0001 Name does not found");
    else
        puts("\t13-16 bits 0000 No error");
}



/* printDnsPayload: To print the payload message */
void printDnsPayload (const struct Dns *d)
{
    int index;

    for (index = 0; index < d->amountPayload; index++) {
        switch (d->types[index]) {
        case QUESTIONS_DNS:
            d->payload[index].questionsDns->print(d->payload[index].questionsDns);
            break;
        case ANSWER_DNS:
            d->payload[index].answerDns->print(d->payload[index].answerDns);
            break;
        case AUTHORIZATION_DNS:
            d->payload[index].authoritativeDns->print(d->payload[index].authoritativeDns);
            break;
        case ADDITIONAL_DNS:
            d->payload[index].additionDns->print(d->payload[index].additionDns);
            break;
        }
    }
}


/* printDnsPackage: To print the package */
void printDnsPackage (const struct Dns *d)
{
	puts("---------------------------------------");
    puts("| DNS |\n");
    printf("Transaction Id: ");
    printHex(d->idTransaction, 1);
    printDnsFlags(d);
    printf("Questions: %u\n", d->questionCount);
    printf("Answer RRs: %u\n", d->answerCount);
    printf("Authority: %u\n", d->authorizationCount);
    printf("Additional RRs: %u\n", d->additionalCount);

    /* And now just print the print the dns payloads */
    printDnsPayload(d);
    
	puts("---------------------------------------");
}

/* deconstructdnsPackage: Is the package to deconstruct */
void deconstructDnsPackage (struct Dns *d)
{
    int index;

    /* Free all the payloads */
    for (index = 0; index < d->amountPayload; index++) {
        switch (d->types[index]) {
        case QUESTIONS_DNS:
            free(d->payload[index].questionsDns);
            break;
        case ANSWER_DNS:
            free(d->payload[index].answerDns->rddata); /* free the dynamic data */
            free(d->payload[index].answerDns);
            break;
        case AUTHORIZATION_DNS:
            free(d->payload[index].authoritativeDns);
            break;
        case ADDITIONAL_DNS:
            free(d->payload[index].additionDns);
            break;
        }
    }

    /* Free all the lists and objects */
    free(d->payload);
    free(d->types);
    free(d);
}

struct Dns *DnsPackage (byte *data)
{
    struct Dns *d = (struct Dns *) malloc(sizeof(struct Dns));

    d->bsp = data;
    readDnsPackage(d, data);
    
    d->deconstruct = &deconstructDnsPackage;
    d->print = &printDnsPackage;
    
    return d;
}

