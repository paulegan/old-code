/*
 * TCP/IP Session Layer Internal Header File.
 *
 * (c) 1992 SCRG, Trinity College, Dublin.
 */

/*
 * This structure holds the data that has NOT been sent so far.
 */

struct conn_q{
    char *buf;
    int pos;
    int len;
    struct conn_q *next;
};

/*
 * The connection structure.
 */

typedef struct _conn_t{
    int conn_sd;               /* The socket descriptor */
    int conn_read_status;      /* The status of a socket */
    int conn_write_status;     /* The status of a socket */
    int conn_pending;          /* The pending status of a socket */
    int conn_read_len;         /* The amount of data read so far */
    int conn_mesg_len;         /* The amount of data to be read */
    char *conn_buf;            /* The buffer of data read so far */
    struct conn_q *conn_outgoing; /* The list of unsent data */
    void (*conn_handler)();    /* Handler for user file descriptors */
    char *conn_addr;           /* Where the connection is to/from */
    long conn_id;              /* The ID of the socket */
    unsigned short conn_port;  /* The port number of the connection */
}conn_t;

/*
 * This macro allocates some space for a new connection
 */

#define ALLOC_CONN_Q() ((struct conn_q *)malloc(sizeof(struct conn_q)))

/*
 * The status of a connection.
 */

#define CONN_SERVER 0		/* Listening for connections */
#define CONN_CLIENT_OK 1	/* Communicating OK */
#define CONN_CLIENT_BROKEN 2	/* Not communicating OK (start queueing) */
#define CONN_CLIENT_READING 3	/* Still reading data structure */
#define CONN_CLIENT_WRITING 4	/* Queueing up outgoing data */

/*
 * The status values below mean that the connection is really a file
 * descriptor that the user would like to have the event loop check also.
 * This would usually be used to poll the keyboard.
 */

#define CONN_USER_READFD 1000	/* A read on a file descriptor */
#define CONN_USER_WRITEFD 1001	/* A write on a file descriptor */
#define CONN_USER_EXCEPTFD 1002	/* An exception on a file descriptor */

/*
 * The pending status of a connection (or file descriptor).
 */

#define CONN_READ_PENDING 0
#define CONN_WRITE_PENDING 1
#define CONN_EXCEPT_PENDING 2

/*
 * The user-supplied callback routines and timeout for a connection.
 * Timeout is how long a connection will wait around after a connection
 * is broken for a reconnect before it's structure is deallocated.
 */

struct _conn_user_prefs{
    void (*user_read)();
    void (*user_accept)();
    void (*user_abort)();
    void (*user_close)();
};

extern struct _conn_user_prefs conn_user_prefs;

/*
 * Allocate memory for a connection.
 */

static conn_t *conn_alloc(void);

/*
 * Read data from the connection.
 */

static char *conn_read(conn_t *);

/*
 * The list of connections.
 */

struct conn_list{
    conn_t *connection;
    struct conn_list *next;
};

/*
 * Allocate a connection.
 */

#define ALLOC_CONN_LIST() (struct conn_list *)malloc(sizeof(struct conn_list))

/*
 * Do a select to see which connections can have something done on them.
 */

static conn_t *conn_select(void);

/*
 * Add a connection to the list of connections to check...
 */

static int conn_addconnect(conn_t *);

/*
 * Close a connection
 */

static int conn_iclose(conn_t *);

/*
 * Queue data for transmission on a connection later.
 */

static int conn_queue(conn_t *, char *, int, int);

/*
 * Accept a connection.
 */

static conn_t *conn_accept(conn_t *);

/*
 * Find the connection with the specified ID.
 */

static conn_t *conn_findconnect(long);

/*
 * Remove a connection.
 */

static int conn_removeconnect(conn_t *);

/*
 * Get a connection entry.
 */

static conn_t *get_conn_ent(int);

/*
 * Write a header down the connection.
 */

static int conn_write_head(conn_t *, u_long, u_long);

/*
 * Defines for the protocols.
 */

#define CONN_MESG_CONNECT (u_long)1234 /* Request a connection */
#define CONN_MESG_RECONNECT (u_long)2345 /* Request a reconnection */
#define CONN_MESG_DATA (u_long)3456 /* Sending some data */
#define CONN_MESG_SHUTDOWN (u_long)4567 /* Gracefully shutting down the
					   connection */
#define CONN_MESG_ACK (u_long)5678 /* Acknowledge connection and
				      send session id */

/*
 * Misc useful things.
 */

#define strdup(c) (char *)(strcpy((char *)malloc(strlen(c)), (c)))

