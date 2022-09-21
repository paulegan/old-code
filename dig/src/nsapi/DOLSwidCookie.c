/********************************************************************\
 * This is the Disney Online Swid Painter.
 ********************************************************************
 *
 * $Id$
 *
 * $Log: DOLSwidCookie.c,v $
 * Revision 1.3  2000/01/31 23:44:57  yoshi
 * Added QueryString.
 *
 * Revision 1.2  1999/12/22 23:39:04  yoshi
 * Removed some extraneous logging and added Host header support
 *
 * Revision 1.1  1999/12/21 21:11:40  yoshi
 * initial checkin
 *
\********************************************************************/

/*
 * To install:
 * Put into obj.conf
 *	Init fn="load-modules" shlib="DOLSwidCookie.so" \
 *		funcs="DOLSwidCookie-init,DOLSwidCookie-service,DOLSwidCookie-log"
 *
 *	Init fn="DOLSwidCookie-init" server-name="FOO1" server-ip="127.0.0.1" \
 *		cookie-name="SWID" cookie-expires="31536000" cookie-domain=".disney.com" \
 *		logname="/opt/suitespot/https-cheetah/logs/swid_cookie.log" \
 *		debug=1 debuglogname="/opt/suitespot/https-cheetah/logs/swid_cookie_debug.log"
 *
 *	Service fn="DOLSwidCookie-service" method="(GET|HEAD)" type="text/html"
 *
 *	AddLog fn="DOLSwidCookie-log"
 */


#ifdef XP_WIN32
#define NSAPI_PUBLIC __declspec(dllexport)
#else /* !XP_WIN32 */
#define NSAPI_PUBLIC
#endif /* !XP_WIN32 */


/*
 * System includes
 */
#include <stdio.h>

#ifndef XP_WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif


/*
 * Standard NSAPI Includes
 */
#include <nsapi.h>


/*
 * Defines
 */
#define BUFSIZ 1024
#define COOKIE_PART_SIZE 128
#define LOG_SIZE 8192
#define MAX_COOKIE_SIZE 8192

#define TO_CONSOLE 0
#define TO_BROWSER 1

/*
 * Globals
 */
static int				debugging;

static SYS_FILE			debug_log_fd = SYS_ERROR_FD;
static SYS_FILE			log_fd = SYS_ERROR_FD; 

static char				default_server_name[ BUFSIZ ];
static char				server_ip[ BUFSIZ ];

static char				swid_cookie_name[ BUFSIZ ];
static char				swid_cookie_domain[ BUFSIZ ];
static unsigned long	swid_cookie_expires;

static const char day_of_week[7][4]  = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static const char month_of_year[12][4] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" 
};


/*
 * Function Prototypes
 */
NSAPI_PUBLIC int	DOLSwidCookie_init( pblock *pb, Session *sn, Request *rq );
NSAPI_PUBLIC void	DOLSwidCookie_cleanup( void *unused );

NSAPI_PUBLIC int	DOLSwidCookie_service( pblock *pb, Session *sn, Request *rq );
NSAPI_PUBLIC int	DOLSwidCookie_log( pblock *pb, Session *sn, Request *rq );

pblock				*parse_cookie( const char *cookie );
int					ns_print( Session *sn, Request *rq, char *buffer );
int  				print_pblock( int dest, pblock *pb, Session *sn, Request *rq, char *prefix );
void				debug_log( Session *sn, Request *rq, const char *message );
/*  struct tm			*get_gmtoff( long *tz ); */
void				get_date_time( char *the_date, char* the_time );

/*
 * DOLSwidCookie_init
 *	Init fn="DOLSwidCookie-init" server-name="FOO1" server-ip="" \
 *		debug=1 debug-log-name="/opt/suitespot/https-cheetah/logs/swid_cookie_debug.log" \
 *		cookie-name="SWID" cookie-expires="31536000" cookie-domain=".go.com" \
 *		log-name="/opt/suitespot/https-cheetah/logs/swid_cookie.log" \
 */
#ifdef __cplusplus
extern "C"
#endif
NSAPI_PUBLIC int
DOLSwidCookie_init( pblock *pb, Session *sn, Request *rq ) {
	char		*debug = pblock_findval( "debug", pb );
	char		*debug_log_name = pblock_findval( "debug-log-name", pb );

	char		*r_server_name = pblock_findval( "server-name", pb );
	char		*r_server_ip = pblock_findval( "server-ip", pb );

	char		*r_cookie_name = pblock_findval( "cookie-name", pb );
	char		*r_cookie_domain = pblock_findval( "cookie-domain", pb );
	char		*r_cookie_expires = pblock_findval( "cookie-expires",pb );

	char		*log_name = pblock_findval( "log-name", pb );

	magnus_atrestart( DOLSwidCookie_cleanup, NULL );

	if( debug != NULL ) {
		debugging = atoi( debug );
	}

	if( debugging ) {
		if( debug_log_name == NULL ) {
			pblock_nvinsert( "error", "DOLSwidCookie_init: Debugging requested with no debug log",
							 pb );
			return REQ_ABORTED;
		}
		
		debug_log_fd = system_fopenWA( debug_log_name );

		if( debug_log_fd == SYS_ERROR_FD ) {
			pblock_nvinsert( "error", "DOLSwidCookie_init: Couldn't open debug log for writing",
							 pb );
			return REQ_ABORTED;
		}
	}

	if( r_server_name == NULL ) {
		pblock_nvinsert( "error", "DOLSwidCookie_init: Server name not specified",
						 pb );
		return REQ_ABORTED;
	} else {
		strncpy( default_server_name, r_server_name, BUFSIZ );
	}

	if( r_server_ip == NULL ) {
		pblock_nvinsert( "error", "DOLSwidCookie_init: Server ip not specified",
						 pb );
		return REQ_ABORTED;
	} else {
		strncpy( server_ip, r_server_ip, BUFSIZ );
	}


	if( r_cookie_name == NULL ) {
		pblock_nvinsert( "error", "DOLSwidCookie_init: Cookie name not specified",
						 pb );
		return REQ_ABORTED;
	} else {
		strncpy( swid_cookie_name, r_cookie_name, BUFSIZ );
	}

	if( r_cookie_domain == NULL ) {
		pblock_nvinsert( "error", "DOLSwidCookie_init: Cookie domain not specified",
						 pb );
		return REQ_ABORTED;
	} else {
		strncpy( swid_cookie_domain, r_cookie_domain, BUFSIZ );
	}

	if( r_cookie_expires == NULL ) {
		pblock_nvinsert( "error", "DOLSwidCookie_init: Cookie expire time not specified",
						 pb );
		return REQ_ABORTED;
	} else {
		swid_cookie_expires = atol( r_cookie_expires );
	}
		
	if( log_name == NULL ) {
		pblock_nvinsert( "error", "DOLSwidCookie_init: No log file specified",
						 pb );
		return REQ_ABORTED;
	} else {
		log_fd = system_fopenWA( log_name );
	
		if( log_fd == SYS_ERROR_FD ) {
			pblock_nvinsert( "error", "DOLSwidCookie_init: Couldn't open log for writing",
							 pb );
			return REQ_ABORTED;
		}
	}

	return REQ_PROCEED;
}

/*
 * DOLSwidCookie_cleanup
 */
void
DOLSwidCookie_cleanup( void *unused ) {
	/* cleanup the logging stuff */

	if( debug_log_fd != SYS_ERROR_FD ) {
		system_fclose( debug_log_fd );
	}
	debug_log_fd = SYS_ERROR_FD;

	if( log_fd != SYS_ERROR_FD ) {
		system_fclose( log_fd );
	}
	log_fd = SYS_ERROR_FD;
}

/*********************************************************************
 *
 * DOLSwidCookie_service
 *
 *********************************************************************/
#ifdef __cplusplus
extern "C"
#endif
NSAPI_PUBLIC int
DOLSwidCookie_service( pblock *pb, Session *sn, Request *rq ) {
	int			inject_swid = 1;

	char		*cookie;
	pblock		*cookie_pblock;
	char		*swid_cookie = NULL;

	char		client_cookie[ MAX_COOKIE_SIZE ] = { '\0' };
	char		cookie_buffer[ BUFSIZ * 6 ] = { '\0' };
	char		swid_buffer[ BUFSIZ ] = { '\0' };
	char		expire_buffer[ BUFSIZ ] = { '\0' };

	time_t		now;
	struct tm	the_tm;

	if( debugging ) debug_log( sn, rq, "Starting SWID Service" );

	/*
	 * Do some cookie detection and determination if we have to
	 * construct the SWID
	 */
	cookie = pblock_findval( "cookie", rq->headers );

	/* make a copy because parse_cookie is destructive */
	if( cookie != NULL ) {
		util_snprintf( client_cookie, MAX_COOKIE_SIZE, "%s", cookie );
		cookie_pblock = parse_cookie( client_cookie );

		if( cookie_pblock != NULL ) {
			swid_cookie = pblock_findval( swid_cookie_name, cookie_pblock );
			if( swid_cookie != NULL ) {
				inject_swid = 0;
			}
		}
	}

	/*
	 * Inject the SWID cookie if needed
	 */
	if( inject_swid == 1 ) {
		/* Construct the SWID (taken from starwave code */
		util_snprintf( swid_buffer, BUFSIZ,
					   "%04X%04X-%04X-%04X-%04X-%04X%04X%04X",
					   rand(), rand(), rand(), rand(),
					   rand(), rand(), rand(), rand() );
		
		/* Construct the Expire Time */

		/* Expires a year from now */
		/* now = time( NULL ) + 31536000; */
		now = time( NULL ) + swid_cookie_expires;

		/* Format the expires time */
		if( gmtime_r( &now, &the_tm ) != NULL ) {
			util_snprintf( expire_buffer, BUFSIZ,
						   "%s, %02u-%s-%02u %02u:%02u:%02u GMT",
						   day_of_week[ the_tm.tm_wday ], the_tm.tm_mday,
						   month_of_year[ the_tm.tm_mon ], the_tm.tm_year + 1900,
						   the_tm.tm_hour, the_tm.tm_min, the_tm.tm_sec );
		}

		/*
		 * Format the cookie
		 */
		util_snprintf( cookie_buffer, BUFSIZ * 6,
					   "%s=%s; path=/; expires=%s; domain=%s;",
					   swid_cookie_name, swid_buffer,
					   expire_buffer, swid_cookie_domain );
		
		if( debugging ) debug_log( sn, rq, cookie_buffer );

		pblock_nvinsert( swid_cookie_name, swid_buffer, rq->vars );
		pblock_nvinsert( "set-cookie", cookie_buffer, rq->srvhdrs );
	}

	return( REQ_NOACTION );
}

/*********************************************************************
 *
 * DOLSwidCookie_log
 *
 *********************************************************************/
#ifdef __cplusplus
extern "C"
#endif
NSAPI_PUBLIC int
DOLSwidCookie_log( pblock *pb, Session *sn, Request *rq ) {
	char	log_buffer[ LOG_SIZE ] = { '\0' };

	char	unavailable[] = "-";

	char	*remote_host = pblock_findval( "ip", sn->client );
	char	*username = unavailable;
	char	date[ 128 ] = { '\0' };
	char	time[ 128 ] = { '\0' };

	char	*protocol = pblock_findval( "protocol", rq->reqpb );
	char	*status = pblock_findval( "status", rq->srvhdrs );
	char	*method = pblock_findval( "method", rq->reqpb );

	char	*server_name = pblock_findval( "host", rq->headers );

	char	*content_length = pblock_findval( "content-length", rq->srvhdrs );

	char	*uri = pblock_findval( "uri", rq->reqpb );
	char	*query = pblock_findval( "query", rq->reqpb );

	char	*user_agent = pblock_findval( "user-agent", rq->headers );
	char	*referer = pblock_findval( "referer", rq->headers );

	char	status_id[ 128 ] = { '\0' };
	char	status_code[ 128 ] = { '\0' };

	char	client_cookie[ MAX_COOKIE_SIZE ] = { '\0' };
	char	*cookie;
	pblock	*cookie_pblock;
	char	*swid_cookie = NULL;
	char	swid_cookie_buffer[ BUFSIZ ] = { '\0' };

	get_date_time( date, time );

	if( server_name == NULL ) {
		server_name = default_server_name;
	}

	if( content_length == NULL ) {
		content_length = unavailable;
	}

	sscanf( status, "%s %s", status_id, status_code );

	if( user_agent == NULL ) {
		user_agent = unavailable;
	}

	if( referer == NULL ) {
		referer = unavailable;
	}

	if( query == NULL ) {
		query = unavailable;
	}

	/* Attempt 1: try to find the swid cookie in the cookie header */
	cookie = pblock_findval( "cookie", rq->headers );
	if( cookie != NULL ) {
		/* parse_cookie is desctructive, so we need to make a copy */
		util_snprintf( client_cookie, MAX_COOKIE_SIZE, "%s", cookie );
		cookie_pblock = parse_cookie( client_cookie );

		if( cookie_pblock != NULL ) {
			swid_cookie = pblock_findval( swid_cookie_name, cookie_pblock );
		}
	}

	/* Attempt 2: try to find the swid cookie in the request variables */
	if( swid_cookie == NULL ) {
		swid_cookie = pblock_findval( swid_cookie_name, rq->vars );
	}

	/* Result: set to unavailable if unavailable.... duh. */
	if( swid_cookie == NULL || ( strcmp( status_id, "304" ) == 0 ) ) {
		util_snprintf( swid_cookie_buffer, BUFSIZ, "%s", unavailable );
	} else {
		util_snprintf( swid_cookie_buffer, BUFSIZ, "%s=%s", swid_cookie_name, swid_cookie );
	}

	/*
	 * Put all the log pieces together.
	 */
	util_snprintf( log_buffer, LOG_SIZE,
				   "%s, %s, %s, %s, W3SVC, %s, %s, 100, 100, %s, %s, 0, %s, %s, %s, %s, %s, %s,\n",
				   remote_host, username, date, time,
				   /* protocol, */
				   server_name, server_ip,
				   content_length,
				   status_id, method, uri,
				   user_agent, referer, swid_cookie_buffer, query );

	/*
	 * Output the log entry in all it's splendor.
	 */
	system_fwrite_atomic( log_fd, log_buffer, strlen( log_buffer ) );

	return REQ_PROCEED;
}

/*
 * parse_cookie
 */
pblock *
parse_cookie( const char *cookie ) {
	char	*name, *value;
    char	*ptr;
    pblock	*cookie_pblock;
    int		fInValue = 0;
    int		done = 0;

    if (!cookie)
        return NULL;

    cookie_pblock = pblock_create(5);

    ptr = (char *)cookie;
    while( *ptr && isspace( (int)*ptr))
        ptr++;
    name = ptr;
    while(!done) {
        switch(*ptr) {
		case '=':
			if (!fInValue) {
				*ptr = '\0';
				ptr++;
				while(*ptr && isspace((int)*ptr)) 
					ptr++;
				value = ptr;
				fInValue = 1;
			}
			break;
		case '\0':
		case ';':
			if (fInValue) {
				if (*ptr) {
					*ptr = '\0';
					ptr++;
					while(*ptr && isspace((int)*ptr)) 
						ptr++;
					if (!ptr)    /* don't go past edge */
						ptr--;
				} else  
					done = 1;
				pblock_nvinsert(name, value, cookie_pblock);
				name = ptr;
				fInValue = 0;
			}
			break;
        }
        ptr++;
    }

    return cookie_pblock;
}



void
debug_log( Session *sn, Request *rq, const char *message ) {
	char	log_buffer[BUFSIZ];

	char	*remote_host;

	char	the_date[BUFSIZ];
	char	the_time[BUFSIZ];

	char	*uri = pblock_findval( "uri", rq->reqpb );

	char	unavailable[] = "-";

	/*
	 * Obtain the remote_host.
	 * Provide the opprotunity to get use the resolved name too. Later.
	 */
 
	if( ( remote_host = pblock_findval( "ip", sn->client ) ) == NULL ) {
		remote_host = unavailable;
	}

	get_date_time( the_date, the_time );

	util_snprintf( log_buffer, BUFSIZ, "%s %s %s %s %s\n",
				   remote_host, the_date, the_time, uri, message );
	
	/*
	 * Output the log entry in all it's splendor.
	 */
	system_fwrite_atomic( debug_log_fd, log_buffer, strlen( log_buffer ) );
}

/*
 * This routine gets the current timezone and the current time.
 * It originally appeared in the Apache 1.0 source code.
 */
/*  struct tm * */
/*  get_gmtoff( long *tz, struct tm *the_tm) { */
/*  	time_t tt; */
/*  	tt = time( NULL ); */
/*  	the_tm = util_localtime( &tt, the_tm ); */
/*  	*tz = - timezone; */
/*  	if( the_tm->tm_isdst ) */
/*  		*tz += 3600; */
/*  	return t; */
/*  } */

/*
 * This routine populates the given string with the curerent date and time.
 * I put it here cuz, the logging routine was getting kinda cluttered.
 */
void
get_date_time( char *the_date, char *the_time ) {
	long the_timezone;
	struct tm *the_tm;
	char the_sign;
	char the_date_buffer[ 128 ];
	char the_time_buffer[ 128 ];

	time_t		cur_time;

	the_tm = system_malloc( sizeof( struct tm ) );

	cur_time = time( NULL );

	the_tm = util_localtime( &cur_time, the_tm );

	the_timezone = - timezone;

	if( the_tm->tm_isdst ) {
		the_timezone += 3600;
	}

	if( the_timezone < 0 ) {
		the_sign = '-';
		the_timezone = - the_timezone;
	} else {
		the_sign = '+';
	}

	util_strftime( the_date_buffer, "%m/%d/%y", the_tm );
	util_sprintf( the_date, "%s", the_date_buffer );

	util_strftime( the_time_buffer, "%H:%M:%S", the_tm );
	util_sprintf( the_time, "%s", the_time_buffer );

	system_free( the_tm );
}

/*
 * This routine prints up debugging information onto the console.
 * Although the output is not necessarily what you get (sometimes
 * there is a name that exists but does not appear in the pblock),
 * its pretty nice to see what you are dealing with in the server
 * functions.
 */
int
print_pblock( int dest, pblock *pb, Session *sn, Request *rq, char *prefix ) {
	char real_prefix[BUFSIZ];
	char buffer[BUFSIZ];
	int  i;

	if( prefix == NULL ) {
		strcpy( real_prefix, "PBLOCK:" );
	} else {
		strcpy( real_prefix, prefix );
	}

	for( i = 0; i < pb->hsize; i++ ) {
		if( pb->ht[i] != NULL ) {
			if( pb->ht[i]->param != NULL ) {
				util_snprintf( buffer, BUFSIZ, "%s%s=%s\n", real_prefix,
							   pb->ht[i]->param->name,
							   pb->ht[i]->param->value );
				
				switch( dest ) {
				case TO_CONSOLE:
					printf( "%s", buffer );
					break;
				case TO_BROWSER:
					if( ns_print( sn, rq, buffer ) == REQ_EXIT )
						return REQ_EXIT;
					break;
				}
			}
		}
	}
	return REQ_PROCEED;
}

/*
 * This routine sends out a line of text to the browser.
 */
int
ns_print( Session *sn, Request *rq, char *buffer ) {
	if ( net_write( sn->csd, buffer, strlen( buffer ) ) == IO_ERROR )
		return REQ_EXIT;

	return REQ_PROCEED;
}

