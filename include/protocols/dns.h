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

/* This is the strcuture for the question dns */
struct QuestionsDns {
    char domainName[253];
    unsigned short type;
    unsigned short class;
    byte classBytes[2];

    byte *data;
    
    /* also it has its print function */
    void (*print) (struct QuestionsDns *);
};


/* This is the structure for the answers that the dns could have */
struct AnswerDns {
    char domainName[253];
    unsigned short type;
    unsigned short class;
    byte classBytes[2];

    unsigned int ttl;
    unsigned short rdLenght;
    byte *rddata; /* can be any amount of data */
    
    byte *rawdata; /* This depends of the type can be ipv4 or an alias etc. */

    /* for type 15 mx */
    unsigned short preference;
    char name[253];
    
    /* To have track of the package */
    byte *data;

    /*also it has its print fucntion */
    void (*print) (struct AnswerDns *);
};

struct AdditionalDns {
    char name[253];
    unsigned short type;
    unsigned short udpLength;
    unsigned short dataLength;

    /* for type 1 */
    byte address[16];

    /* to track the pointer */
    byte *data;

    void (*print) (struct AdditionalDns *);
};


struct AuthoritativeDns {
    char name[253];
    unsigned short type;
    unsigned short class;
    byte classBytes[2];
    unsigned ttl;
    unsigned short dataLength;

    /* for the SOA start of a zone of athority */
    char primaryNameSever[253];
    char authorityMailBox[253];
    unsigned serialNum;
    unsigned refreshInterval;
    unsigned retryInterval;
    unsigned expireLimit;
    unsigned minimumTtl;

    /* for the ipv4 or ipv6 */
    byte address[16];

    byte *data;
    void (*print) (struct AuthoritativeDns *);
};


/* the dns can has these packages */
union DnsPayload {
    struct QuestionsDns *questionsDns;
    struct AnswerDns *answerDns;
    struct AdditionalDns *additionDns;
    struct AuthoritativeDns *authoritativeDns;
};


/* This is the structure of the Dns protrocol */
struct Dns {
    byte idTransaction[2];
    byte flags[2];
    int amountPayload;
    unsigned short questionCount;
    unsigned short answerCount;
    unsigned short authorizationCount;
    unsigned short additionalCount;
    
    byte *bsp; /* base pointer fo the dns package */
    
    byte *data;
    
    union DnsPayload *payload;
    enum PAYLOADS_TYPES *types;
    
    
    void (*print) (struct Dns *);
    void (*deconstruct) (struct Dns *);
};


struct Dns *DnsPackage (byte *data);

typedef struct Dns Dns;


#endif





