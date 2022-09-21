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
 
 dataFile = fopen ("pers.dat", "rb");
 while(!feof(dataFile)){
  fread(&employee, sizeof(struct emp_rec),1,dataFile);

  if ((employee.salary >= salaryCheck) && !feof(dataFile)){
    printf("%d  %-30s%-50s%-8s %c %d %f\n",
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

