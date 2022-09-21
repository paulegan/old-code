/*  Program to Construct Text and Binary Data Files  */

#include <stdio.h>
#include <stdlib.h>
#define MAXRECS  20
main()
{

FILE *fpt, *fpb ;  /*  Text and Binary File Pointers  */
int  i, writecount ;

struct  emp_rec    /*  Record Structure of Both Files  */ 
{
    int  work_id ;
    char  name[31] ;
    char  address[51] ;
    char  birthdate[9] ;
    char  sex ;
    int  department ;
    float  salary  ;
} ;
struct  emp_rec  employee ;
char  int_string[4], float_string[10], char_string[2];

fpt = fopen ("pers.txt", "w") ;
fpb = fopen ("pers.dat", "wb") ;


for ( i = 1 ; i <= MAXRECS ; i++ )
{

  /*  Prompt for data entry on each field
      Note that all data is entered into string variables regardless of
      its expected type. If a numeric data type is expected (integer or
      float), then a conversion function (atoi or atof) is used to
      convert the string to the appropriate type. This arises due to
      problems in handling the newline (\n) character in C.
      For a similar reason, the string fields of the personnel records
      have been extended by one character in the struct definition above.
      The values therein have been written out using the correct lengths,
      however.                                                            */

   printf ( "\n\nEnter Data for Record Number %d\n", i );
   printf ( "\nWork Id:  " ) ;
   gets ( int_string ) ;
   employee.work_id = atoi (int_string) ;
   printf ( "\nName:  " ) ;
   gets ( employee.name ) ;
   printf ( "\nAddress:  " ) ;
   gets ( employee.address ) ;
   printf ( "\nBirthdate:  " ) ;
   gets ( employee.birthdate ) ;
   printf ( "\nSex:  " ) ;
   gets ( char_string );
   employee.sex = char_string[0] ;
   printf ( "\nDepartment:  " ) ;
   gets ( int_string ) ;
   employee.department = atoi (int_string) ;
   printf ( "\nSalary:  " ) ;
   gets ( float_string ) ;
   employee.salary = atof (float_string) ;

   fprintf ( fpt, "%d  %-30s%-50s%-8s %c %d %f\n", 
                   employee.work_id,
                   employee.name,
                   employee.address,
                   employee.birthdate,
                   employee.sex,
                   employee.department,
                   employee.salary  ) ;

   writecount = fwrite ( &employee, sizeof (struct emp_rec), 
                                              1, fpb ) ;
}

fclose (fpb) ;
fclose (fpt) ;
}
