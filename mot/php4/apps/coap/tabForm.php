<?php

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

 class TabForm
 {
  var $tabList;
  
  function TabForm($tabList)
  {
   $this->tabList = $tabList;
  }// TabForm


  function print($selectedTab)
  {
   $returnStatus = true;

   if (!$selectedTab)
     $selectedTab = $form['tab']?$form['tab']:$this->tabList[0]->name;

   print("<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>\n\n <TR><TD>\n\n");
   print(" <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0><TR>\n\n");

   for ($a=0; $a<count($this->tabList); $a++ ){

     if ( $this->tabList[$a]->name == $selectedTab ) $sel = $a;

     print(" <TD>\n  <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>\n   <TR>\n");
     print("    <TD COLSPAN=2 ROWSPAN=2 BGCOLOR=#CCCCCC VALIGN=TOP><IMG SRC=\"images/tabLeftCurve.gif\" WIDTH=2 HEIGHT=2></TD>\n");
     print("    <TD BGCOLOR=#EDEDED><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
     print("    <TD COLSPAN=2 ROWSPAN=2 BGCOLOR=#CCCCCC VALIGN=TOP><IMG SRC=\"images/tabRightCurve.gif\" WIDTH=2 HEIGHT=2></TD>\n");
     print("   </TR>\n   <TR>\n");
     print("    <TD BGCOLOR=#CCCCCC><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
     print("   </TR>\n   <TR>\n");
     print("    <TD BGCOLOR=#EDEDED><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
     print("    <TD BGCOLOR=#CCCCCC><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");

     if ( $sel == $a )
       print("    <TD BGCOLOR=#CCCCCC VALIGN=CENTER NOWRAP>&nbsp;<FONT FACE=\"Arial,Helvetica\" COLOR=#000000>".$this->tabList[a].title."</FONT>&nbsp;</TD>\n");
     else
       print("    <TD BGCOLOR=#CCCCCC VALIGN=CENTER NOWRAP>&nbsp;<A HREF=\"".($PHP_SELF."?tab=".$this->tabList[$a]->name)."\"><FONT FACE=\"Arial,Helvetica\" COLOR=#000000>".$this->tabList[$a]->title."</FONT></A>&nbsp;</TD>\n");

     print("    <TD BGCOLOR=#CCCCCC><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
     print("    <TD BGCOLOR=#666666><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
     print("   </TR>\n   <TR>\n");

     if ( sel == a ) {
       print("      <TD BGCOLOR=#EDEDED><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
       print("      <TD COLSPAN=3 BGCOLOR=#CCCCCC><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=3></TD>\n");
       print("      <TD BGCOLOR=#666666><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
     }else{
       print("      <TD COLSPAN=5 BGCOLOR=#EDEDED><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
     }

     print("    </TR>\n   </TABLE>\n  </TD>\n");

   }

   print("  <TD>\n   &nbsp;\n  </TD>\n </TR></TABLE>\n\n </TD></TR><TR><TD>\n");

   print(" <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>\n  <TR>\n");
   print("   <TD BGCOLOR=#EDEDED><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
   print("   <TD BGCOLOR=#CCCCCC ALIGN=CENTER VALIGN=MIDDLE>\n");
   print("     <IMG SRC=\"images/space.gif\" WIDTH=400 HEIGHT=1>\n");

   $returnStatus = $this->tabList[$sel]->writeBody();

   print("   </TD>\n   <TD BGCOLOR=#666666><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD>\n");
   print("  </TR>\n  <TR><TD COLSPAN=3 BGCOLOR=#666666><IMG SRC=\"images/space.gif\" WIDTH=1 HEIGHT=1></TD></TR>\n");
   print(" </TABLE>\n\n </TD></TR>\n\n</TABLE>\n");

   return ($returnStatus);
  }// write

 }
 
?>
