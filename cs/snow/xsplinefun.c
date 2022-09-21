/* xsplinefun.c - X11 version of spline fun #3
**
** Displays colorful moving splines in the X11 root window.
**
** Copyright (C) 1992 by Jef Poskanzer
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/

#include <stdio.h>
#include <errno.h>
#include <malloc.h>
#include <signal.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#if defined(SYSV) || defined(SVR4)
#include <string.h>
#define index strchr
#include <sys/termio.h>
#else /*SYSV*/
#include <strings.h>
#endif /*SYSV*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#define XTSTRINGDEFINES
#include <X11/StringDefs.h>


/* Definitions. */

#define X_CLASS "Xsplinefun"

#define DEFAULT_MAX_COLORS 256
#define DEFAULT_LOOPS_PER_SECOND 50

#define MIN_COLORS 4
#define INITIAL_LOOPS_PER_SLEEP 10
#define MIN_LOOPS_PER_SLEEP 1
#define INITIAL_SLEEP_USECS 100000
#define MIN_SLEEP_USECS 50000

#define POINTS 5
#define MAX_DELTA 3
#define MAX_COLOR_DELTA (3*256)

#define SPLINE_THRESH 5


/* Externals. */

extern char* getenv();
extern long random();


/* Forward routines. */

static void x_init();
static void x_alloc_colors();
static void x_cleanup();
static Window VirtualRootWindowOfScreen();
static void x_rdb_init();
static char* x_get_resource();
static int x_str_to_bool();
static void stealth();
static void main_loop();
static void sigcatch();
static void init_timing();
static void timing();
static void init_splines();
static void rotate_colormap();
static void new_color();
static void move_splines();
static void XDrawSpline();


/* Routines. */

static char* argv0;
static char* app_name;
static int forwards, backwards;
static int max_colors;
static int loops_per_second;

static int loops_per_sleep;
static int sleep_usecs;

void
main( argc, argv )
    int argc;
    char* argv[];
    {
    char* rval;
    char* usage = "usage: %s [-display d] [-forwards|-backwards] [-maxcolors n] [-loops n] [-id]\n";

    argv0 = argv[0];
    app_name = "xsplinefun";
    forwards = backwards = False;
    max_colors = DEFAULT_MAX_COLORS;
    loops_per_second = DEFAULT_LOOPS_PER_SECOND;
    loops_per_sleep = INITIAL_LOOPS_PER_SLEEP;
    sleep_usecs = INITIAL_SLEEP_USECS;

    /* Parse args and initialize X stuff. */
    x_init( &argc, argv );

    /* Check usage. */
    if ( argc != 1 )
	{
	(void) fprintf( stderr, usage, argv[0] );
	exit( 1 );
	}

    /* Check rotation. */
    rval = x_get_resource( "rotation", "Rotation" );
    if ( rval != (char*) 0 )
	if ( rval[0] == 'f' || rval[0] == 'F' )
	    forwards = True;
	else if ( rval[0] == 'b' || rval[0] == 'B' )
	    backwards = True;

    /* Check max colors. */
    rval = x_get_resource( "maxcolors", "Maxcolors" );
    if ( rval != (char*) 0 )
	max_colors = atoi(rval);
    x_alloc_colors();

    /* Check loops. */
    rval = x_get_resource( "loops", "Loops" );
    if ( rval != (char*) 0 )
	loops_per_second = atoi(rval);

    /* Initialize the random number generator. */
    srandom( (int) ( time( (long*) 0 ) ^ getpid() ) );

    /* Initialize the splines. */
    init_splines();

    /* Fork, if necessary. */
    rval = x_get_resource( "id", "Id" );
    if ( rval != (char*) 0 )
	if ( x_str_to_bool( rval ) )
	    stealth();

    /* Main loop. */
    main_loop();
    
    /* Done. */
    x_cleanup();
    exit( 0 );
    }


/* X stuff. */

static Display* display = (Display*) 0;
static int screennum;
static Screen* screen;
static Window root;
static int width, height;
static int depth;
static GC gc;
static Colormap cmap;
static int ncolors;
static unsigned long pixels[DEFAULT_MAX_COLORS];

static void
x_init( argcP, argv )
    int* argcP;
    char** argv;
    {
    char* display_name;
    char* rval;
    int i, d;

    /* Scan args looking for -display. */
    display_name = (char*) 0;
    for ( i = 1; i + 1 < *argcP; ++i )
	{
	if ( strcmp( argv[i], "-display" ) == 0 ||
	     strcmp( argv[i], "-displa" ) == 0 ||
	     strcmp( argv[i], "-displ" ) == 0 ||
	     strcmp( argv[i], "-disp" ) == 0 ||
	     strcmp( argv[i], "-dis" ) == 0 ||
	     strcmp( argv[i], "-di" ) == 0 ||
	     strcmp( argv[i], "-d" ) == 0 )
	    {
	    display_name = argv[i + 1];
	    for ( i = i + 2; i <= *argcP; ++i )
		argv[i - 2] = argv[i];
	    *argcP -= 2;
	    break;
	    }
	}

    display = XOpenDisplay( display_name );
    if ( display == (Display*) 0 )
	{
	(void) fprintf(
	    stderr, "%s: can't open display \"%s\"\n", argv0,
	    XDisplayName( display_name ) );
	exit( 1 );
	}

    screennum = DefaultScreen( display );
    screen = ScreenOfDisplay( display, DefaultScreen( display ) );
    root = VirtualRootWindowOfScreen( screen );
    width = WidthOfScreen( screen );
    height = HeightOfScreen( screen );
    depth = DefaultDepthOfScreen( screen );
    if ( depth <= 1 )
	{
	(void) fprintf(
	    stderr, "%s: screen depth must be greater than 1\n", argv0 );
	exit( 1 );
	}
    gc = XCreateGC( display, root, 0, (XGCValues*) 0 );
    cmap = DefaultColormapOfScreen( screen );

    x_rdb_init( argcP, argv );

    rval = x_get_resource( XtNname, "Name" );
    if ( rval != (char*) 0 )
	app_name = rval;

    rval = x_get_resource( "synchronous", "Synchronous" );
    if ( rval != (char*) 0 )
	if ( x_str_to_bool( rval ) )
	    XSynchronize( display, True );
    }

static void
x_alloc_colors()
    {
    for ( ncolors = max_colors; ncolors >= MIN_COLORS; --ncolors )
	if ( XAllocColorCells(
		display, cmap, False, (unsigned long*) 0, 0, pixels, ncolors ) )
	    break;
    if ( ncolors < MIN_COLORS )
	{
	(void) fprintf( stderr, "%s: can't allocate enough colors\n", argv0 );
	exit( 1 );
	}
    if ( ncolors < max_colors )
	(void) fprintf(
	    stderr, "%s: only %d colors available\n", argv0, ncolors );
    }

static void
x_cleanup()
    {
    XClearArea( display, root, 0, 0, width, height, True );
    XFreeColors( display, cmap, pixels, ncolors, (unsigned long) 0 );
    XFreeGC( display, gc );
    XCloseDisplay( display );
    }


/* From vroot.h by Andreas Stolcke. */

static Window
VirtualRootWindowOfScreen( screenP )
    Screen* screenP;
    {
    static Window root = (Window) 0;
    Display* dpy = DisplayOfScreen( screenP );
    Atom __SWM_VROOT = None;
    int i;
    Window rootReturn, parentReturn;
    Window* children;
    unsigned int numChildren;

    root = RootWindowOfScreen( screenP );

    /* Go look for a virtual root. */
    __SWM_VROOT = XInternAtom( dpy, "__SWM_VROOT", False );
    if ( XQueryTree(
	     dpy, root, &rootReturn, &parentReturn, &children,
	     &numChildren ) )
	{
	for ( i = 0; i < numChildren; ++i )
	    {
	    Atom actual_type;
	    int actual_format;
	    unsigned long nitems, bytesafter;
	    Window* newRoot = (Window*) 0;

	    if ( XGetWindowProperty(
		     dpy, children[i], __SWM_VROOT, 0, 1, False, XA_WINDOW,
		     &actual_type, &actual_format, &nitems, &bytesafter,
		     (unsigned char**) &newRoot ) == Success && newRoot )
		{
		root = *newRoot;
		break;
		}
	    }
	if ( children )
	    XFree( (char*) children );
	}

    return root;
    }


/* X resources stuff. */

static XrmDatabase rdb;

static XrmOptionDescRec x_options[] = {
    { "-forwards",       "*rotation",       XrmoptionNoArg,  (caddr_t) "f" },
    { "-backwards",      "*rotation",       XrmoptionNoArg,  (caddr_t) "b" },
    { "-maxcolors",      "*maxcolors",      XrmoptionSepArg, (caddr_t) 0 },
    { "-loops",          "*loops",          XrmoptionSepArg, (caddr_t) 0 },
    { "-id",             "*id",             XrmoptionNoArg,  (caddr_t) "on" },
    { "-name",           ".name",           XrmoptionSepArg, (caddr_t) 0 },
    { "-synchronous",    "*synchronous",    XrmoptionNoArg,  (caddr_t) "on" },
    { "-xrm",            (char*) 0,         XrmoptionResArg, (caddr_t) 0 },
    };

static void
x_rdb_init( argcP, argv )
    int* argcP;
    char** argv;
    {
    char* resource_string;
    char* xenv;
    XrmDatabase xenv_rdb;

    XrmInitialize();

    /* Look for resource databases on server. */
    resource_string = XResourceManagerString( display );
    if ( resource_string != (char*) 0 )
	rdb = XrmGetStringDatabase( resource_string );
    else
	{
	/* No server databases, try ~/.Xdefaults */
	char* cp;
	char buf[500];

	cp = getenv( "HOME" );
	if ( cp != (char*) 0 )
	    (void) strcpy( buf, cp );
	else
	    {
	    struct passwd* pw;

	    cp = getenv( "USER" );
	    if ( cp != (char*) 0 )
		pw = getpwnam( cp );
	    else
		pw = getpwuid( getuid() );
	    if ( pw != (struct passwd*) 0 )
		(void) strcpy( buf, pw->pw_dir );
	    else
		(void) strcpy( buf, "." );	/* best we can do */
	    }
	(void) strcat( buf, "/.Xdefaults" );
	rdb = XrmGetFileDatabase( buf );
	}

    /* Merge in XENVIRONMENT, if any. */
    xenv = getenv( "XENVIRONMENT" );
    if ( xenv != (char*) 0 )
	{
	xenv_rdb = XrmGetFileDatabase( xenv );
	XrmMergeDatabases( xenv_rdb, &rdb );
	}

    /* And add command line options. */
    XrmParseCommand(
	&rdb, x_options, sizeof(x_options) / sizeof(*x_options),
	app_name, argcP, argv );
    }

static char*
x_get_resource( name, class )
    char* name;
    char* class;
    {
    char rname[500], rclass[500];
    char* type;
    XrmValue value;

    (void) sprintf( rname, "%s.%s", app_name, name );
    (void) sprintf( rclass, "%s.%s", X_CLASS, class );
    if ( XrmGetResource( rdb, rname, rclass, &type, &value ) == True )
	if ( strcmp( type, XtRString ) == 0 )
	    return (char*) value.addr;
    return (char*) 0;
    }

static int
x_str_to_bool( str )
    char* str;
    {
    if ( strcmp( str, "True" ) == 0 ||
         strcmp( str, "true" ) == 0 ||
         strcmp( str, "Yes" ) == 0 ||
         strcmp( str, "yes" ) == 0 ||
         strcmp( str, "On" ) == 0 ||
         strcmp( str, "on" ) == 0 ||
         strcmp( str, "Si" ) == 0 ||
         strcmp( str, "si" ) == 0 ||
         strcmp( str, "Da" ) == 0 ||
         strcmp( str, "da" ) == 0 ||
         strcmp( str, "T" ) == 0 ||
         strcmp( str, "t" ) == 0 ||
         strcmp( str, "Y" ) == 0 ||
         strcmp( str, "y" ) == 0 ||
         strcmp( str, "1" ) == 0 )
	return True;
    return False;
    }


/* Generic application stuff. */

static void
stealth()
    {
    int pid, tty;

    pid = fork();
    if ( pid < 0 )
	{
	perror( "fork" );
	exit( 1 );
	}
    else if ( pid > 0 )
	/* Parent just exits. */
	exit( 0 );
    (void) printf( "%d\n", getpid() );
    (void) fflush( stdout );

    /* Go stealth (ditch our controlling tty). */
    tty = open( "/dev/tty", 0 );
    if ( tty < 0 )
	{
	/* ENXIO means that there is no controlling terminal, so we don't
	** have to detach anything.
	*/
        if ( errno != ENXIO )
	    {
	    (void) fprintf( stderr, "%s: ", argv0 );
	    perror( "/dev/tty open" );
	    exit( 1 );
	    }
	}
    else
	{
#ifdef TIOCNOTTY
	if ( ioctl( tty, TIOCNOTTY, 0 ) < 0 )
	    {
	    (void) fprintf( stderr, "%s: ", argv0 );
	    perror( "TIOCNOTTY ioctl" );
	    exit( 1 );
	    }
#endif /*TIOCNOTTY*/
	(void) close( tty );
	}
    }

static int goflag;
static int loops;

static void
sigcatch()
    {
    goflag = 0;
    }

static int then_loops;
static time_t then;

static void
init_timing()
    {
    then = time( (time_t*) 0 );
    then_loops = 0;
    }

static void
timing()
    {
    float ratio;
    int t;
    time_t now;

    if ( sleep_usecs > 0 && loops_per_sleep > 0 &&
	 loops % loops_per_sleep == 0 )
	{
	XFlush( display );
	usleep( sleep_usecs );
	if ( loops_per_second > 0 )
	    {
	    now = time( (time_t*) 0 );
	    if ( now != then )
		{
		if ( then_loops != 0 )
		    {
		    ratio = (float) ( loops - then_loops ) /
			    (float) loops_per_second;
		    if ( ratio > 1.0 )
			{
			/* We're going too fast. */
			t = loops_per_sleep / ratio;
			if ( t >= MIN_LOOPS_PER_SLEEP )
			    loops_per_sleep = t;
			else
			    sleep_usecs *= ratio;
			}
		    else if ( ratio < 1.0 )
			{
			/* We're going too slow. */
			t = sleep_usecs * ratio;
			if ( t >= MIN_SLEEP_USECS )
			    sleep_usecs = t;
			else
			    {
			    loops_per_sleep /= ratio;
			    if ( loops_per_sleep > 3 * loops_per_second )
				loops_per_sleep = 0;	/* give up */
			    }
			}
		    }
		then_loops = loops;
		then = now;
		}
	    }
	}
    }

static void
main_loop()
    {
    /* Set up for signal catching. */
    goflag = 1;
    (void) signal( SIGHUP, sigcatch );
    (void) signal( SIGINT, sigcatch );
    (void) signal( SIGTERM, sigcatch );

    /* Do it. */
    for ( loops = 1; goflag; ++loops )
	{
	move_splines();
	timing();
	}
    }


/* Spline-fun smarts. */

static int x[POINTS], y[POINTS], dx[POINTS], dy[POINTS];
static int nred, ngreen, nblue, dred, dgreen, dblue;
static int color;
static XColor xcolors[DEFAULT_MAX_COLORS];

static void
init_splines()
    {
    int i;

    /* Initialize points. */
    for ( i = 0; i < POINTS; ++i )
	{
	x[i] = random() % width;
	y[i] = random() % height;
	dx[i] = random() % ( MAX_DELTA * 2 ) - MAX_DELTA;
	if ( dx[i] <= 0 ) --dx[i];
	dy[i] = random() % ( MAX_DELTA * 2 ) - MAX_DELTA;
	if ( dy[i] <= 0 ) --dy[i];
	}

    /* Initalize colors. */
    for ( color = 0; color < ncolors; ++color )
	{
	xcolors[color].red = xcolors[color].green = xcolors[color].blue = 0;
	xcolors[color].pixel = pixels[color];
	xcolors[color].flags = DoRed|DoGreen|DoBlue;
	}
    color = 0;
    nred = ngreen = nblue = 0;
    dred = random() % ( MAX_COLOR_DELTA * 2 ) - MAX_COLOR_DELTA;
    if ( dred <= 0 ) --dred;
    dgreen = random() % ( MAX_COLOR_DELTA * 2 ) - MAX_COLOR_DELTA;
    if ( dgreen <= 0 ) --dgreen;
    dblue = random() % ( MAX_COLOR_DELTA * 2 ) - MAX_COLOR_DELTA;
    if ( dblue <= 0 ) --dblue;
    }

static void
rotate_colormap()
    {
    int t, i;

    if ( forwards )
	{
	t = xcolors[0].pixel;
	for ( i = 0; i < ncolors - 1; ++i )
	    xcolors[i].pixel = xcolors[i + 1].pixel;
	xcolors[ncolors - 1].pixel = t;
	XStoreColors(display, cmap, xcolors, ncolors );
	}
    else if ( backwards )
	{
	t = xcolors[ncolors - 1].pixel;
	for ( i = ncolors - 1; i > 0; --i )
	    xcolors[i].pixel = xcolors[i - 1].pixel;
	xcolors[0].pixel = t;
	XStoreColors(display, cmap, xcolors, ncolors );
	}
    }

static void
new_color()
    {
    int t;

    for ( ; ; )
	{
	t = (int) nred + dred;
	if ( t >= 0 && t < 65536 ) break;
	dred = random() % ( MAX_COLOR_DELTA * 2 ) - MAX_COLOR_DELTA;
	if ( dred <= 0 ) --dred;
	}
    xcolors[color].red = nred = t;
    for ( ; ; )
	{
	t = (int) ngreen + dgreen;
	if ( t >= 0 && t < 65536 ) break;
	dgreen = random() % ( MAX_COLOR_DELTA * 2 ) - MAX_COLOR_DELTA;
	if ( dgreen <= 0 ) --dgreen;
	}
    xcolors[color].green = ngreen = t;
    for ( ; ; )
	{
	t = (int) nblue + dblue;
	if ( t >= 0 && t < 65536 ) break;
	dblue = random() % ( MAX_COLOR_DELTA * 2 ) - MAX_COLOR_DELTA;
	if ( dblue <= 0 ) --dblue;
	}
    xcolors[color].blue = nblue = t;
    XStoreColor(display, cmap, &(xcolors[color]) );
    XSetForeground( display, gc, xcolors[color].pixel );
    if ( ++color >= ncolors ) color -= ncolors;
    }

static void
move_splines()
    {
    int i, t, px, py, zx, zy, nx, ny;

    /* Rotate colormap if necessary. */
    rotate_colormap();

    /* Choose new color. */
    new_color();

    /* Backwards rotation requires two new colors each loop. */
    if ( backwards )
	new_color();

    /* Move the points. */
    for ( i = 0; i < POINTS; i++ )
	{
	for ( ; ; )
	    {
	    t = x[i] + dx[i];
	    if ( t >= 0 && t < width ) break;
	    dx[i] = random() % ( MAX_DELTA * 2 ) - MAX_DELTA;
	    if ( dx[i] <= 0 ) --dx[i];
	    }
	x[i] = t;
	for ( ; ; )
	    {
	    t = y[i] + dy[i];
	    if ( t >= 0 && t < height ) break;
	    dy[i] = random() % ( MAX_DELTA * 2 ) - MAX_DELTA;
	    if ( dy[i] <= 0 ) --dy[i];
	    }
	y[i] = t;
	}

    /* Draw the figure. */
    px = zx = ( x[0] + x[POINTS-1] ) / 2;
    py = zy = ( y[0] + y[POINTS-1] ) / 2;
    for ( i = 0; i < POINTS-1; ++i )
	{
	nx = ( x[i+1] + x[i] ) / 2;
	ny = ( y[i+1] + y[i] ) / 2;
	XDrawSpline( display, root, gc, px, py, x[i], y[i], nx, ny );
	px = nx;
	py = ny;
	}
    XDrawSpline(
	display, root, gc, px, py, x[POINTS-1], y[POINTS-1], zx, zy );
    }


/* X spline routine. */

#define abs(x) ((x) < 0 ? -(x) : (x))

static void
XDrawSpline( display, d, gc, x0, y0, x1, y1, x2, y2 )
Display* display;
Drawable d;
GC gc;
int x0, y0, x1, y1, x2, y2;
    {
    register int xa, ya, xb, yb, xc, yc, xp, yp;

    xa = ( x0 + x1 ) / 2;
    ya = ( y0 + y1 ) / 2;
    xc = ( x1 + x2 ) / 2;
    yc = ( y1 + y2 ) / 2;
    xb = ( xa + xc ) / 2;
    yb = ( ya + yc ) / 2;

    xp = ( x0 + xb ) / 2;
    yp = ( y0 + yb ) / 2;
    if ( abs( xa - xp ) + abs( ya - yp ) > SPLINE_THRESH )
	XDrawSpline( display, d, gc, x0, y0, xa, ya, xb, yb );
    else
	XDrawLine( display, d, gc, x0, y0, xb, yb );

    xp = ( x2 + xb ) / 2;
    yp = ( y2 + yb ) / 2;
    if ( abs( xc - xp ) + abs( yc - yp ) > SPLINE_THRESH )
	XDrawSpline( display, d, gc, xb, yb, xc, yc, x2, y2 );
    else
	XDrawLine( display, d, gc, xb, yb, x2, y2 );
    }


#ifdef SYSV

/* Most SysV's don't have a usleep.  Sone of them have select. */

usleep( usecs )
int usecs;
    {
    struct timeval timeout;

    timeout.tv_sec = usecs / 1000000;
    timeout.tv_usec = usecs % 1000000;
    select( 0, 0, 0, 0, &timeout );
    }

#endif /*SYSV*/
