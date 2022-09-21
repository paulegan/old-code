
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19990903
//  TestedWith:		ES 362
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        
//  Keywords:            
//  Notes:		


function Tab(name,title,acronym,writeBody)
{
 this.name = name || "dummy";
 this.title = title || "Dummy";
 this.writeBody = writeBody || TabForm_dummyBody;
 this.acronym = acronym || "D";
}// Tab

function TabForm(tabList)
{

 // Properties
 this.tabList = tabList || [ new Tab() ];

 // Methods
 this.write = TabForm_write;

}// TabForm


function TabForm_write(selectedTab)
{
 var sel;
 var returnStatus = true;

 if (!selectedTab)
   selectedTab = request.tab?request.tab:this.tabList[0].name;

 write("<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>\n\n <TR><TD>\n\n");
 write(" <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0><TR>\n\n");

 for (var a=0; a<this.tabList.length; a++ ){

   if ( this.tabList[a].name == selectedTab ) sel = a;

   write(" <TD>\n  <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>\n   <TR>\n");
   write("    <TD COLSPAN=2 ROWSPAN=2 BGCOLOR=#CCCCCC VALIGN=TOP><IMG SRC=\"images/tabLeftCurve.gif\" WIDTH=2 HEIGHT=2></TD>\n");
   write("    <TD BGCOLOR=#EDEDED><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
   write("    <TD COLSPAN=2 ROWSPAN=2 BGCOLOR=#CCCCCC VALIGN=TOP><IMG SRC=\"images/tabRightCurve.gif\" WIDTH=2 HEIGHT=2></TD>\n");
   write("   </TR>\n   <TR>\n");
   write("    <TD BGCOLOR=#CCCCCC><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
   write("   </TR>\n   <TR>\n");
   write("    <TD BGCOLOR=#EDEDED><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
   write("    <TD BGCOLOR=#CCCCCC><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");

   if ( sel == a )
     write("    <TD BGCOLOR=#CCCCCC VALIGN=CENTER NOWRAP>&nbsp;<FONT FACE=\"Arial,Helvetica\" COLOR=#000000>"+this.tabList[a].title+"</FONT>&nbsp;</TD>\n");
   else
     write("    <TD BGCOLOR=#CCCCCC VALIGN=CENTER NOWRAP>&nbsp;<A HREF=\""+(request.uri.replace(/^.*\//,"")+"?tab="+this.tabList[a].name)+"\"><FONT FACE=\"Arial,Helvetica\" COLOR=#000000>"+this.tabList[a].title+"</FONT></A>&nbsp;</TD>\n");

   write("    <TD BGCOLOR=#CCCCCC><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
   write("    <TD BGCOLOR=#666666><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
   write("   </TR>\n   <TR>\n");
 
   if ( sel == a ) {
     write("      <TD BGCOLOR=#EDEDED><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
     write("      <TD COLSPAN=3 BGCOLOR=#CCCCCC><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=3></TD>\n");
     write("      <TD BGCOLOR=#666666><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
   }else{
     write("      <TD COLSPAN=5 BGCOLOR=#EDEDED><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
   }

   write("    </TR>\n   </TABLE>\n  </TD>\n");

 }

 write("  <TD>\n   &nbsp;\n  </TD>\n </TR></TABLE>\n\n </TD></TR><TR><TD>\n");

 write(" <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>\n  <TR>\n");
 write("   <TD BGCOLOR=#EDEDED><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
 write("   <TD BGCOLOR=#CCCCCC ALIGN=CENTER VALIGN=MIDDLE>\n");
 write("     <IMG SRC=\"images/space.gif\" WIDTH=400 HEIGHT=1>\n");

 returnStatus = this.tabList[sel].writeBody();

 write("   </TD>\n   <TD BGCOLOR=#666666><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
 write("  </TR>\n  <TR><TD COLSPAN=3 BGCOLOR=#666666><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD></TR>\n");
 write(" </TABLE>\n\n </TD></TR>\n\n</TABLE>\n");

 return (returnStatus);
}// TabForm_write


function TabForm_dummyBody()
{
 write("<P> Please define list of tabs! </P>\n<BR CLEAR=ALL>\n");

 return (true);
}// TabForm_dummyBody

