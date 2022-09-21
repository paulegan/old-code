

/* clientLib.c			lumpy@cs.ucc.ie			14.8.1996	*/


#include <config.h>
#include <des.h>
#include <rsa.h>

Socket *mySocket;
Socket *errorSkt;

int getResult()
{
 int  result;
 
 if (Stimeoutwait(errorSkt,5,0) < 0){
   error(ERROR,"No response from server! \n");
   Sclose(errorSkt);
 }
 
 Sscanf(errorSkt,"%d",&result);
 
 return(result);
}/* getResult */


void startSession(char *name, char *server)
{
 if (!(mySocket = Sopen(server,"c")))
   error(ERROR,"Unable to connect to <%s>!\n",server);
 if (!(errorSkt = Sopen(server,"c")))
   error(ERROR,"Unable to connect error socket to <%s>!\n",server);
 
 Sputs(name,mySocket);
 
 if (getResult() == AlreadyRegistered)
   error(ERROR,"%s already in use!\n",name);
   
}/* register */


int endSession()
{
 int  result;
 
 Sprintf(mySocket,"%d",EndSession);
 result = getResult();
 Sclose(mySocket);
 Sclose(errorSkt);

 return(result);
}/* endSession */


int sendMessageTo(char *principal, char *message)
{
 Sprintf(mySocket,"%d",SendMessage);
 Sputs(principal,mySocket);
 Sprintf(mySocket,"%d",strlen(message)+3);
 Sputs(message,mySocket);
 
 return(getResult());
}/* sendMessage */


char *receiveMessage()
{
 int   messageSize;
 char *returnedMessage;
  
 Sscanf(mySocket,"%d",&messageSize);
 returnedMessage = (char*)memalloc(messageSize);
 Sgets(returnedMessage,messageSize,mySocket);
  
 return(returnedMessage);
}/* receiveMessage */


int interceptFrom(char *principal)
{
 Sprintf(mySocket,"%d",InterceptFrom);
 Sputs(principal,mySocket);
 
 return(getResult());
}/* interceptFrom */


int interceptTo(char *principal)
{
 Sprintf(mySocket,"%d",InterceptTo);
 Sputs(principal,mySocket);
 
 return(getResult());
}/* interceptTo */


int listenFrom(char *principal)
{
 Sprintf(mySocket,"%d",ListenFrom);
 Sputs(principal,mySocket);
 
 return(getResult());
}/* listenFrom */


int listenTo(char *principal)
{
 Sprintf(mySocket,"%d",ListenTo);
 Sputs(principal,mySocket);
 
 return(getResult());
}/* listenTo */


int newMessage()
{
 return(Stest(mySocket));
}/* newMessage */


char *newTimeStamp()
{
 char *timeStr = (char*)memalloc(TimeStampSize);
 
 /*  To get time from server:
  Sprintf(mySocket,"%d",NewTimeStamp);
  Sgets(timeStr,TimeStampSize,mySocket);
  getResult();
 */
 
 sprintf(timeStr,"%d",time(NULL));
 
 return(timeStr);
}/* newTimeStamp */


int validTimeStamp(char *ts, long range)
{
 int result;
 
 /* To check time stamp according to server's time:
  Sprintf(mySocket,"%d",ValidTimeStamp);
  Sprintf(mySocket,"%s %d",ts,range);
  Sscanf(mySocket,"%d",&result);
 */
 
 result = (abs(time(NULL)-atol(ts))<range);

 return(result);
}/* validTimeStamp */


