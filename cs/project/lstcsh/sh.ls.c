/* $Header: sh.ls.c,v 0.01 1998.04.25  lumpy@cs.ucc.ie  */
/*
 * sh.ls.c: All load sharing funcs.
 */

#include "sh.h"
#include <protocols/rwhod.h>
#include <dirent.h>
#include <values.h>

#define	DefaultHWM	"1.0"
#define	DefaultLWM	"0.5"
#define	DefaultRExec	"rexec"
#define MaxRecvTime	600
#define MaxHostNameLen	40

/* Initializes all the LS environment vars	*/
void ls_init()
{
 set(STRlsOn, SAVE(""), VAR_READWRITE);
 set(STRlsHWM, SAVE(DefaultHWM), VAR_READWRITE);
 set(STRlsLWM, SAVE(DefaultLWM), VAR_READWRITE);
 set(STRlsRExec, SAVE(DefaultRExec), VAR_READWRITE);
}/* ls_init */


/* Adds in an "rexec remoteHost" at the start of the command list.	*/
void ls_changeCommand(struct wordent* commandList, char* remoteHost)
{
 struct wordent* rexec = xmalloc(sizeof(struct wordent));
 struct wordent* host = xmalloc(sizeof(struct wordent));

 rexec->prev = commandList;
 rexec->next = host;
 rexec->word = Strsave(varval(STRlsRExec));

 host->prev = rexec;
 host->next = commandList->next;
 host->word = SAVE(remoteHost);

 commandList->next = rexec;

 free(remoteHost);
}/* ls_changeCommand */



/* Finds the host with the lowest load avg ( below the LWM ) and	*/
/* with the fewest users.  Returns NULL if none is found.		*/
/* All hosts are checked unless lsHosts is defined.			*/
char* ls_findRemoteHost()
{
 DIR*		dirPtr;
 struct dirent*	dirEntry;
 struct whod	hostInfo;
 char*		filename;
 char*		remoteHost = xmalloc(MaxHostNameLen * sizeof(char));
 double		loadAvg, lowestLoad = atof(short2str(varval(STRlsLWM)));
 int		numUsers, lowestNumUsers = MAXINT;
 int		len, fd, match;
 int		whodHeaderSz = sizeof(hostInfo) - sizeof(hostInfo.wd_we);
 time_t		now;
 struct varent* lsHostsVar;
 Char**		lsHosts;
 Char**		hn;

 time(&now);
 strcpy(remoteHost,"");

 /* If lsHosts is undefined then search all hosts	*/
 if ((lsHostsVar = adrof(STRlsHosts)) == NULL)
   lsHosts = NULL;
 else
   lsHosts = lsHostsVar->vec;

 if ((dirPtr = opendir(_PATH_RWHODIR)) == NULL && adrof(STRlsDebug) )
   xprintf("Could not open %s.",_PATH_RWHODIR);


 /* Go through all files in the rwho directory and find the	*/
 /*  the machine with the lowest load.				*/
 while ((dirEntry = readdir(dirPtr)) != NULL){
   if (dirEntry->d_ino == 0 || strncmp(dirEntry->d_name, "whod.", 5))
     continue;

   /* Search for a match in lsHosts ( if its defined. )		*/
   if ( hn=lsHosts ){
     match = 0;
     while (*hn){
       if ( !strcmp(short2str(*hn),(dirEntry->d_name)+5) ){
         match = 1;
         break;
       }
       hn++;
     }
     if ( !match ) continue;
   }

   filename = xmalloc( (strlen(_PATH_RWHODIR)+strlen(dirEntry->d_name)+2)*sizeof(char) );
   strcpy(filename,_PATH_RWHODIR); strcat(filename,"/"); strcat(filename,dirEntry->d_name);

   fd = open(filename, O_RDONLY, 0);
     if ( fd < 0 ) continue;
     len = read(fd, &hostInfo, sizeof(hostInfo));
   close(fd);
   free(filename);

   if (len < whodHeaderSz) continue;

   numUsers = (len - whodHeaderSz)/sizeof(struct whoent);
   loadAvg = (hostInfo.wd_loadav[0]/100.0);

   if (	( (now - hostInfo.wd_recvtime) < MaxRecvTime) && 
	( (loadAvg < lowestLoad) ||
	( (loadAvg == lowestLoad) && (numUsers < lowestNumUsers) ) ) ){
     lowestLoad = loadAvg;
     lowestNumUsers = numUsers;
     strcpy(remoteHost,hostInfo.wd_hostname);
   }
 }

 closedir(dirPtr);

 if ( !strcmp(remoteHost,"") ){
   free(remoteHost);
   return(NULL);
 }else{
   return(remoteHost);
 }
}/* ls_findRemoteHost */



/* Makes the decision whether to share or not. If 	*/
/* so the commandList is modified.			*/
void ls_main(commandList)
    struct wordent *commandList;
{
 register struct varent* lsCmdsVar;
 register Char** lsCmds;
 char* firstWord;
 char* remoteHost;

 if (!adrof(STRlsOn))
   return;

 if ((lsCmdsVar = adrof(STRlsCmds)) == NULL)
   return;

 /* This bollox is neccessary 'cause short2str returns a static!	*/
 firstWord = xmalloc(sizeof(char) * (strlen(short2str(commandList->next->word))+1));
 strcpy(firstWord,short2str(commandList->next->word));

 lsCmds = lsCmdsVar->vec;
 while (*lsCmds){
   if (	!strcmp(short2str(*lsCmds),firstWord) ){
     if (adrof(STRlsDebug)) xprintf("%s is migratable.\n",firstWord); 
     if ( (load > atof(short2str(varval(STRlsHWM)))) && 
          ((remoteHost = ls_findRemoteHost()) != NULL) ){
       ls_changeCommand(commandList,remoteHost);
       break;
     }
   }
   lsCmds++;
 }

 free(firstWord);
}/* ls_main */
