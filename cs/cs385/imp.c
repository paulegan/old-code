
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct{
  int emp_id;
  int t;
}keycard;
typedef char* response;
typedef struct{
  int emp_id;
  int hours_worked;
  int start_time;
}employee;
typedef struct{
  int  length;
  employee e[MAX];
}employeeDB;
typedef struct{
  int weekday_times[1];
  int weekend_times[1];
}

response checkID(keycard k);
int inOrOut(keycard k);
response clockIn(keycard k);
response clockOut(keycard k);
response checkTime(keycard k);
response performOperation(keycard k);

void main()
{
 keycard k;
 
 scanf("%d",&(k.emp_id));
 k.t = time(NULL);
 puts(performOperation(k));

}/* main */


response performOperation(keycard k)
{
 response r;

 if ( r = checkID(k) )
  return(r);

 if (inOrOut(k) == IN)
   return(clockIn(k));
 else
   return(clockOut(k));

}/* performOperation */


response checkID(keycard k)
{
 response r = NULL;

 if (employeeDB(k.emp_id)){
   r = malloc(sizof("id_unknown"));
   strcpy(r,"id_unknown");
 }

 return r;
}/* checkID */

int inOrOut(keycard k)
{
 employee e = employeeDB(k.emp_id);
 return(e->start_time);
}/* inOrOut */

response clockIn(keycard k)
{
 employee e = employeeDB(k.emp_id);
 
 if (r = checkTime(k))
  return(r);

 e.startTime = k.time;

 return("in");
}/* clockIn */


response clockOut(keycard k)
{
 flexi-time ft = flexi-times(k.time);
 employee e = employeeDB(k.emp_id);

 if ( k.time > ft[0] && k.time < ft[1] )
   finish_time = ft[1];
 else
   finish_time = k.time;
 
 e.hours_worked+=(finish_time-e.start_time);
 return("Out",e.hours_worked);
}/* clockOut */


response checkTime(keycard k)
{
 flexi-time ft = flexi-times(k.time);
 employee e = employeeDB(k.emp_id);

 if ( k.time > ft[0] && k.time < ft[1] )
   return("Invalid time",e.hours_worked);
 else
   return(NULL);
}/* checkTime */
