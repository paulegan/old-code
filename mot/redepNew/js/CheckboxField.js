
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19971122
//  TestedWith:		ES 351
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        CheckboxField is a form object with a link/checkbox element for each item in the table.  Also includes the job count.
//  Keywords:            
//  Notes:		


function CheckboxField(tableName)
{
 this.tableName = tableName;
 this.columnName = tableName;
 CheckboxField.prototype.write = CheckboxField_write;
 CheckboxField.prototype.selected = CheckboxField_selected;
}// CheckboxField

function CheckboxField_write(numCols)
{
 var item, numEntriesPerCol;
 var colWidthStr = "WIDTH=\""+Math.ceil(100/numCols)+"%\"";
 var i=0;

 if (!(item = database.superCursor("SELECT name, NVL(count,0) AS count FROM "+this.tableName+", ( SELECT "+this.tableName+", count("+this.tableName+") AS count FROM jobs WHERE sysDate BETWEEN advertise_date AND expiry_date+1 GROUP BY "+this.tableName+" ) WHERE name = "+this.tableName+"(+)")))
   return(false);
 numEntriesPerCol = (item.length/numCols);

 write("<TABLE BORDER=0 WIDTH=100%><TR>\n");
 for(var col=0; col<numCols; col++){
   write("<TD "+colWidthStr+" VALIGN=TOP>\n");
   for(var ci=(numEntriesPerCol*col); ci<(numEntriesPerCol*(col+1)); ci++){
     if (!item.next()) break;
     write("<INPUT TYPE=CHECKBOX NAME="+this.tableName+(i++)+" VALUE=\""+item.NAME+"\">&nbsp;");
     write("<A HREF=\"results.html?"+this.tableName+"="+escape(item.NAME)+'"');
     write(" onMouseOver=\"window.status='Show all positions in "+item.NAME+"';return true;\" onMouseOut=\"window.status='';return true;\" ");
     write(">"+item.NAME.replace(/ /g,"&nbsp;")+"</A>&nbsp;<FONT SIZE=-1><I>"+item.COUNT+"</I></FONT><BR>\n");
   }
   write("</TD>\n");
 }
 write("</TR></TABLE>\n");

 item.close();

 return true;
}// CheckboxField_write


function CheckboxField_selected()
{
 var colMatch = new RegExp("^"+this.tableName+"[0-9]*$","i");
 var selection = new Array();

 for ( prop in request )
   if (prop.match(colMatch))
     selection.push(request[prop]);

 return(selection);
}// CheckboxField_selected

