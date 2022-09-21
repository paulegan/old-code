
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19971130
//  TestedWith:		ES 351
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        AreaMap is a form object with a element for each item in the table that matches the given criteria.  Used for descending a hierarchical structure.
//  Keywords:            
//  Notes:		


function AreaMap(tableName)
{
 this.tableName = tableName;
 this.columnName = tableName;
 AreaMap.prototype.write = AreaMap_write;
 AreaMap.prototype.selected = AreaMap_selected;
}// AreaMap

function AreaMap_write(target,areaInfo)
{
 var numCols=3, item, numEntriesPerCol;
 var colWidthStr = "WIDTH=\""+Math.ceil(100/numCols)+"%\"";
 var area;

 if (areaInfo && areaInfo!=""){
   areaInfo = areaInfo.split("|");
   area = areaInfo[0]+" = '"+areaInfo[1]+"'";
 }

 if (!(item = database.superCursor("SELECT name FROM "+this.tableName+" "+(areaInfo?(" WHERE "+area):"")+" ORDER BY name")))
   return(false);
 numEntriesPerCol = (item.length/numCols);

 write("<TABLE BORDER=0 WIDTH=100% CELLSPACING=0 CELLPADDING=10><TR>\n");
 for(var col=0; col<numCols; col++){
   write("<TD "+colWidthStr+" VALIGN=TOP>\n");
   for(var ci=(numEntriesPerCol*col); ci<(numEntriesPerCol*(col+1)); ci++){
     if (!item.next()) break;
     var dest = this.columnName+"|"+item.NAME.replace(/'/g,"");
     write("<A HREF=\""+target+escape(dest).replace(/,/g,"%2C")+"\"");
     write(" onClick=\"return descendTo('"+dest+"');\" ");
     write(" onMouseOver=\"window.status='Descend to "+item.NAME.replace(/['"]/g,"")+"';return true;\"");
     write(" onMouseOut=\"window.status='';return true;\" ");
     write(">"+item.NAME.replace(/ /g,"&nbsp;")+"</A><BR>\n");
   }
   write("</TD>\n");
 }
 write("</TR></TABLE>\n");

 item.close();

 return true;
}// AreaMap_write


function AreaMap_selected()
{
 return([request[this.columnName]]);
}// AreaMap_selected

