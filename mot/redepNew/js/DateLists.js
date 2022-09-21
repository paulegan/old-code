
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19990419
//  TestedWith:		ES 351
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        DateLists is for use with date selection lists.
//  Keywords:            
//  Notes:		


function DateLists(name)
{
 this.name = name;
 DateLists.prototype.write = DateLists_write;
 DateLists.prototype.selected = DateLists_selected;
}// DateLists


function DateLists_write(def)
{
 var months = "Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec";
 var years = "1997,1998,1999,2000,2001,2002";

 if (!def) def = new Date();

 write("<SELECT NAME=\""+this.name+"Date\">\n");
 for(var date=1; date<32; date++)
   write("<OPTION VALUE=\""+date+'"'+(date==def.getDate()?" SELECTED":"")+" > "+date+"\n"); 
 write("</SELECT>\n");

 var month = months.split(",");
 write("<SELECT NAME=\""+this.name+"Month\">\n");
 for(var mon=0; mon<(month.length); mon++)
   write("<OPTION VALUE=\""+mon+'"'+(mon==def.getMonth()?" SELECTED":"")+" > "+month[mon]+"\n"); 
 write("</SELECT>\n");

 var year = years.split(",");
 write("<SELECT NAME=\""+this.name+"Year\">\n");
 for(var yr=0; yr<(year.length); yr++)
   write("<OPTION VALUE=\""+year[yr]+'"'+(year[yr]==((def.getYear()<100)?(1900+def.getYear()):(def.getYear()))?" SELECTED":"")+" > "+year[yr]+"\n"); 
 write("</SELECT>\n");

 return (true);
}// DateLists_write


function DateLists_selected()
{
 return (new Date(getOptionValue(this.name+"Year",0),getOptionValue(this.name+"Month",0),getOptionValue(this.name+"Date",0)));
}// DateLists_selected

