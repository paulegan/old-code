
/**********************************************************************
 *
 * ID:		$Id$
 *
 * Author:	paul.egan@disney.com
 * 		2001/11/01
 *
 * Last update:	$Author$
 * 		$Date$
 *
 * Description:	NSAPI SAF to add a HTTP header to the server response.
 *		Returns REQ_NOACTION so request is processed as normal.
 *
 *		Init fn=load-modules shlib=/libAddHttpHeader.so funcs=add-http-header
 *		Service fn=add-http-header name=custom value=foo
 *
 * Log:		$Log: $
 * Log:
 *
 **********************************************************************/

#include "nsapi.h"

NSAPI_PUBLIC int add_http_header(pblock *pb, Session *sn, Request *rq)
{
 char *name = pblock_findval("name",pb);
 char *value = pblock_findval("value",pb);
 if (name && value){
   param_free(pblock_remove(name, rq->srvhdrs));
   pblock_nvinsert(name, value, rq->srvhdrs);
 }
 return (REQ_NOACTION);
}

