//--file: main.h----------------------------------------------
//
#ifndef _libGoAuth_main_h_
#define _libGoAuth_main_h_


#include <auth/CGoAuth.h>
#include <auth/CGoAuthResponse.h>

#include <utils/UrlEncode.h>

#include <stdio.h>
#include <memory.h>
#include <string.h>

#include <nsapi.h>
#include <base/pblock.h>
#include <base/session.h>
#include <frame/req.h>
#include <frame/http.h>

int assignRedirect( pblock * pb, Session * sn, Request * rq, char * pszUrl, unsigned long cRequest ) ;
void goauth_terminate(void * parameter );
void goauth_log( char * psz );
int requestIsAdmin( const char * pszQry, unsigned long cRequest );
int performAdminRedirect( pblock * pb, Session * sn, Request * rq, char * pszQry, unsigned long cRequest );
int performAdminRequest(  pblock * pb, Session * sn, Request * rq, char * pszQry );
bool addResponseHeaders( pblock * pb, Session * sn, Request * rq, CGoAuthResponse & resp, bool * bRedirect );

extern "C" NSAPI_PUBLIC int goauth_init( pblock * pb, Session * sn, Request * rq );
extern "C" NSAPI_PUBLIC int goauth_response( pblock * pb, Session * sn, Request * rq );
extern "C" NSAPI_PUBLIC int goauth_auth( pblock * pb, Session * sn, Request * rq );
extern "C" NSAPI_PUBLIC int goauth_admin( pblock * pb, Session * sn, Request * rq );

#endif
