/*
 * file: client.c
 * author: Simon Foley, Nov 25, 1996.
 *
 * This program is a simple demonstration of the Wide-Mouth Frog protocol. 
 * It's simple interface allows a user to carry out a key exchange with 
 * other clients, assuming the existence of a trusted third party TRENT
 * who is willing to act as a secure forwarding service. 
 * 
 * The client program takes arguments: client <name> <key>, where <name> 
 * gives the name of the client (Alice, Bob, etc.), and <key> gives the
 * secret key that <name> shares with TRENT. 
 * Once started, the client may:
 *   * propose new keys to share with another client
 *   * see if any new keys have arrived 
 * the keys are exchanged according to the Wide-Mouth Frog protocol. The
 * client may also eavesdrop the protocol: This is done by a simple replay 
 * attack (described in notes), with the effect that an eavesdropping
 * client should get a copy of every key subsequently exchanged by the 
 * protocol. 
 */
#include <stdio.h>
#include "cryproto.h"           /*a useful collection of crypto & protocol fns*/
#include "frogmsg.h"      /*interface for accessing structure of protocol msgs*/

/*human-user front end: menu options*/
#define ipREC  0
#define ipSEND 1
#define ipEVESDROP 2
#define ipEXIT 9

#define CRYPTOSERVER "cryptoServer"    /* must be running for cryproto.h to work*/
#define TRENT "trent"       /*trusted third party registered with CRYPTOSERVER*/

void reqinstructions();
void reqsendkey(const char *alice, const char *trent);
void reqreceivekey(const char *bob, const char *trent);
void reqevesdrop(const char *trent);

void main(int argc, char *argv[]){

    char principle[SIZE];
    char key[SIZE];

    int i, resp;

    if (argc!=3){
        printf("Error. Command format is: client <name> <secret-key>\n");
        exit(1);
    }/*if*/

    /*Greetings*/ 
    strcpy(principle, argv[1]); 
    strcpy(key,argv[2]);
    printf("Hello %s. %s is providing a trusted key forwarding service\n",
                                                           principle, TRENT);
    printf("under the wide-mouth-frog protocol. \n");
    printf("You share secret key %s with %s\n", key, TRENT);

    startSession(principle, CRYPTOSERVER);        /*register with CRYPTOSERVER*/
                                                         /*this *must* be done*/
    storeKey(TRENT, key);                  /* the secure key shared with trent*/

    reqinstructions();                          /*give `menu' options to user */

    while (1){
        printf("?: "); scanf("%d", &resp);

        if (resp==ipREC)     reqreceivekey(principle,TRENT);
        else 
        if (resp==ipSEND)    reqsendkey(principle,TRENT);
        else
        if (resp==ipEVESDROP) reqevesdrop(TRENT);
        else
        if (resp==ipEXIT)    break;
        else{
            printf("Invalid input\n"); 
            reqinstructions();
        }/*ifelse*/ 
    }/*while*/ 

    printf("Bye.\n"); 
    freeKeyList();
    endSession();

}/*main*/ 

/*
 * Output the `menu' options to user.
 */
void reqinstructions(){
    printf("What would you like me to do?\n");
    printf("    %d - see if message has arrived\n", ipREC);
    printf("    %d - propose a session key\n", ipSEND);
    printf("    %d - eavesdrop and subvert the protocol\n", ipEVESDROP);
    printf("    %d - exit\n",ipEXIT); 
}/*instructions*/ 

/*
 * This client (represented as alice) wants to send a new key Kab to bob 
 * (name requested from user). This corresponds to the first step of the
 * wide-mouth frog protocol. 
 *  
 */
void reqsendkey(const char *alice, const char *trent){
    char *Kab;                     /*proposed secret key between alice and bob*/
    char bob[10];                                                 /*bob's name*/
    char* smsg;                                   /*message sent with key etc.*/

    printf("Principle: ");     
    scanf("%s", bob);                              /*get principle `bob's name*/


    Kab=newDESKey();              /*cryptoserv generates good fresh keys here!*/
    printf("I'm proposing to share key %s with %s\n", Kab, bob);

                                      /*build up the message to be sent to bob*/
    smsg= cnv_msg_str(mk_msg(fwd,alice, bob, DESencrypt(Kab,getKey(trent))));
    sendMessageTo(trent, smsg);               /*but remember it goes via trent*/

    printf("I just sent %s to %s via %s\n", smsg, bob, trent);
}/*reqsendkey*/

/*
 * This client (called bob) looks to see if a message has arrived from 
 * trent. If so, extract the key the message contains and adds it to 
 * the current list of keys for this client. This corresponds to step
 * 2 of the wide mouth frog protocol. 
 */ 
void reqreceivekey(const char *bob, const char *trent){
    char *smsg;                             /*the message received as a string*/
    Msg msg;                        /*smsg converted into its component struct*/
    char *Kab;                            /*the proposed shared key with alice*/

    if (newMessage()){                        /*then there is a message for us*/
        smsg=receiveMessage();                  /*get it the message(a string)*/
        printf("I just got message: %s\n", smsg); 
        msg=cnv_msg_rec(smsg);     /*convert it to a Msg, to access components*/

        Kab= DESdecrypt(msg->key,getKey(trent));             /*extract the key*/
        storeKey(msg->fromprin,Kab);                      /*add it to our list*/
        printf("Its a key <%s> from %s\n", Kab, msg->fromprin);
    }else
        printf("Sorry, no message\n"); 
}/*reqreceivekey*/ 

/*
 * Inform CRYPTOSERVER that client wants a copy of every message
 * sent to trent.
 */
void reqevesdrop(const char *trent){
    listenTo(trent);
    printf("Now doing a replay attack on messages sent to trusted %s\n",trent); 
    printf("These will be replayed so that you will subsequently be sent \n"); 
    printf("all session keys exchanged using this protocol!\n");
}/*eavesdrop*/
