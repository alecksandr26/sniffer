#ifndef __DNS_H__
#define __DNS_H__

#include <stdlib.h>
#include <string.h>

#include "../../include/helpers/helpers.h"

enum PAYLOADS_TYPES {
    QUESTIONS_DNS,
    ANSWER_DNS,
    AUTHORIZATION_DNS,
    ADDITIONAL_DNS
};

/* This is the structure of the Dns protrocol */
struct QuestionsDns {
    char domainName[253];
    unsigned short type;
    unsigned short class;
    byte classBytes[2];

    byte *data;
    
    /* also it has its print function */
    void (*print) (struct QuestionsDns *);
};

/* the dns can has these packages */
union DnsPayload {
    struct QuestionsDns *questionsDns;
};

struct Dns {
    byte idTransaction[2];
    byte flags[2];
    int amountPayload;
    unsigned short questionCount;
    unsigned short answerCount;
    unsigned short authorizationCount;
    unsigned short additionalCount;


    byte *data;
    
    union DnsPayload *payload;
    enum PAYLOADS_TYPES *types;
    
    
    void (*print) (struct Dns *);
    void (*deconstruct) (struct Dns *);
};


struct Dns *DnsPackage (byte *data);

typedef struct Dns Dns;


#endif





