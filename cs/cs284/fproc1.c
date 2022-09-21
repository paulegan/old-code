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

 printf("Please enter the salary value to check the file with: ");
 scanf("%f",&salaryCheck);
 fflush(stdin);
 
 dataFile = fopen ("pers.txt", "r");
 while(!feof(dataFile)){
  fscanf(dataFile,"%d  ",&employee.work_id);
  fgets(employee.name,31,dataFile);
  fgets(employee.address,51,dataFile);
  fgets(employee.birthdate,9,dataFile);
  fscanf(dataFile," %c %d %f",&employee.sex,
  		   	      &employee.department,
  		   	      &employee.salary);
 
  if ((employee.salary >= salaryCheck) && !eof(dataFile)){
    printf("%d  %s%s%s %c %d %f \n",
                   employee.work_id,
                   employee.name,
                   employee.address,
                   employee.birthdate,
                   employee.sex,
                   employee.department,
                   employee.salary);   
    printf("Press any key to continue...");
    getchar();
  }/* if */                   
 }/* while */
 fclose(dataFile);
}/* main */

