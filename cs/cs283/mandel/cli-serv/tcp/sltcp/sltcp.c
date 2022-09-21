/*
 * sltcp.c - TCP/IP Low level session layer routines
 * Robert Walsh & Gavin Doherty
 *
 * 25/5/94
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/file.h>
#include <ndbm.h>
#include <stdio.h>
#include <string.h>

#include <sltcp.h>
#include "sltcp_int.h"

/*
 * The list of user connections.
 */

static struct conn_list *conn_connections_list=NULL;

/*
 * The user preferences.
 */

static struct _conn_user_prefs conn_user_prefs = {
    NULL, NULL, NULL, NULL
};

/*
 * The read, write and exception file descriptor sets used by select().
 */

static fd_set readfds, writefds, exceptfds;

/*
 * Our equivilant of errno
 */

int slerrno;

/*
 * Create a passive connection, and listen for people trying to connect to it.
 */

int conn_attach(unsigned short portno)
{
    int sock;
    struct sockaddr_in server;
    conn_t *servcon;

    if((sock=socket(AF_INET, SOCK_STREAM, 0))<0){
	slerrno=(int)CE_SOCKET_FAIL;
	return -1;
    }
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(portno);
    if(bind(sock, &server, sizeof(server))){
	close(sock);
	slerrno=(int)CE_BIND_FAIL;
	return -1;
    }
    listen(sock, 5);
    FD_SET(sock, &readfds);
    if((servcon=conn_alloc())==NULL){
	close(sock);
	slerrno=(int)CE_MALLOC_FAIL;
	return -1;
    }
    servcon->conn_sd=sock;
    servcon->conn_read_status=CONN_SERVER;
    servcon->conn_write_status=CONN_SERVER;
    conn_addconnect(servcon);
    return servcon->conn_sd;
}

/*
 * Open a connection to a passive socket on another host.
 */

int conn_open(char *address, unsigned short portno)
{
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;
    conn_t *servcon;

    if((sock=socket(AF_INET, SOCK_STREAM, 0))<0){
	slerrno=(int)CE_SOCKET_FAIL;
	return -1;
    }
    if((hp=gethostbyname(address))==0){
	close(sock);
	slerrno=(int)CE_GETHOSTBYNAME_FAIL;
	return -1;
    }
    bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
    server.sin_family=AF_INET;
    server.sin_port=htons(portno);
    if(connect(sock, &server, sizeof(server))<0){
	close(sock);
	slerrno=(int)CE_CONNECT_FAIL;
	return -1;
    }
    if((servcon=conn_alloc())==NULL){
	close(sock);
	slerrno=(int)CE_MALLOC_FAIL;
	return -1;
    }
    servcon->conn_sd=sock;
    servcon->conn_read_status=CONN_CLIENT_OK;
    servcon->conn_write_status=CONN_CLIENT_OK;
    servcon->conn_addr=strdup(address);
    FD_SET(sock, &readfds);
    conn_addconnect(servcon);
    conn_write_head(servcon, CONN_MESG_CONNECT, (u_long)0);
    return servcon->conn_sd;
}

/*
 * Register a callback routine for a particular function (e.g. Data available
 * on a connection, user has closed connection, etc..)
 */

int conn_register(void (*fn)(), int cb)
{
    switch(cb){
    case IO_READ:
	conn_user_prefs.user_read=fn;
	break;
    case IO_ACCEPT:
	conn_user_prefs.user_accept=fn;
	break;
    case IO_ABORT:
	conn_user_prefs.user_abort=fn;
	break;
    case IO_CLOSE:
	conn_user_prefs.user_close=fn;
	break;
    default:
	slerrno=(int)CE_REGISTER_FAIL;
	return -1;
    }
    return 0;
}

/*
 * Send a piece of data down a connection. This is buffered, so it doesn't
 * write until it can.
 */

int conn_write(int cc, char *buf, int len)
{
    u_long wlen, temp[2], tlen;
    int wval;
    char *wbuf;
    conn_t *c;

    if((c=get_conn_ent(cc))==NULL){
	slerrno=(int)CE_UNKNOWN_SESSION;
	return -1;
    }

    /*
     * Change data to form suitable for sending
     */

    if(buf){
	int t;

	tlen=2*sizeof(u_long)+len;
	wbuf=(char *)malloc(tlen);
	temp[0]=htonl(CONN_MESG_DATA);
	temp[1]=htonl((u_long)len);
	bcopy(temp, wbuf, 2*sizeof(u_long));
	bcopy(buf, wbuf+2*sizeof(u_long), len);
	if(t=conn_queue(c, wbuf, tlen, 0)){

	    /*
	     * We've encountered an error in adding
	     * stuff to the queue. Return to the user.
	     * Should we really close the connection
	     * down too? We'll see in trial runs..
	     */

	    if(conn_user_prefs.user_abort)
		conn_user_prefs.user_abort(cc);
	    conn_iclose(c);
	    slerrno=t;
	    return -1;
	}
    }

    /*
     * Write out data queued to this connection
     */

    while(c->conn_outgoing){
	int lpos, llen;
	struct conn_q *temp;

	lpos=c->conn_outgoing->pos;
	llen=c->conn_outgoing->len;
	wval=write(c->conn_sd, c->conn_outgoing->buf+lpos, llen-lpos);
	if(wval<0){        /* Some error has occured */
	    if(conn_user_prefs.user_abort)
		conn_user_prefs.user_abort(cc);
	    conn_iclose(c);
	    slerrno=(int)CE_WRITE_FAIL;
	    return -1;
	}
	if(wval+lpos<llen){
	    c->conn_write_status=CONN_CLIENT_WRITING;
	    c->conn_outgoing->pos+=wval;
	    FD_SET(c->conn_sd, &writefds);
	    slerrno=(int)CE_OK;
	    return 0;
	}
	temp=c->conn_outgoing;
	c->conn_outgoing=c->conn_outgoing->next;
	free(temp);
    }
    FD_CLR(c->conn_sd, &writefds);
    c->conn_write_status=CONN_CLIENT_OK;
    return 0;
}

/*
 * Queue some data for sending down a connection.
 */

static int conn_queue(conn_t *c, char *buf, int len, int pos)
{
    struct conn_q *outq;

    if((outq=ALLOC_CONN_Q())==NULL){
	slerrno=(int)CE_MALLOC_FAIL;
	return -1;
    }
    outq->buf=buf;
    outq->len=len;
    outq->pos=pos;
    outq->next=NULL;
    c->conn_write_status=CONN_CLIENT_WRITING;
    if(c->conn_outgoing){
	struct conn_q *tempq;

	for(tempq=c->conn_outgoing; tempq->next; tempq=tempq->next);
	tempq->next=outq;
    }else{
	c->conn_outgoing=outq;
    }
    return 0;
}

/*
 * Read some data from a connection. Won't return the buffer until it has
 * recieved all the data.
 */

static char *conn_read(conn_t *c)
{
    int rval, rlen;
    u_long type[2];
    char *buf;
	
    if(c->conn_read_status==CONN_CLIENT_OK){
	if((rlen=read(c->conn_sd, type, 2*sizeof(u_long)))<0){
	    if(conn_user_prefs.user_abort)
		conn_user_prefs.user_abort(c->conn_sd);
	    conn_iclose(c);
	    slerrno=(int)CE_READ_FAIL;
	    return (char *)-1;
	}
	if(rlen==0){
	    if(conn_user_prefs.user_abort)
		conn_user_prefs.user_abort(c->conn_sd);
	    conn_iclose(c);
	    return 0;
	}
	type[0]=ntohl(type[0]);
	type[1]=ntohl(type[1]);
	switch(type[0]){
	case CONN_MESG_SHUTDOWN: /* Connection closed by peer */
	    if(conn_user_prefs.user_close)
		conn_user_prefs.user_close(c->conn_sd);
	    conn_iclose(c);
	    break;
	case CONN_MESG_CONNECT:
	    conn_write_head(c, CONN_MESG_ACK, (u_long)c);
	    c->conn_id=(u_long)c;
	    if(conn_user_prefs.user_accept)
		conn_user_prefs.user_accept(c->conn_sd);
	    break;
	case CONN_MESG_RECONNECT:
	    {
		conn_t *old=conn_findconnect(type[1]);

		old->conn_sd=c->conn_sd;
		conn_removeconnect(c);
		if(conn_user_prefs.user_accept)
		    conn_user_prefs.user_accept(c->conn_sd);
	    }
	    break;
	case CONN_MESG_ACK:	/* login acknowledge */
	    c->conn_id=type[1];
	    return 0;
	case CONN_MESG_DATA:
	    if((buf=(char *)malloc(type[1]))==NULL){
		slerrno=(int)CE_MALLOC_FAIL;
		return (char *)-1;
	    }
	    if((rval=read(c->conn_sd, buf, (int)type[1])) <0){
		slerrno=(int)CE_READ_FAIL;
		return (char *)-1;
	    }
	    c->conn_mesg_len=(int)type[1];
	    if(rval!=(int)type[1]){ /* Not all data was received */
		c->conn_read_status=CONN_CLIENT_READING;
		c->conn_read_len=rval;
		c->conn_buf=buf;
		return NULL;
	    }
	    return buf;
	default:
	    slerrno=(int)CE_MESSAGE_FAIL;
	    return (char *)-1;
	}
	return 0;
    }else if(c->conn_read_status==CONN_CLIENT_READING){
	rval=c->conn_read_len;
	rlen=c->conn_mesg_len;
	if((rval+=read(c->conn_sd, buf+rval, rlen-rval))<0){
	    slerrno=(int)CE_READ_FAIL;
	    return (char *)-1;
	}
	c->conn_read_len=rval;
	if(rval!=rlen)		/* Not all data was received */
	    return 0;
	c->conn_read_status=CONN_CLIENT_OK;
	c->conn_buf=NULL;
    }else{
	if(conn_user_prefs.user_abort)
	    conn_user_prefs.user_abort(c->conn_sd);
	conn_iclose(c);
	slerrno=(int)CE_READ_FAIL;
	return (char *)-1;
    }
    return buf;
}

/*
 * Close a connection. Free all resources associated with it.
 */

int conn_close(int cc)
{
    conn_t *c;
	
    if((c=get_conn_ent(cc))==NULL){
	slerrno=(int)CE_UNKNOWN_SESSION;
	return -1;
    }
    return conn_iclose(c);
}

/*
 * The internal version of conn_close()
 */

static int conn_iclose(conn_t *c)
{
    FD_CLR(c->conn_sd, &readfds);
    FD_CLR(c->conn_sd, &writefds);
    FD_CLR(c->conn_sd, &exceptfds);
    conn_write_head(c, CONN_MESG_SHUTDOWN, (u_long)0);
    conn_removeconnect(c);
    close(c->conn_sd);
    if(c->conn_buf)
	free(c->conn_buf);
    if(c->conn_addr){
	free(c->conn_addr);
    }
    if(c->conn_outgoing){
	struct conn_q *tempq;

	for(tempq=c->conn_outgoing; tempq; tempq=tempq->next)
	    free(tempq);
    }
    free(c);
    return 0;
}

/*
 * Associate a callback routine for a certain purpose with a file descriptor.
 */

int conn_addfd(void (*fn)(), int fd, int type)
{
    conn_t *fdcon;
	
    if((fdcon=conn_alloc())==NULL){
	slerrno=(int)CE_MALLOC_FAIL;
	return -1;
    }
    fdcon->conn_sd=fd;
    fdcon->conn_handler=fn;
    switch(type){
    case CONN_FD_READ:
	FD_SET(fd, &readfds);
	fdcon->conn_read_status=CONN_USER_READFD;
	break;
    case CONN_FD_WRITE:
	FD_SET(fd, &writefds);
	fdcon->conn_write_status=CONN_USER_WRITEFD;
	break;
    case CONN_FD_EXCEPT:
	FD_SET(fd, &exceptfds);
	fdcon->conn_read_status=CONN_USER_EXCEPTFD;
	break;
    default:
	slerrno=(int)CE_STATE_UNKNOWN;
	return -1;
    }
    conn_addconnect(fdcon);
    return fdcon->conn_sd;
}

/*
 * An internal routine to register that we have used this connection.
 */

static conn_addconnect(conn_t *con)
{
    struct conn_list *newcl, *temp;
    int count;

    if((newcl=ALLOC_CONN_LIST())==NULL){
	slerrno=(int)CE_MALLOC_FAIL;
	return -1;
    }
    newcl->connection=con;
    newcl->next=NULL;
    if(conn_connections_list){
	struct conn_list *temp=conn_connections_list;

	count=0;
	while(temp->next) temp=temp->next;
	temp->next=newcl;
    }else{
	conn_connections_list=newcl;
    }
    return 0;
}

/*
 * Do a roundrobin select on all the current registered file descriptors and
 * connections, returning their status.
 */

static conn_t *conn_select(void)
{
    struct conn_list *curcon = conn_connections_list;
    fd_set rfds, wfds, efds;

    if(curcon==NULL)
	return NULL;
    bcopy(&readfds, &rfds, sizeof(fd_set));
    bcopy(&writefds, &wfds, sizeof(fd_set));
    bcopy(&exceptfds, &efds, sizeof(fd_set));
    select(FD_SETSIZE-1, &rfds, &wfds, &efds, 0);
    for(;;){
	if(FD_ISSET(curcon->connection->conn_sd, &rfds)){
	    curcon->connection->conn_pending=CONN_READ_PENDING;
	    return curcon->connection;
	}
	if(FD_ISSET(curcon->connection->conn_sd, &wfds)){
	    curcon->connection->conn_pending=CONN_WRITE_PENDING;
	    return curcon->connection;
	}
	if(FD_ISSET(curcon->connection->conn_sd, &efds)){
	    curcon->connection->conn_pending=CONN_EXCEPT_PENDING;
	    return curcon->connection;
	}
	if((curcon=curcon->next)==NULL)
	    curcon=conn_connections_list;
    }
}

/*
 * Allocate memory and resources for a connection.
 */

static conn_t *conn_alloc(void)
{
    conn_t *c;
	
    if((c=(conn_t *)malloc(sizeof(conn_t)))==NULL){
	return NULL;
    }
    c->conn_read_len=NULL;
    c->conn_mesg_len=NULL;
    c->conn_buf=NULL;
    c->conn_outgoing=NULL;
    c->conn_handler=NULL;
    c->conn_addr=NULL;
    c->conn_port=NULL;
    c->conn_sd=NULL;
    c->conn_id=NULL;
    return c;
}

/*
 * The event loop.
 */

void sl_event_loop(void)
{
    conn_t *n=NULL;
    char *buf;

    for(;;){
	n=conn_select();
	switch(n->conn_pending){
	case CONN_READ_PENDING:
	    switch(n->conn_read_status){
	    case CONN_SERVER:
		conn_accept(n);
		break;
	    case CONN_USER_READFD:
		n->conn_handler(n->conn_sd);
		break;
	    default:
		if(buf=conn_read(n))
		    if(conn_user_prefs.user_read)
			conn_user_prefs.user_read(n->conn_sd, buf,
						  n->conn_mesg_len);
		break;
	    }
	case CONN_WRITE_PENDING:
	    if(n->conn_write_status==CONN_USER_WRITEFD)
		if(n->conn_handler)
		    n->conn_handler(n->conn_sd);
	    conn_write(n->conn_sd, NULL, 0); 
	    break;
	case CONN_EXCEPT_PENDING:
	    if(n->conn_handler)
		n->conn_handler(n->conn_sd);
	    break;
	}
    }
}

char *conn_addr(int cc)
{
    conn_t *c;

    if((c=get_conn_ent(cc))==NULL){
	slerrno=(int)CE_UNKNOWN_SESSION;
	return (char *)-1;
    }
    return c->conn_addr;
}

static conn_t *conn_accept(conn_t *n)
{
    conn_t *new_n;
    int t;

    t=accept(n->conn_sd, 0, 0);
    if((new_n=conn_alloc())==0){
	close(t);
       	slerrno=(int)CE_MALLOC_FAIL;
	return (conn_t *)-1;
    }
    new_n->conn_sd=t;
    new_n->conn_read_status=CONN_CLIENT_OK;
    new_n->conn_write_status=CONN_CLIENT_OK;
    FD_SET(new_n->conn_sd, &readfds);
    conn_addconnect(new_n);
    return new_n;
}

static int conn_write_head(conn_t *c, u_long type, u_long info)
{
    u_long buf[2];

    buf[0]=htonl(type);
    buf[1]=htonl(info);
    if(write(c->conn_sd, buf, 2*sizeof(u_long))<0){
	slerrno=(int)CE_WRITE_FAIL;
	return -1;
    }
    return NULL;
}

static conn_t *conn_findconnect(long id)
{
    struct conn_list *curt=conn_connections_list;

    while(curt){
	if(curt->connection->conn_id==id)
	    return curt->connection;
	else
	    curt=curt->next;
    }
    return NULL;
}

static conn_t *get_conn_ent(int cc)
{
    struct conn_list *curt = conn_connections_list;

    while(curt){
	if(curt->connection->conn_sd==cc)
	    return curt->connection;
	else
	    curt=curt->next;
    }
    return NULL;
}

static int conn_removeconnect(conn_t *c)
{
    struct conn_list **prev, *cur;

    prev=&conn_connections_list;
    for(cur=*prev; cur; cur=cur->next){
	if(cur->connection==c){
	    *prev=cur->next;
	    free(cur);
	    return NULL;
	}else{
	    prev=&cur->next;
	}
    }
    slerrno=(int)CE_UNKNOWN_SESSION;
    return -1;
}

int slperror(char *s)
{
    if(slerrno==(int)CE_OK)
	return;
    fprintf(stderr, "%s: ", s);
    switch(slerrno){
    case CE_SOCKET_FAIL:
	fprintf(stderr, "socket failed\n");
	break;
    case CE_BIND_FAIL:
	fprintf(stderr, "bind failed\n");
	break;
    case CE_MALLOC_FAIL:
	fprintf(stderr, "malloc failed\n");
	break;
    case CE_GETHOSTBYNAME_FAIL:
	fprintf(stderr, "gethostbyname failed\n");
	break;
    case CE_CONNECT_FAIL:
	fprintf(stderr, "connect failed\n");
	break;
    case CE_UNKNOWN_SESSION:
	fprintf(stderr, "unknown session\n");
	break;
    case CE_READ_FAIL:
	fprintf(stderr, "read failed\n");
	break;
    case CE_WRITE_FAIL:
	fprintf(stderr, "write failed\n");
	break;
    case CE_MESSAGE_FAIL:
	fprintf(stderr, "message failed\n");
	break;
    case CE_STATE_UNKNOWN:
	fprintf(stderr, "unknown state\n");
	break;
    default:
	fprintf(stderr, "error %d\n", slerrno);
    }
}

