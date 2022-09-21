
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
 * Description:	NSAPI SAF to strip the port number from the url param in
 *		vars pblock - used if server is NAT'd with port translation.
 *		Returns REQ_NOACTION so request is processed as normal.
 *
 *		Init fn=load-modules shlib=/libStripPort.so funcs=strip-port
 *		Error fn=strip-port code=302
 *
 * Log:		$Log: $
 * Log:
 *
 **********************************************************************/

#include "nsapi.h"

NSAPI_PUBLIC int strip_port(pblock *pb, Session *sn, Request *rq)
{
 char *a,*b,*c;
 pb_param *url = pblock_find("url",rq->vars);
 if (url && url->value && (a=strstr(url->value,"://"))){
   if ((b=strchr(a+3,':')) && (c=strchr(a+3,'/')) && (b<c)){
     while (*c) *b++ = *c++;
     *b = 0;
   }
 }
 return (REQ_NOACTION);
}

