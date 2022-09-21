
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19990419
//  TestedWith:		ES 351
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        SelectList is for use with selection lists.
//  Keywords:            
//  Notes:		


function SelectList(tableName)
{
 this.tableName = tableName;
 SelectList.prototype.write = SelectList_write;
 SelectList.prototype.selected = SelectList_selected;
}// SelectList


function SelectList_write(def)
{
 var item;

 if (!(item = database.superCursor("SELECT name FROM "+this.tableName+" ORDER BY name"))){
   debug("Nothing returned by list select! - "+database.errorMessage);
   return(false);
 }

 write("<SELECT NAME=\""+this.tableName+"\">\n");
 while (item.next())
   write("<OPTION VALUE=\""+item.NAME+"\""+(item.NAME==def?" SELECTED":"")+"> "+item.NAME+"\n");
 write("</SELECT>\n");
 item.close();

 return (true);
}// SelectList_write


function SelectList_selected()
{
 var selection = new Array();

 for (var i=0; i<getOptionValueCount(this.tableName); i++)
   selection.push(getOptionValue(this.tableName,i));

 return (selection);
}// SelectList_selected

