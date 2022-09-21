/*
 * file: trent.c
 * author: Simon Foley, Nov 25, 1996.
 *
 * This program is a simple demonstration of the Wide-Mouth Frog protocol. 
 * This program implements trent, the secure forwarding service. 
 */
#include <stdio.h>
#include "cryproto.h"           /*a useful collection of crypto & protocol fns*/
#include "frogmsg.h" 


#define CRYPTOSERVER "cryptoServer"    /* must be running for cryproto.h to work*/
#define TRENT "trent"                   /*my name registered with CRYPTOSERVER*/
#define KEYLIST "trentkeys"   /* a local file containing secret keys shared   */
                                            /*with the clients in the protocol*/
int forward(); 

void main(){

    char *msg;
    int i,n; 

    startSession(TRENT, CRYPTOSERVER);
    loadKeyList(KEYLIST);

    printf("Hi, I'm trusted third party, %s\n", TRENT); 
    printf("I'm offering a trusted key forwarding service\n"); 

    while (1){
        forward(); 
    }/*while*/

   endSession();
}/*main*/


/*
 * This is all Trent does: recieves messages from `alice' clients, 
 * decrypts the key material (encrypted with secret key shared by 
 * alice and trent), re-encrypts it with the secret key for bob, 
 * and then forwards it to bob. Message is discarded if the shared
 * key does not exist. 
 */
int forward(){

    char *smsg;                      /*the communicated message(string format)*/
    Msg msg;                                       /*and its struct based form*/
    char *Kat;                            /*shared key between alice and trent*/
    char *Kbt;                              /*shared key between bob and trent*/
    char *Kab;          /*proposed session key (enroute) between alice and bob*/
    char *bob;                                       /*message en-route to bob*/


    smsg =  receiveMessage();                               /*get next message*/
    printf("I just got message: %s\n", smsg); 
    msg= cnv_msg_rec(smsg);                 /*make string into a Msg structure*/

    Kat= getKey(msg->fromprin);                   /*extract alice's secret key*/
    if (Kat==NULL){
        printf("I've no key for %s. I've thrown the message away\n", 
                                                        msg->fromprin); 
        return; 
    }

    Kab= DESdecrypt(msg->key, Kat);             /*get the proposed session key*/
    printf("It's key <%s> from %s to be forwarded to %s\n", Kab, 
            msg->fromprin, msg->toprin);

    bob=msg->toprin;                      /*the message to be forwarded to bob*/
    Kbt= getKey(bob);                                   /*get bob's secret key*/
    if (Kbt==NULL){
        printf("I've no key for %s. I've thrown the message away\n", 
                                                        msg->toprin); 
        return; 
    }

    msg->kind= use;    /*change message kind: bob will use this not forward it*/
    strcpy(msg->key,DESencrypt(Kab, Kbt)); /*encrypt Kab with bob's secret key*/
    smsg= cnv_msg_str(msg);    /*and convert message structure back to string */

    sendMessageTo(bob, smsg);       /*send the ,essage to its destination, bob*/
    printf("I just forwarded it as message: %s\n", smsg);
}/*forward*/ 
