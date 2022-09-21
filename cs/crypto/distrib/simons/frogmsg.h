/*
 * file frogmsg.h
 * author: Simon Foley, Nov 25, 1996. 
 *
 * This interface provides routines for constructing the string based 
 * (and structure based) messages that are used in the first implementation
 * of the wide mouth frog protocol. See trent.c and client.c
 *
 * string messages have the form 
 *     "kind:fromprin:toprin:key"
 * where kind is either FWD or USE. If FWD then the principle receiving the
 * message should forward it to princple "toprin". If USE then the principle
 * receiving the message should use the key supplied as a secret key to 
 * share with "fromprin". 
 * These strings are passed as messages in the protocol. They have 
 * equivalent structures of type Msg, and we can convert from one to 
 * the other using the routines below. 
 */

#define USE "U"
#define FWD "F"
#define DELIMITER ":"               /*used to separate components of a message*/

#define SIZE 20                 /*maximum length of any message **component** */

typedef enum{use,fwd} msg_kind;

typedef struct{                    /*an equivalent struct for a string message*/
    msg_kind kind;                                /*what kind of message is it*/
    char fromprin[SIZE];                                     /*who it is from */
    char toprin[SIZE];                        /*who the (final) destination is*/
    char key[SIZE];                       /*the proposed session key encrypted*/
} msg_rec; 
typedef msg_rec* Msg;    

/*
 * convert a string message to a struct message. The string msg is destroyed
 */
Msg cnv_msg_rec(char *cp);

/*
 * convert a struct message to a string message. The struct msg is destroyed
 */
char *cnv_msg_str(Msg mp);

int is_msg_str(char *mp);

/*
 * Primitive constructor function to build a message struct. 
 */
Msg mk_msg(const msg_kind kind, const char *fromprin, 
           const char *toprin, const char *key);
