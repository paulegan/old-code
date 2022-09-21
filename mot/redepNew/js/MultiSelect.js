
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19971130
//  TestedWith:		ES 351
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        MultiSelect is for use with multiple selection lists.
//  Keywords:            
//  Notes:		


function MultiSelect(tableName)
{
 this.tableName = tableName;
 this.columnName = tableName;
 this.formName = tableName+"MS";
 MultiSelect.prototype.write = MultiSelect_write;
 MultiSelect.prototype.selected = MultiSelect_selected;
 MultiSelect.prototype.dumpSelection = MultiSelect_dumpSelection;
 MultiSelect.prototype.readSelection = MultiSelect_readSelection;
}// MultiSelect


function MultiSelect_write(length)
{
 var item;
 var sel = ","+this.readSelection()+",";

 if (!(item = database.superCursor("SELECT name FROM "+this.tableName+" ORDER BY name")))
   return(false);

 write("<SELECT MULTIPLE SIZE="+length+" NAME=\""+this.formName+"\">\n");
 while(item.next()){
   var re = new RegExp(","+item.NAME+",","");
   write("<OPTION VALUE=\""+item.NAME+"\""+(sel.match(re)?" SELECTED":"")+"> "+item.NAME+"\n");
 }
 write("</SELECT>\n");
 item.close();

 return (true);
}// MultiSelect_write


function MultiSelect_selected()
{
 var selection = new Array();
 var dump;

 for (var i=0; i<getOptionValueCount(this.formName); i++)
   selection.push(getOptionValue(this.formName,i));

 if ((dump=this.readSelection())!=null)
   selection = selection.concat(dump.split(","));

 return (selection);
}// MultiSelect_selected


function MultiSelect_dumpSelection()
{
 var s = this.selected();

 if (s.length>0)
   write("<INPUT TYPE=HIDDEN NAME=\""+this.formName+"Dump\" VALUE=\""+s.join(",")+"\">\n");

 return (true);
}// 


function MultiSelect_readSelection()
{
 if (request[this.formName+"Dump"])
   return (request[this.formName+"Dump"]);
 else
   return (null);
}// MultiSelect_dumpSelection
