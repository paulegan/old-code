<HTML>

<HEAD>
 <TITLE> TCTS | List of Transaction</TITLE>
 <META NAME="Author"            CONTENT="atwohi01@email.mot.com">
 <META NAME="DevDate"           CONTENT="19991116">
 <META NAME="TestedWith"        CONTENT="">
 <META NAME="TestDate"          CONTENT="">
 <META NAME="ReviewedBy"        CONTENT="">
 <META NAME="ReviewDate"        CONTENT="">
 <META NAME="ApprovedBy"        CONTENT="">
 <META NAME="ProdDate"          CONTENT="">
 <META NAME="Description"       CONTENT="Page for adding new transactions">
 <META NAME="Keywords"          CONTENT="transactions,tracking">
 <META NAME="Notes"             CONTENT="">
</HEAD>

#include_header#
#include_leftbar#

<TABLE WIDTH="550" BORDER=0 CELLSPACING = 0 CELLLPADDING = 0>
<TR><TD colspan = 3><IMG SRC="/images/tcts11.gif"></TD></TR>
<TR bgcolor="#333366">

<TR><TD COLSPAN="4">
    <FONT FACE="Arial, Helvetica" SIZE=4>Results of Transaction Search</FONT>
    <P><FONT FACE="Arial, Helvetica" SIZE=2>Results #start# to #end# of #total#</TD></TR><TR><TD COLSPAN="4"><FONT FACE="Arial, Helvetica" SIZE=2>Click on a Transaction ID to #option# a transaction.</TD>
</TR>
</TABLE>

<FORM METHOD=POST ACTION="list.phtml">
<INPUT TYPE=HIDDEN NAME="stage" VALUE="search">
<INPUT TYPE=HIDDEN NAME="status" VALUE="#status#">
<INPUT TYPE=HIDDEN NAME="option" VALUE="#option#">
<INPUT TYPE=HIDDEN NAME="adminCID" VALUE="#adminCID#">
<INPUT TYPE=HIDDEN NAME="transID" VALUE="#transID#">
<INPUT TYPE=HIDDEN NAME="empCID" VALUE="#empCID#">
<INPUT TYPE=HIDDEN NAME="day" VALUE="#day#">
<INPUT TYPE=HIDDEN NAME="mth" VALUE="#mth#">
<INPUT TYPE=HIDDEN NAME="year" VALUE="#year#">
<SELECT NAME="center">
#center_select#
</SELECT>
<INPUT TYPE=SUBMIT VALUE="Search">
</FORM>    

<TABLE WIDTH="550" BORDER=0>
<TR bgcolor="#333366">
   <TD><FONT FACE="Arial, Helvetica" SIZE=1 color="#ffffff">Transaction ID</FONT></TD>
   <TD><FONT FACE="Arial, Helvetica" SIZE=1 color="#ffffff">Employee</FONT></TD>
   <TD><FONT FACE="Arial, Helvetica" SIZE=1 color="#ffffff">HR Administrator</FONT></TD>
   <TD><FONT FACE="Arial, Helvetica" SIZE=1 color="#ffffff">Transaction Type</FONT></TD>
   <TD><FONT FACE="Arial, Helvetica"SIZE=1 color="#ffffff">Open Date</FONT></TD>
</TR>

	#results#

</TD></TR>

</TABLE>

<TABLE>
<TR>
  <TD> #go_back# </TD>
  <TD> #go_forward# </TD>
</TR>
</TABLE>

<TABLE>
<TR>
  <TD><A HREF="list.phtml?status=R"> Rejected Transactions </A></TD>
  <TD><A HREF="list.phtml?status=O"> Opened Transactions </A></TD>
  <TD><A HREF="list.phtml?status=C"> Closed Transactions </A></TD>  
</TR>
</TABLE>

<P><A HREF="index.phtml"> Main Page </A></P>

#include_footer#

</BODY>
</HTML>
