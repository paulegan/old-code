/*
 * $Id$
 *
 * Secure modes is a program that updates files & modes to a more
 * secure (sane) permission/ownership set.
 *
 * secure-modes currently has 4 types of modes that are changed:
 *
 *	m	remove group/other write permissions of all files,
 *		except those in the exceptions list.
 *	w	remove user write permission for executables that
 *		are not root owned.
 *	s	remove g/o read permission for set-uid/set-gid executables
 *	o	change the owner of files/directories that can be safely
 *		chowned to root.
 *
 *	Any combination of changes can be switched off by specifying -X
 *
 *	-D	limit changes to directories
 *	-u	undo changes
 *	-r	rollback (from aborted secure-modes)
 *	-d	debug mode
 *	-v	verbose mode
 *	-f	force changes, even if the contents file and the file's
 *		current mode don't match.  This can't be undone.
 *		Undo will restore the file to /var/sadm/install/contents modes
 *		The force option will *not* update information about
 *		files that do not exist, nor will it attempt to change
 *		files on other than ufs/nfs filesystems.
 *	-q	quiet mode, more -q, less output
 *	-S	Limit changes to Sun packages.
 *	-a	Modify all packages.
 *	-c	force consistency between .mods and the supplied options,
 *		(i.e., when you specify "-D" and "-c" , the program will
 *		"unfix" all non-directories)
 *
 *	-R dir	Run as if chrooted to <dir>.
 *
 * WARNING: there are no checks on chown/chmod failures so beware!
 *
 * The best way to operate this program is like this:
 *
 *		# pkgchk -na >& pkgchk.out-before
 *		# secure-modes
 *		# pkgchk -na >& pkgchk.out-after
 *
 * and the compare the output of both pkgchk, they should be identical
 * except that the automounter may cause some mount points to changes
 *
 * output:
 *
 * d m mode -> newmode pathname
 * | ^ whether the file/dir is group writable or even world writable
 * > type of file.
 * d o owner -> newowner pathname [mode]
 *
 * TODO:
 *	if the contents file and the filesystem don't match for
 *	a specific file, don't update it and print a warning.
 *
 * Casper Dik (Casper.Dik@Holland.Sun.COM)
 *
 * NOTE: I took some liberty with types as some types won't be expanded
 * to 64 bits and will become ints, rather than longs.
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <stddef.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <libintl.h>
#include <locale.h>

static char * exceptions [] = {
#include "exceptions.h"
};

#define DEFAULT_SU 1
#define DEFAULT_OWNER 1
#define DEFAULT_MODES 1
#define DEFAULT_USERWRITE 1
#define DEFAULT_DIRSONLY 0
#define DEFAULT_ALLPKGS 1

#define MAXCHANGES	100

static int nexceptions = sizeof(exceptions)/sizeof(char*);
static int dosu = DEFAULT_SU;
static int doowner = DEFAULT_OWNER;
static int domodes = DEFAULT_MODES;
static int douserwrite = DEFAULT_USERWRITE;
static int dirsonly = DEFAULT_DIRSONLY;
static int debug = 0;
static int verbose = 0;
static int quiet = 0;
static int consistent;

static char *contents;		/* "/var/sadm/install/contents" */
static char *newcontents;	/* ".... .new" */
static char *modsfile;		/* ".... .mods" */
static char *newmodsfile;	/* ".... .mods.new */
static char *difffile;		/* ".... .diff */

#define LINE_OK		0
#define LINE_IGNORE	1
#define LINE_ERROR	2

typedef struct cntsline {
    char path[MAXPATHLEN];	/* NUL terminated pathname */
    char *start;		/* buffer start */
    char *rest;			/* buffer after group */
    char *owner;		/* user name */
    char *group;		/* group name */
    int newuid, uid;		/* new and old uid */
    int newgid, gid;		/* new and old gid */
    unsigned int newmode, mode;	/* new and old mode */
    char type;			/* the type of this entry */
    char realtype;		/* the equivalent type of this entry */
} cntsline_t;

static cntsline_t changes[MAXCHANGES];

static int parse_line(char *, cntsline_t *, int);
static void put_line(FILE *cf, cntsline_t *);
static int secure_file(cntsline_t *);
static void setnewuid(cntsline_t *, int);
static void setnewgid(cntsline_t *, int);
static void revert(cntsline_t *, cntsline_t *);

static int check_mode(cntsline_t *, int);
static void report_diffs(cntsline_t *);

static uid_t str2uid(const char *);
static char * uid2str(uid_t);
static gid_t str2gid(const char *);
static char * gid2str(gid_t);

static int loadmods(const char *);
static void dorollback(const char *, int);
static void commit_changes(FILE *);

static FILE *fopenfile(const char*, const char *);
static char *strdupcat(const char *, const char *);

static int undo;
static int allpkgs = DEFAULT_ALLPKGS;
static int nchanges;
static char *program;
static char *rootdir;

static const char *crp(const char *);

static cntsline_t *modentry(cntsline_t *);
static void modlineprint(FILE *, cntsline_t *);

/*
** The following 2 functions were added to handle locking the package database.
** Note: this is a private interface and is subject to change!
**
** XXX: although we lock the database, we don't attempt to write a conforming
** entry into the lockfile, since the fields don't make a lot of sense for us.
*/
static int lockfd = -1;

static void unlockdb(void)
{
    if ( lockfd != -1 ) {
	(void) lockf(lockfd, F_ULOCK, 0);
	(void) close(lockfd);
	lockfd = -1;
    }
}

static void lockdb(void)
{
    /*
     * No lock is placed when debug is set (i.e., -d or alternate cnts)
     */
    if ( (lockfd = open(crp("/var/sadm/install/.lockfile"),
		    O_WRONLY | O_CREAT, 0644)) == -1 ) {
	perror("/var/sadm/install/.lockfile");
	exit(1);
    }

    if ( lockf(lockfd, F_TLOCK, 0) == -1 ) {
	fprintf(stderr,
		gettext("Unable to obtain lock on package database: %s\n"),
		strerror(errno));
	exit(1);
    }

    atexit(unlockdb);
}

static void usage(void) {
    fprintf(stderr,
        gettext("Usage: %s [-owsmDvdrufqaSc] [-R dir] [contentsfile]\n"),
	    program);
    exit(1);
}

int
main(int argc, char **argv)
{
    char buf[8192];
    int c;
    extern int optind, opterr;
    int rollback = 0;
    int undoerrs = 0;
    int lineno = 0;
    cntsline_t line;
    cntsline_t *oldmod;
    int hasmods;
    int force = 0;

    FILE *cf, *mf, *ncf, *df;

    char* p;
    if ( (p = getenv("TEXTDOMAIN")) ) {
	    char domain[64];

	    memset(domain, '\0', sizeof(domain));
	    strncpy(domain, p, sizeof(domain) - 1);
	    textdomain(domain);

	    if ( (p = getenv("TEXTDOMAINDIR")) )
		    bindtextdomain(domain, p);
    }
    setlocale(LC_ALL, "");

    opterr = 0;

    program = strrchr(argv[0],'/');
    if (program)
	program++;
    else
	program = argv[0];

    while ((c = getopt(argc, argv,"asomwSDvdrufR:qc")) != EOF) {
	switch (c) {
	case 's': dosu = !DEFAULT_SU; break;
	case 'o': doowner = !DEFAULT_OWNER; break;
	case 'm': domodes = !DEFAULT_MODES; break;
	case 'w': douserwrite = !DEFAULT_USERWRITE; break;
	case 'D': dirsonly = !DEFAULT_DIRSONLY; break;

	case 'v': verbose = 1; break;
	case 'd': debug = 1; break;
	case 'r': rollback = 1; break;
	case 'u': undo = 1; break;
	case 'f': force = 1; break;
	case 'R': rootdir = optarg; break;
	case 'q': quiet++; break;
	case 'a': allpkgs = 1; break;
	case 'S': allpkgs = 0; break;
	case 'c': consistent = 1; break;
	default: /* FALLTHROUGH */
	case '?': usage(); break;
	}
    }
    argc -= optind;
    argv += optind;

    if (argc > 1)
	usage();

    /* To make sure we create the database more 0644 */
    (void) umask(022);

    if (!consistent)
	consistent = dosu && doowner && domodes && douserwrite && !dirsonly;

    /*
     * Implementation note:
     * Chroot() is implemented through chdir(<newroot>) followed by
     * exclusive use of relative pathnames, using the crp() function
     * (ChRootedPath)
     */
    if (rootdir && chdir(rootdir) != 0) {
	perror(rootdir);
	exit(1);
    }

    if (argv[0] == NULL) {
	contents = "/var/sadm/install/contents";
    } else {
	contents = argv[0];
	debug = 1;
    }

    newcontents = strdupcat(contents,".new");
    difffile = strdupcat(contents,".diff");
    modsfile = strdupcat(contents,".mods");
    newmodsfile = strdupcat(contents,".mods.new");

    /* set lock file - preferably one that locks out pkg* as well */
    if (!debug)
	lockdb();

    /*
     * if a diffs file exist:
     * - remove mods.new, contents.new, rollback diff, remove diff
     */

    if (access(crp(difffile), F_OK) == 0) {
	dorollback(crp(difffile), rollback);
	/*
	 * Keep the files only if rollback and debug are both true,
	 * when debug we can safely remove empty rollback files
	 * when !debug we have rolled back so we can remove them
	 */
	if (!rollback || !debug) {
	    if (unlink(crp(difffile)))
		perror("unlink diffile");
	    (void) unlink(crp(newmodsfile));
	    (void) unlink(crp(newcontents));
	}
	if (rollback)
	    exit(0);
    } else if (rollback) {
	fprintf(stderr, gettext("%s: No rollback file\n"), program);
	exit(1);
    }

    /*
     *  if *.new files exist:
     *    bail out and let user fix.
     * endif
     */
    if (access(crp(newcontents), F_OK) == 0 ||
	    access(crp(newmodsfile), F_OK) == 0) {
	fprintf(stderr,
                gettext("%s: New contents or mods file exists, you must recover by hand\n"),
		program);
	exit(1);
    }

    /* processing contents into:
		- new mods file
		- diff file
		- new contents file
		- verbose output
		- filesystem chmod/chowns [ save some and run them after
		  flushing diff ]
     */

    /* loading old "mods" file */
    hasmods = loadmods(crp(modsfile)) != 0;
    if (!hasmods && undo) {
	fprintf(stderr, gettext("%s: nothing to undo\n"), program);
	exit(1);
    }

    cf = fopenfile(crp(contents),"r");
    ncf = fopenfile(crp(newcontents),"w");
    df = fopenfile(crp(difffile),"w");
    if (hasmods || debug)
	mf = fopenfile(crp(newmodsfile),"w");
    else {
	/* no previous mods ? ===> newmods == diff, !debugging which adds # */
	mf = 0;
	if (link(crp(difffile), crp(newmodsfile)) != 0) {
	    perror("link(diff, newmod)");
	    exit(1);
	}
    }

    for (; fgets(buf, sizeof(buf), cf);
		put_line(ncf, &line),modlineprint(mf, oldmod)) {

	int error = 0;

	oldmod = 0;

	switch (parse_line(buf, &line, ++lineno)) {
	case LINE_IGNORE: continue;
	case LINE_ERROR: error = 1; break;
	}

	/* find old mods in list - either copied or modified further */
	oldmod = modentry(&line);

	/* 
	 * In case of error, we only want to call modentry so the old
	 * entry is preserved in the .mods file
	 */
	if (error)
	    continue;

	if (undo) {

	    if (!oldmod)
		continue;

	    if (check_mode(&line, force) == 0) {
		/* Reset the old mode, owner & group */
		revert(&line, oldmod);

		/* suppress modline output, new modsfile should be empty */
		oldmod = 0;
	    } else {
		undoerrs++;
		continue;
	    }
	} else {
	    int ok = secure_file(&line);

	    if (ok && !oldmod)
		continue;

	    /* If the mode isn't what we expect, and we care ignore the file */
	    if ((!ok && check_mode(&line, force) != 0)) {
		revert(&line, 0);
		continue;
	    }

	    if (oldmod) {
		/*
		 * Merge the old modifications and the new modifications.
		 * If the file permissions remembered in the old .mods
		 * file do no match, we ignore them.  This typically happens
		 * after system upgrades or after patch installation.
		 *
		 * There's one possibility of failure: if, after an upgrade,
		 * the modes have become as this program wants them, the
		 * old modes will be preserved in the .mods file and undo
		 * will get you those modes back.
		 */
		if (oldmod->newuid != line.uid || oldmod->newgid != line.gid ||
		    oldmod->newmode != line.mode) {

		    oldmod = ok ? NULL : &line;
		    if (quiet < 1)
			fprintf(stderr,
				gettext("%s: WARNING: ignoring old info for %s\n"),
				program, line.path);
		} else {
		    /*
		     * We compute the files new permissions using the
		     * original ones as the baseline.  This allows us
		     * to set the file to a less secure mode here,
		     * e.g., if we call secure-modes with different
		     * options, if we needed to fix exceptions.h or
		     * if we fixed other bugs.
		     * We need to keep track of the current modes,
		     * as all changes are relative to the current
		     * file store.
		     * To prevent nasty surprises when running "-f -D"
		     * (meaning force fixes on directories only),
		     * the "consistent" option needs to be set for
		     * this to apply.  Consistent defaults to on only
		     * if "change as much as you want" is specified.
		     */
		    if (consistent) {
			int u, g;
			unsigned int m;

			revert(&line, oldmod);

			u = line.uid;
			g = line.gid;
			m = line.mode;
			line.uid = oldmod->uid;
			line.gid = oldmod->gid;
			line.mode = oldmod->mode;
			secure_file(&line);

			line.uid = u;
			line.gid = g;
			line.mode = m;

			/* Check for any changes made this time round */
			ok = line.newuid == u && line.newgid == g &&
				line.newmode == m;

		    }
		    /* Update mods file accordingly */
		    if (!consistent || !ok) {
			oldmod->newuid = line.newuid;
			oldmod->newgid = line.newgid;
			oldmod->newmode = line.newmode;
		    }
		}

		if (ok)
		    continue;
	    } else
		oldmod = &line;
	}

	if (verbose || debug)
	    report_diffs(&line);

	/* log change to rollback file - when debugging prepend a # */
	if (debug)
	    putc('#', df);
	modlineprint(df, &line);

	/*
	 * To do it safely, changes to  the diff file must
	 * be committed before we do the actual chown/chmod
	 * Rather than doing fflush()/fsync() each time, we
	 * gather a number of chown/chmods in changes.
	 * and sync when our buffer is full;
	 */

	changes[nchanges] = line;
	if (++nchanges == MAXCHANGES) {
	    commit_changes(df);
	    nchanges = 0;
	}
    }
    commit_changes(df);
    if (fclose(df) == EOF || fclose(ncf) == EOF || fclose(cf) == EOF ||
	    (mf && fclose(mf) == EOF)) {
		fprintf(stderr, gettext("%s: ABORT: Can't flush output files: %s\n"),
			program, strerror(errno));
		/*
		 * We must keep the diff file, it's no problem if it's
		 * truncated, we must remove the new files as it is a
		 * problem if they're used
		 */
		(void) unlink(crp(newcontents));
		(void) unlink(crp(newmodsfile));
		exit(1);
    }
    sync();

    /* at the end we're left with:
	- new mods file
	- new contents file
	- diff file
       process as follows:
	- remove diff file - so no rollback
	- rename mods.new -> mods
	- rename contents.new -> contents
    */
    if (!debug) {
	if (unlink(crp(difffile))) {
	    perror("unlink difffile");
	    exit(1);
	}
	if (rename(crp(newcontents), crp(contents))) {
	    perror("rename contents");
	}
	if (rename(crp(newmodsfile), crp(modsfile))) {
	    perror("rename modsfile");
	}
	 /* The mods file is zero sized after an error-free undo run */
	 if (undo && undoerrs == 0)
	    (void) unlink(crp(modsfile));
    }
    return 0;
}

#define NEXTWORD(tmp,end) \
    do { \
	    tmp = strpbrk(tmp,"\t "); if (!tmp) fatal(contents,lineno);\
	    end = tmp++;\
	    while(*tmp && isspace(*tmp)) tmp++; \
	} \
    while (0)

static void
fatal(const char *file, int line)
{
    fprintf(stderr, gettext("%s: error in line format in %s at %d\n"),
	    program, file, line);
    exit(1);
}

static void put_line(FILE *f, cntsline_t *line)
{
    int res;

    if (line->rest)
	res = fprintf(f,"%s%04o %s %s %s",
		line->start, (int) line->newmode, line->owner,
		line->group, line->rest);
    else
	res = fputs(line->start, f);

    if (res == EOF) {
	fprintf(stderr, gettext("%s: ABORT: write (new contents) failed: %s\n"),
			program, strerror(errno));
	exit(1);
    }
}

/* Sun packages and other Solaris platform packages */
static char *sunprefixes[] = {"SUNW", "SICG", "SPRO", "SME", "PFU", "AXIL",
	"FJSV", "TAD", "TSBW", "TWS" };

static int
sunpackage(const char *buf)
{
    int i;
    char *q = strchr(buf,' ');			/* skip pathname */

    if (q == 0)
	return 0;

    for (i = 0; i < sizeof(sunprefixes)/sizeof(char*); i++) {
	char *p = strstr(q, sunprefixes[i]);

	if (p == 0)
	    continue;

	/*
	 * Usually, there's a space in front of the package name,
	 * sometimes there's a "*" or another symbol.
	 */
	if (!isalnum(p[-1]))
	    return 1;
    }
    return 0;
}
	


/*
   the first field is the path, the second the type, the
   third the class, the fourth the mode, when appropriate.
   We're interested in
		f (file)
		e (edited file)
		v (volatile file)
		d (directory)
		c (character devices)
		b (block devices)

    LINE_IGNORE is returned for lines we do no want.
    LINE_ERROR is returned for lines with a parse error,
    at this point we do have the pathname and the code
    calling parse_line will need to sync with the mods file
    in this case and not drop entries from the mods file
    when some patch/pkgadd introduces "?" for modes.
*/
static int
parse_line(char *buf, cntsline_t *parse, int lineno)
{
    char *tmp;
    char *p = buf;
    char *end, *q;

    parse->start = buf;
    parse->rest = 0;		/* makes put_line work */

    if (*p == '#' || *p == ':' || *p == '\n' || *p == '!')
	return LINE_IGNORE;

    /*
     * If it doesn't come from a Sun package, skip it.
     * When undoing, we don't care what package
     * a file is in; if it's in the .mods file, it needs undoing.
     */
    if (!undo && !allpkgs && !sunpackage(buf))
	return LINE_IGNORE;

    /*
     * Parse the contents file line:
     * <path> <type> <class> [<major> <minor>]
     * [ <mode> <owner> <group> .... ]
     */

    /* pathname begins at start of line, end points to last byte */
    NEXTWORD(p,end);	/* after path */

    parse->realtype = parse->type = *p;

    if (strchr("sl", parse->type) != 0)
	return LINE_IGNORE;

    NEXTWORD(p,tmp);	/* after type */

    strncpy(parse->path, buf, end - buf);
    parse->path[end - buf] = '\0';

    /* skip class */
    NEXTWORD(p,end);	/* after class */

    switch (parse->type) {
    case 'e':
    case 'v':
	/* type 'e' and 'v' are files, just like 'f', use 'f' in out */
	parse->type = 'f';
	/* FALLTHROUGH */
    case 'f':
    case 'd':
    case 'p': /* FIFO - assume its modes are sensible, don't treat as file */
	break;

    case 'x': /* Exclusive directory */
	parse->type = 'd';
	break;

    /* device files have class major minor, skip */
    case 'c':
    case 'b':
	NEXTWORD(p,end); NEXTWORD(p,end); /* after major minor */
	break;

    case '?':
	fprintf(stderr, gettext("%s: WARNING: Incomplete entry at %s:%d\n"),
                program, contents, lineno);
	return LINE_ERROR;

    default:
	fprintf(stderr, gettext("%s: ABORT: Unknown type '%c', %s:%d\n"),
                program, parse->type, contents, lineno);
	exit(1);
    }
    tmp = p;
    NEXTWORD(p,end); /* after mode */

    /*
     * the mode is either a 4 digit number (file is sticky/set-uid or set-gid
     * or the mode has a leading 0) or a three digit number
     * mode has all the mode bits, mode points to the three least
     * significant bits of the mode
     */
    parse->mode = 0;
    for (q = tmp; q < end; q++) {
	if (!isdigit(*q) || *q > '7') {
	    fprintf(stderr,
                    gettext("WARNING: Unparseble mode \"%.*s\" at %s:%d\n"),
                    end-tmp, tmp, contents, lineno);
	    
	    return LINE_ERROR;
	}
	parse->mode <<= 3;
	parse->mode += *q - '0';
    }
    tmp[0] = '\0';

    parse->owner = p;
    NEXTWORD(p,end); /* after owner */
    *end = '\0';

    parse->group = p;
    NEXTWORD(p,end); /* after group */
    *end = '\0';

    parse->rest = end+1;

    parse->newmode = parse->mode;
    parse->newuid = parse->uid = str2uid(parse->owner);
    parse->newgid = parse->gid = str2gid(parse->group);

    return LINE_OK;
}

/* return 1 if the file is already "secure" */
static int secure_file(cntsline_t *line)
{
    int i;
    int changed = 0;

    int mode_diff = 0;

    if (dirsonly && line->type != 'd')
	return 1;

    if (dosu && line->type == 'f' && (line->mode & (S_ISUID|S_ISGID)))
	mode_diff = line->mode & (S_IRGRP|S_IROTH);

    /* The following heuristics are used to determine whether a file
     * can be safely chown to root:
	    - it's not set-uid.
	    and one of the following applies:
		- it's not writable by the current owner and is
		  group/world readable
		- it's world executable and a file
		- owner, group and world permissions are identical
		- it's a bin owned directory or "non-volatile" file (any owner)
		  for which group and other r-x permissions are identical
		  or it's a bin-owned executable
    */

    if (doowner && !(line->mode & S_ISUID)
	    && line->newuid &&
	    (
	    (   !(line->mode & S_IWUSR) &&
		(line->mode&(S_IRGRP|S_IROTH)) == (S_IRGRP|S_IROTH)
	    )
	    ||
		(line->type == 'f' && (line->mode & S_IXOTH))
	    ||
		 ((line->mode & 07) == ((line->mode>>3) & 07) &&
		  (line->mode & 07) == ((line->mode>>6) & 07) &&
		  strcmp(line->owner,"uucp") != 0)
	    ||
		((line->type == 'd' && strcmp(line->owner,"bin") == 0 ||
		 line->realtype == 'f')
		 && ((line->mode & 05) == ((line->mode>>3) & 05) ||
		     (line->mode & 0100) && strcmp(line->owner,"bin") == 0)
		 && ((line->mode & 0105) != 0 ||
			strncmp(line->path, "/etc/security/dev/", 18) == 0)
		 )
	    )) {
	setnewuid(line, 0);
	changed = 1;
    }
    /*
     * Strip user write bit if owner != root and executable by user.
     * root can write even if no write bits set
     * Could prevent executables from being overwritten.
     */
    if (douserwrite && line->type == 'f' && line->newuid &&
	    (line->mode & (S_IWUSR|S_IXUSR)) == (S_IWUSR|S_IXUSR))
	mode_diff |= S_IWUSR;

    /* Sticky directories will keep group and world write */
    if (domodes && (line->mode & (S_IWGRP|S_IWOTH)) != 0 &&
	    (line->mode & S_ISVTX) == 0) {
	for (i = 0; i < nexceptions; i++) {
	    if (strcmp(line->path, exceptions[i]) == 0)
		break;
	}
	if (i == nexceptions)
	    mode_diff |= line->mode & (S_IWGRP|S_IWOTH);
    }

    if (mode_diff) {
	line->newmode &= ~mode_diff;

	if (line->mode != line->newmode)
	    changed = 1;
    }
    return changed == 0;
}

static int parsemodline(FILE *f, cntsline_t *line)
{
    char buf[1024];

    while (1/*CONSTCOND*/) {
	if (fgets(buf, sizeof(buf), f) == 0)
	    return -1;

	if (strchr(buf, '\n') == 0) {
	    fprintf(stderr, gettext("%s: incomplete mode file\n"), program);
	    return -1;
	}
	if (*buf == '#')
	    continue;

	if (sscanf(buf,"%s %d:%d:%o %d:%d:%o", line->path,
	    &line->newuid, &line->newgid, &line->newmode,
	    &line->uid, &line->gid, &line->mode) != 7) {
	    fprintf(stderr, gettext("%s: invalid mode line: %s\n"),
                    program, buf);
	    continue;
	}
	return 0;
    }
    /* NOTREACHED */
}

static FILE *modf;
static cntsline_t mods;

static int loadmods(const char *mfile)
{
    modf = fopen(mfile, "r");

    if (modf == 0)
	return 0;

    if (parsemodline(modf, &mods) == 0)
	return 1;

    (void) fclose(modf);
    modf = 0;
    return 0;
}

static void modlineprint(FILE *f, cntsline_t *a)
{
    if (f && a) {
	if (a->newuid == a->uid && a->newgid == a->gid && a->newmode == a->mode)
	    fprintf(stderr, gettext("%s: reset to original mode\n"), a->path);
	else
	    if (fprintf(f, "%s %d:%d:%o %d:%d:%o\n", a->path,
		a->newuid, a->newgid, a->newmode,
		a->uid, a->gid, a->mode) == EOF) {
		    fprintf(stderr, gettext("%s: Can't write output\n"),
                            program);
		    exit(1);
	    }
    }
}

/*
 * search entry, we may already have read one; flag entries dropped;
 * contents and mods need to be sorted
 */
static cntsline_t *modentry(cntsline_t *line)
{
    static int used = 0;
    if (modf == 0)
	return 0;

    do {
	int cmp;
	
	if (used) continue;

	cmp = strcmp(mods.path, line->path);

	/* match */
	if (cmp == 0) {
	    used = 1;
	    return &mods;
	}
	/* overshoot */
	if (cmp > 0)
	    return 0;

	fprintf(stderr, gettext("%s: WARNING: %s no longer exists\n"), program,
		mods.path);
    } while (used = 0, parsemodline(modf, &mods) == 0);
    (void) fclose(modf);
    modf = 0;
    return 0;
}

/* Execute NULL rollback w/o complaints */
static void dorollback(const char *d, int doit)
{
    FILE *f = fopen(d,"r");
    cntsline_t l;

    if (f == 0) {
	perror(d);
	exit(1);
    }
    while (parsemodline(f, &l) == 0) {
	if (!doit) {
	    fprintf(stderr,
                gettext("%s: ABORT: rollback needed but -r not specified\n"),
		program);
	    exit(1);
	}
	if (debug || verbose) {
	    if (l.uid != l.newuid || l.gid != l.newgid)
		printf("lchown(%s, %d, %d)\n", l.path, (int)l.uid, (int)l.gid);
	    if (l.mode != l.newmode)
		printf("chmod(%s, %04o)\n", l.path, (int) l.mode);
	}
	if (!debug) {
	    /* XXX: bail out on errors */
	    if (l.uid != l.newuid || l.gid != l.newgid)
		(void) lchown(crp(l.path), l.uid, l.gid);
	    if (l.mode != l.newmode)
		(void) chmod(crp(l.path), l.mode);
	}
    }
    (void) fclose(f);
}

static void commit_changes(FILE *df)
{
    int i;

    if (fflush(df) == EOF || fsync(fileno(df))) {
	fprintf(stderr, gettext("%s: ABORT: writing rollback file failed: %s\n"),
		program, strerror(errno));
	exit(1);
    }
    if (debug)
	return;
    for (i = 0; i < nchanges;i++) {
	if (changes[i].newuid != changes[i].uid ||
		changes[i].newgid != changes[i].gid)
	    (void) lchown(crp(changes[i].path), changes[i].newuid,
			changes[i].newgid);
	if (changes[i].newmode != changes[i].mode)
	    (void) chmod(crp(changes[i].path), changes[i].newmode);
    }
}

/*
 * Caching speeds things up here; uncached we spend most of our time here.
 * (70%), even w/ nscd.  Cached we go to 3.5 from 11 seconds.
 */

#ifndef NOIDCACHE
struct idcache {
    char	*key;
    int		id;
};

/*
 * Sorted on my contents file before fixing it, with root moved to front
 * Some people remove or rename standard users and groups, default to the
 * standard uids and gids in those cases.
 */
static struct idcache _uidcache[] = {
    { "root", 0 },
    { "bin", 2 },
    { "lp", 71 },
    { "uucp", 5 },
    { "adm", 4 },
    { "sys", 3 },
    { NULL, -1}		/* cache for lru */
};

/* Sorted on the statistics of my contents file */
static struct idcache _gidcache[] = {
    { "bin", 2 },
    { "sys", 3 },
    { "lp", 8 },
    { "other", 1 },
    { "uucp", 5},
    { "root", 0 },
    { "adm", 4 },
    { "mail", 6 },
    { "tty", 7 },
    { "staff", 10 },
    { "daemon", 12 },
    { NULL, -1}		/* cache for lru */
};


typedef struct cache {
    void *(*byname )(const char* name);
    void *(*byid )(int id);
    int n_off, i_off;
    int size;
    struct idcache *idcache;
    int loaded;
} cache_t;

static cache_t uidcache = {
    (void *(*)(const char*)) getpwnam,
    (void *(*)(int)) getpwuid,
    offsetof(struct passwd, pw_name),
    offsetof(struct passwd, pw_uid),
    sizeof(_uidcache)/sizeof(struct idcache),
    _uidcache,
    0
}, gidcache = {
    (void *(*)(const char*)) getgrnam,
    (void *(*)(int)) getgrgid,
    offsetof(struct group, gr_name),
    offsetof(struct group, gr_gid),
    sizeof(_gidcache)/sizeof(struct idcache),
    _gidcache,
    0
};


static void loadcache(cache_t * cache)
{
    int i;
    char *lookup;
    struct idcache *c = cache->idcache;

    for (i = 0; i < cache->size-1; i++) {
	lookup = (char*) cache->byname(c[i].key);
	if (lookup) {
	    /* LINTED */
	    int id = *(int*)(lookup+cache->i_off);
	    if (c[i].id != -1 && c[i].id != id)
		fprintf(stderr, gettext("%s: WARNING: unexpected id %d for %s(%d)\n"),
			program, id, c[i].key, c[i].id);
	    c[i].id = id;
	} else {
	    fprintf(stderr, gettext("%s: %s: Can't find required uid/gid %s\n"),
		    program, c[i].id == -1 ? "ERROR" : "WARNING", c[i].key);
	    if (c[i].id == -1)
		exit(1);
	}
    }
    cache->loaded = 1;
}

static char *getidbyid(int id, cache_t *cache)
{
    int i;
    char *lookup;
    struct idcache *c = cache->idcache;

    if (!cache->loaded)
	loadcache(cache);

    for (i = 0; i < cache->size; i++)
	if (c[i].key && c[i].id == id) {
	    return c[i].key;
	}

    i = cache->size-1;
    c[i].key = NULL;
    lookup = (char*) cache->byid(id);
    if (!lookup) {
	fprintf(stderr, gettext("%s: Unknown id: %d\n"), program, id);
	return NULL;
    }
    /* LINTED */
    c[i].key = *(char**)(lookup+cache->n_off);
    /* LINTED */
    c[i].id = *(int*)(lookup+cache->i_off);
#if 0
    fprintf(stderr, "miss %p %s %d\n", cache->byname, c[i].key, c[i].id);
#endif
    return c[i].key;
}

static int getidbyname(const char *name, cache_t * cache)
{
    int i;
    char *lookup;
    struct idcache *c = cache->idcache;

    if (!cache->loaded)
	loadcache(cache);

    for (i = 0; i < cache->size; i++)
	if (c[i].key && strcmp(c[i].key, name) == 0) {
	    return c[i].id;
	}

    i = cache->size-1;
    c[i].key = NULL;
    lookup = (char*) cache->byname(name);
    if (!lookup) {
	char *c;
	long id;

	id = strtol(name, &c, 0);
	if (c == name || *c != '\0') {
	    fprintf(stderr, gettext("%s: Unknown name: %s\n"), program, name);
	    return -1;
	} else
	    return id;
    }
    /* LINTED */
    c[i].key = *(char**)(lookup+cache->n_off);
    /* LINTED */
    c[i].id = *(int*)(lookup+cache->i_off);
#if 0
    fprintf(stderr, "miss %p %s %d\n", cache->byname, c[i].key, c[i].id);
#endif
    return c[i].id;
}
#endif /* NOIDCACHE */

static uid_t str2uid(const char *user)
{
#ifdef NOIDCACHE
    struct passwd *pwd = getpwnam(user);

    return pwd ? pwd->pw_uid : -1;
#else
    return getidbyname(user, &uidcache);
#endif
}

static char * uid2str(uid_t user)
{
    static char buf[10];
#ifdef NOIDCACHE
    struct passwd *pwd = getpwuid(user);

    if (pwd)
	return pwd->pw_name;
#else
    char *str = getidbyid(user, &uidcache);
    if (str)
	return str;
#endif
    sprintf(buf,"%d", (int) user);
    return buf;
}

static gid_t str2gid(const char *group)
{
#ifdef NOIDCACHE
    struct group *grp = getgrnam(group);

    return grp ? grp->gr_gid : -1;
#else
    return getidbyname(group, &gidcache);
#endif
}

static char * gid2str(gid_t group)
{
    static char buf[10];
#ifdef NOIDCACHE
    struct group *grp = getgrgid(group);

    if (grp)
	return grp->gr_name;
#else
    char *str = getidbyid(group, &gidcache);
    if (str)
	return str;
#endif
    sprintf(buf,"%d", (int) group);
    return buf;
}

static void setnewuid(cntsline_t *l, int uid)
{
    if (l->newuid != uid) {
	l->owner = uid2str(uid);
	l->newuid = uid;
    }
}

static void setnewgid(cntsline_t *l, int gid)
{
    if (l->newgid != gid) {
	l->group = gid2str(gid);
	l->newgid = gid;
    }
}

static void revert(cntsline_t *l, cntsline_t *o)
{
    if (!o)
	o = l;

    setnewuid(l, o->uid);
    setnewgid(l, o->gid);
    l->newmode = o->mode;
}

/* concactenate in malloc'ed string */
static char *strdupcat(const char *a, const char *b)
{
    char *res = malloc(strlen(a)+strlen(b)+1);

    strcpy(res, a);
    strcat(res, b);

    return res;
}

/* fopen or bail out */
static FILE *fopenfile(const char *name, const char *mode)
{
    FILE *res = fopen(name, mode);

    if (res == 0) {
	fprintf(stderr, gettext("%s: Can't open %s: %s\n"), program, name,
		strerror(errno));
	exit(1);
    }
    return res;
}

static const char *crp(const char * path)
{
    if (!rootdir)
	return path;

    while (*path == '/')
	path++;

    return path;
}

/* one final check; make we're changing from the modes as they exist */
static int check_mode(cntsline_t *l, int force)
{
    struct stat realmode;
    char t, *longtype;
    if (lstat(crp(l->path), &realmode) != 0) {
	fprintf(stderr,"%s: %s\n", l->path, strerror(errno));
	return -1;
    }
    switch (realmode.st_mode & S_IFMT) {
    case S_IFIFO:
	t = 'p'; longtype = "named pipe"; break;
    case S_IFCHR:
	t = 'c'; longtype = "character special file"; break;
    case S_IFBLK:
	t = 'b'; longtype = "block special file"; break;
    case S_IFDIR:
	t = 'd'; longtype = "directory"; break;
    case S_IFREG:
	t = 'f'; longtype = "regular file"; break;
    case S_IFLNK:
	t = 's'; longtype = "symbolic link"; break;
    case S_IFSOCK:
	t = 'X'; longtype = "socket"; break;
#ifdef S_IFDOOR
    case S_IFDOOR:
	t = 'D'; longtype = "door"; break;
#endif
    default:
	t = 'X'; longtype = "unknown type"; break;
    }

    if (l->type != t) {
	fprintf(stderr, gettext("%s: didn't expect a %s\n"), l->path, longtype);
	return -1;
    }

    /*
     * The fs types listed here are the ones expected to
     * contain real files that can be modified.
     */
    if (strcmp(realmode.st_fstype,"ufs") != 0 &&
	strcmp(realmode.st_fstype,"lofs") != 0 &&
	strcmp(realmode.st_fstype,"nfs") != 0) {
	if (quiet < 2)
	    fprintf(stderr,
                    gettext("%s: WARNING: %s: wrong fstype: %s (won't change)\n"),
                    program, l->path, realmode.st_fstype);
	return -1;
    }
	
    if (realmode.st_uid != l->uid || realmode.st_gid != l->gid ||
	    (realmode.st_mode & 07777) != l->mode) {
	if (quiet < 3) {
            /*
            ** XXX: This may not localize well in two parts like this, but
            ** uid2str maintains a static buffer so we can't combine the two
            ** messages
            */
	    fprintf(stderr, gettext("%s: expected <%s:%s:%04o>"),
		    l->path, uid2str(l->uid), gid2str(l->gid),
		    l->mode);
	    fprintf(stderr, gettext(" actual <%s:%s:%04o>\n"),
		uid2str(realmode.st_uid), gid2str(realmode.st_gid),
		realmode.st_mode & 07777);
	}
	return force ? 0 : 1;
    }
    return 0;
}

static void report_diffs(cntsline_t *l)
{
    int mode_diff = l->mode ^ l->newmode;
    if (l->newuid != l->uid)
	printf("%c o %s -> %s %s [%04o]\n", l->realtype, uid2str(l->uid),
		l->owner, l->path, (int) l->mode);
    if (mode_diff)
	printf("%c %c %04o -> %04o %s\n", l->realtype,
	    (mode_diff & (S_IRGRP|S_IROTH)) ? 's' : 'm',
	    (int) l->mode, (int) l->newmode, l->path);
}
