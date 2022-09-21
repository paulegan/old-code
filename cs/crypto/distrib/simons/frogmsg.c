#include <stdio.h>
#include <assert.h>
#include "tester.h"
#include "frogmsg.h"


Msg mk_msg(const msg_kind k, const char *fp, 
           const char *tp, const char *ky){
    Msg msg; 

    msg= (Msg)malloc(sizeof(msg_rec));
    msg->kind= k; 
    strcpy(msg->fromprin,fp);
    strcpy(msg->toprin,tp);
    strcpy(msg->key,ky);
    return(msg);
}/*mk_msg*/

Msg cnv_msg_rec(char *cp){
    char buff[SIZE];
    char *holdcp;
    Msg msg; 

    assert(is_msg_str(cp));
    holdcp=cp;
    msg= (Msg)malloc(sizeof(msg_rec));
    strtok(cp, DELIMITER);
    if (!strcmp(cp,USE))    msg->kind=use;
    else                       msg->kind=fwd;

    strcpy(msg->fromprin,(char*)strtok(NULL,DELIMITER)); 
    strcpy(msg->toprin,  (char*)strtok(NULL,DELIMITER)); 
    strcpy(msg->key,     (char*)strtok(NULL,DELIMITER)); 
    free(holdcp);
    return(msg);
}/*cnv_msg_rec*/

char *cnv_msg_str(Msg mp){
    char *cp;

    cp= (char*)malloc(1+ 1 + strlen(mp->fromprin) 
                       + 1 + strlen(mp->toprin) 
                       + 1 + strlen(mp->key) +1); 
    if (mp->kind==use)
        strcpy(cp,USE);
    else
        strcpy(cp,FWD); 
    strcat(cp,DELIMITER);
    strcat(cp,mp->fromprin);
    strcat(cp,DELIMITER);
    strcat(cp,mp->toprin);
    strcat(cp,DELIMITER);
    strcat(cp,mp->key);
    free(mp);
    return(cp);
}/*cnv_msg_rec*/


int is_msg_str(char *mp){
    int i=0;

    while ((mp=(char*)strstr(mp+1,DELIMITER)) != NULL) 
        i++;
    return(i==3); 
}/*is_msg_str*/
