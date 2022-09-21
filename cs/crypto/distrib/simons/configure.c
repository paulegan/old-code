/*
 * file: configure.c
 * author: Simon Foley, Nov 25, 1996.
 *
 * This program is part of a simple demonstration of the Wide-Mouth Frog 
 * protocol. Use this program to set up initial secret keys for clients 
 * to share with trent.  Also saves these keys into a file trentkeys that
 * are loaded on startup by trent. 
 */

#include <stdio.h>
#include "cryproto.h"

void main(){

    char principle[10];
    char *key; 

    startSession("configure", "cryptoServer");

    printf("Name: ");
    scanf("%s", &principle);

    while (strcmp(principle,".")){
        key=newDESKey();
        printf("Allocating DES key %s between Trent and %s\n", 
                           key, principle);
        storeKey(principle,key);
        free(key); 

        printf("Name: ");
        scanf("%s", &principle);
    }/*while*/

    saveKeyList("trentkeys"); 
    freeKeyList();

    endSession();
    printf("Bye\n"); 
}/*main*/ 
