<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">

<HTML>

<HEAD>
 <TITLE>Group Member List</TITLE>
 <META NAME="Author"		CONTENT="paul@hr.emea.mot.com">
 <META NAME="DevDate"           CONTENT="19990202">
 <META NAME="TestedWith"        CONTENT="">
 <META NAME="TestDate"          CONTENT="">
 <META NAME="ReviewedBy"        CONTENT="">
 <META NAME="ReviewDate"        CONTENT="">
 <META NAME="ApprovedBy"        CONTENT="">
 <META NAME="ProdDate"          CONTENT="">
 <META NAME="History"		CONTENT="">
 <META NAME="Description"       CONTENT="Page to display the members of a group.">
 <META NAME="Keywords"          CONTENT="group,member,list,display">
 <META NAME="Notes"             CONTENT=""> 
</HEAD>

<BODY BGCOLOR="#FFFFFF" TEXT="#000000" LINK="#0000FF" VLINK="#000077">

  <TABLE BORDER=0 WIDTH=400>
   <TR><TD COLSPAN=2 ALIGN=LEFT><FONT FACE="Arial, Helvetica" size="4">Document Repository Group</FONT><P></TD></TR>
   <TR><TD COLSPAN=2 ALIGN=LEFT><FONT FACE="Arial, Helvetica" size="2"><IMG SRC="/images/group.gif" ALT="Show Group Members" BORDER=0 WIDTH=32 HEIGHT=32>&nbsp;<b>Group:</b>&nbsp;#NAME#</FONT></TD></TR>
   #LIST#
  </TABLE>
  
<FORM><INPUT TYPE=BUTTON VALUE=" Back " onClick="history.go(-1);">&nbsp;<INPUT TYPE=BUTTON VALUE="  Cancel  " onClick="#CANCEL_CODE#"></FORM>

</BODY>
</HTML>
