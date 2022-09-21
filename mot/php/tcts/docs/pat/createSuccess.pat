<HTML>

<HEAD>
 <TITLE> TCTS | Confirm Transaction</TITLE>
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
<TR><TD> &nbsp; </TD></TR>
<TR bgcolor="#333366"><TD colspan = 3><FONT FACE="Arial, Helvetica" SIZE=4 color="#FFFFFF">Transaction Inserted</TD></TR>

<TR bgcolor = "#ffffcc"><TD colspan = 3>
<TABLE WIDTH = 550>


<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#date#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Approved Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#approved_date#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Effective Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#effective_date#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>HR Administrator:</B> </FONT></TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#adminCID#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Transaction Center:</B> </FONT></TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#center#</FONT></TD></TR>

<TR><TD> </TD></TR>

<TR>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Trans ID</B></TD>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Employee CID</B> </TD>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Transaction Type</B> </TD>
</TR><TR>

#confirmLines#

</TABLE>
</TD></TR>


<TR><TD colspan = 5><FONT FACE="Arial, Helvetica" SIZE=3><A HREF="index.phtml">Back To Main</A></FONT></TD></TR>
</TABLE>

#include_footer#

</BODY>
</HTML>

