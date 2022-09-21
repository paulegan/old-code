/* printLocation - a simple redirect handler			*/

/* paul.egan@disney.com 20010721			*/

/*  
   Example use:
    Init fn=load-modules shlib=/printLocation.so funcs=printLocation
    Error fn=printLocation code=302
*/

#include "nsapi.h"

#define PROTOCOL "http://"
#define SUFFIX   "/"

int printLocation(pblock *pb, Session *sn, Request *rq)
{
 char  *host   = pblock_findval("host",rq->headers);
 char  *path   = pblock_findval("uri",rq->reqpb);
 size_t locLen = strlen(PROTOCOL)+strlen(host)+strlen(path)+strlen(SUFFIX)+1;
 char  *loc    = MALLOC(locLen);
 snprintf(loc,locLen,"%s%s%s%s",PROTOCOL,host,path,SUFFIX);
 pblock_nvinsert("Location", loc, rq->srvhdrs);
 FREE(loc);
 protocol_status(sn, rq, PROTOCOL_REDIRECT, NULL);
 return (protocol_start_response(sn, rq));
}

