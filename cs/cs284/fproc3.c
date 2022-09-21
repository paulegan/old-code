#include <stdio.h>

void main()
{
 struct emp_rec{
    int   work_id;
    char  name[31];
    char  address[51];
    char  birthdate[9];
    char  sex;
    int   department;
    float salary;
 }employee;
 float salaryCheck;
 FILE *dataFile;
 char  int_string[4], float_string[10], char_string[2];

 printf("\nEnter Data for New Employee\n");
 printf("Work Id: ");
  gets(int_string) ;
  employee.work_id = atoi(int_string);
 printf("Name: ");
  gets(employee.name);
 printf("Address: ");
  gets(employee.address);
 printf("Birthdate: ");
  gets(employee.birthdate);
 printf("Sex: ");
  gets(char_string);
  employee.sex = char_string;
 printf("Department: ");
  gets(int_string);
  employee.department = atoi(int_string);
 printf("Salary: ");
  gets(float_string);
  employee.salary = atof(float_string);

 dataFile = fopen("pers.dat", "ab");
  fwrite(&employee,sizeof (struct emp_rec),1,dataFile);
 fclose(dataFile);

}/* main */
