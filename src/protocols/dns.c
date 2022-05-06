#include "../../include/protocols/dns.h"

/* readNameDnsPackage: Is double pointer to the data pointer */
byte *readNameDnsPackage (char *domainName, byte *data, byte *bsp)
{
    byte len;
    const byte dot = '.';

    memset(domainName, 0, 253);

    len = 1;
    while (len != 0) {
        memcpy(&len, data, 1);
        data++;

        if (len == 0)
            break;
        
        if (len == 0xc0) { /* pointer */
            /* get the lenght */
            memcpy(&len, bsp + *(data), 1);

            /* Read the data from the pointer */
            memcpy(domainName, bsp + *(data) + 1, len);
            data++; /* just increment one because is where the pointer needs to jump */
            domainName += len;

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
    printf("type: %s\n", (qd->type == 1) ? "A (Host Address) (1)" : "NULL");
    printf("class: %s", (qd->class == 1) ? "IN (" : "NULL (");
    printHex(qd->classBytes, 1);
    puts(")");
}


/* To readn and return the questions dns structure */
struct QuestionsDns *QuestionsDnsPackage (byte *data, byte *bsp)
{
     struct QuestionsDns *qd = (struct QuestionsDns *) malloc(sizeof(struct QuestionsDns));

     /* first read the name */
     data = readNameDnsPackage(qd->domainName, data, bsp);

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
    puts("\nAnswer: ");
    printf("name: %s\n", ad->domainName);
    printf("type: %s\n", (ad->type == 1) ? "A (Host Address) (1)" : "NULL");
    printf("class: %s", (ad->class == 1) ? "IN (" : "NULL (");
    printHex(ad->classBytes, 1);
    puts(")");
    printf("ttl: %u\n", ad->ttl);
    printf("rdlenght: %u\n", ad->rdLenght);
}


/* AnswerDnsPackage:  */
struct AnswerDns *AnswerDnsPackage (byte *data, byte *bsp)
{
    struct AnswerDns *ad = (struct AnswerDns *) malloc(sizeof(struct AnswerDns));
    
    /* first read the name */
    
    data = readNameDnsPackage(ad->domainName, data, bsp) - 1;

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
    

    ad->print = &printAnswerDnsPackage;

    /* Track the pointer */
    ad->data = data;
    
    return ad;
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
            break;
        case ADDITIONAL_DNS:
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
            free(d->payload[index].answerDns);
            break;
        case AUTHORIZATION_DNS:
            break;
        case ADDITIONAL_DNS:
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

