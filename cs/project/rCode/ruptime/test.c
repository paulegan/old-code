

#include <sys/param.h>

#include <protocols/rwhod.h>

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


int
main(argc, argv)
	int argc;
	char **argv;
{
	struct dirent *dp;
	struct whod awhod;
	DIR *dirp;
	char* filename;
	char remoteHost[32];
	int len, fd, numUsers;
	int lowestLoad = 100000, lowestNumUsers = 100000;
	time_t now;
	int whodHeaderSz = sizeof(awhod) - sizeof(awhod.wd_we);
	
	time(&now);

	if ((dirp = opendir(_PATH_RWHODIR)) == NULL)
	  printf("dirp is null!");

	while ((dp = readdir(dirp)) != NULL){
		if (dp->d_ino == 0 || strncmp(dp->d_name, "whod.", 5)) continue;
		filename = malloc( (strlen(_PATH_RWHODIR)+strlen(dp->d_name)+2)*sizeof(char) );
		strcpy(filename,_PATH_RWHODIR);
		strcat(filename,"/");
		strcat(filename,dp->d_name);
		fd = open(filename, O_RDONLY, 0);
		free(filename);
		if ( fd < 0 ) continue;
		len = read(fd, &awhod, sizeof(awhod));
		close(fd);
		if (len < whodHeaderSz) continue;

		numUsers = (len - whodHeaderSz)/sizeof(struct whoent);

		if (	((now - awhod.wd_recvtime) < 720) &&
			(awhod.wd_loadav[0] <= lowestLoad) &&
			(numUsers < lowestNumUsers) )
		{
		  lowestLoad = awhod.wd_loadav[0];
		  lowestNumUsers = numUsers;
		  strcpy(remoteHost,awhod.wd_hostname);
		}
	}
	closedir(dirp);

	printf("Host: %s\n",remoteHost);
	exit(0);
}

