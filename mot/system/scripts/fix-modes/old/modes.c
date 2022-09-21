/*
 * $Id$
 *
 * Program to change most mode 664/666 and 775/777 files in Solaris 2.x
 * to a more secure mode (664/755).
 *
 * Also creates a new /var/sadm/install/contents file.
 * Does NOT honor any locking scheme as used by pkg*
 *
 * No files are actually changed. All files of which the
 * modes are deemed in appropriate are printed to stdout so
 * they can be fed to xargs chmod og-w.
 *
 * Sample usage:
 * 	modes | xargs chmod og-w
 * 	mv /var/sadmin/install/contents /var/sadmin/install/contents.org
 * 	mv /tmp/contents /var/sadmin/install/contents
 *
 * Casper Dik (casper@fwi.uva.nl)
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

char * exceptions [] = {
#include "exceptions.h"
};

int nexceptions = sizeof(exceptions)/sizeof(char*);

void
fatal(int line)
{
    fprintf(stderr,"modes: error in line format at line %d\n", line);
    exit(1);
}

#define TMPFILE "/tmp/contents"

int
main(int argc, char **argv)
{
    FILE *cnts;
    FILE *out;
    int fd;
    char buf[8192];
    int i;
    char *contentsfile;
    unsigned int lineno = 0;

    umask(022);

    if (argc > 2) {
	fprintf(stderr,"Usage: %s [contents file]\n", *argv);
	exit(1);
    }
    contentsfile = argc == 2 ? argv[1] : "/var/sadm/install/contents";

    cnts = fopen(contentsfile,"r");
    if (cnts == 0) {
	fprintf(stderr,"can't open contents data base\n");
	exit(1);
    }
    remove(TMPFILE);
    fd = open(TMPFILE, O_CREAT|O_WRONLY|O_EXCL, 0666);
    if (fd >= 0)
	out = fdopen(fd, "w");
    if (fd < 0 || out == 0) {
	perror(TMPFILE);
	exit(1);
    }
    for (;fgets(buf, sizeof(buf), cnts) != 0; fputs(buf, out)) {
	/* the first field is the path, the second the type, the
	   third the class, the fourth the mode, when appropriate.
	   We're interested in
			f (file)
			e (edited file)
			v (volatile file)
			d (directory)
			c (character devices)
			b (block devices)
				
	 */
	 char type;
	 char *end_of_name;
	 char *mode;
	 char *tmp;
	 int gm, om;

	 lineno ++;

	 if (buf[0] == '#')
	    continue;
	 tmp = strchr(buf, ' ');
	 if (!tmp) fatal(lineno);
	 end_of_name = tmp;
	 tmp++;
	 type = *tmp;
	 tmp += 2;
	 switch (type) {
	 case 'f':
	 case 'e':
	 case 'v':
	 case 'd':
	    break;

	 /* device files have class major minor */
	 case 'c':
	 case 'b':
	     tmp = strchr(tmp, ' ');
	     if (!tmp) fatal(lineno);
	     tmp++;
	     tmp = strchr(tmp, ' ');
	     if (!tmp) fatal(lineno);
	     tmp++;
	     break;

	 case 's':
	 case 'l':
	    continue;
	 case '?':
	    fprintf(stderr,"Warning: incomplete entry at line %d\n", lineno);
	    continue;
	default:
	    fprintf(stderr,"Unknown type '%c', line %d\n",type, lineno);
	    exit(1);
	}
	tmp = strchr(tmp, ' ');
	if (!tmp) fatal(lineno);
	tmp++;
	mode = tmp;
	tmp = strchr(tmp, ' ');
	if (!tmp) fatal(lineno);
	if (tmp - mode != 4 || !isdigit(mode[0])) {
	    fprintf(stderr,"Warning: Unparseble mode \"%.*s\" at line %d\n",
			tmp-mode, mode, lineno);
	    continue;
	}

	gm = (mode[2] - '0') & 2;
	om = (mode[3] - '0') & 2;
	if (gm == 0 && om == 0)
	    continue;
	*end_of_name = 0;
	for (i = 0; i < nexceptions; i++) {
	    if (strcmp(buf, exceptions[i]) == 0)
		break;
	}
	if (i == nexceptions) {
	    printf("%s\n", buf);
	    if (gm)
		mode[2] -= 2;
	    if (om)
		mode[3] -= 2;
	}
	*end_of_name = ' ';
    }
    fflush(out);
    if (ferror(out)) {
	fprintf(stderr,"modes: Error writing output\n");
	exit(1);
    }
    exit(0);
}
