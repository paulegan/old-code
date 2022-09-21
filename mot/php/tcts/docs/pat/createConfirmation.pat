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

<FORM ACTION="new.phtml" METHOD="POST">
<INPUT TYPE="HIDDEN" NAME="stage" VALUE="insert">
<INPUT TYPE="HIDDEN" NAME="year" VALUE="#year#">
<INPUT TYPE="HIDDEN" NAME="mth" VALUE="#mth#">
<INPUT TYPE="HIDDEN" NAME="day" VALUE="#day#">
<INPUT TYPE="HIDDEN" NAME="approved_year" VALUE="#approved_year#">
<INPUT TYPE="HIDDEN" NAME="approved_mth" VALUE="#approved_mth#">
<INPUT TYPE="HIDDEN" NAME="approved_day" VALUE="#approved_day#">
<INPUT TYPE="HIDDEN" NAME="effective_year" VALUE="#effective_year#">
<INPUT TYPE="HIDDEN" NAME="effective_mth" VALUE="#effective_mth#">
<INPUT TYPE="HIDDEN" NAME="effective_day" VALUE="#effective_day#">
<INPUT TYPE="HIDDEN" NAME="batchID" VALUE="#batchID#">
<INPUT TYPE="HIDDEN" NAME="adminCID" VALUE="#adminCID#">
<INPUT TYPE="HIDDEN" NAME="numOfLines" VALUE="#numOfLines#">
<INPUT TYPE="HIDDEN" NAME="sending" VALUE="#sending#">
<INPUT TYPE="HIDDEN" NAME="center" VALUE="#center#">

<TABLE WIDTH="550" BORDER=0 CELLSPACING = 0 CELLLPADDING = 0>
<TR><TD colspan = 3><IMG SRC="/images/tcts11.gif"></TD></TR>
<TR><TD> &nbsp; </TD></TR>
<TR bgcolor="#333366"><TD colspan = 3><FONT FACE="Arial, Helvetica" SIZE=4 color="#FFFFFF">Transaction To Insert</TD></TR>

<TR bgcolor = "#ffffcc"><TD colspan = 3>
<TABLE WIDTH = 550>


<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#day#-#mth#-#year#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Approved Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#approved_day#-#approved_mth#-#approved_year#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Effective Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#effective_day#-#effective_mth#-#effective_year#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>HR Administrator:</B> </FONT></TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#adminCID# - #adminName#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Transaction Center:</B> </FONT></TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#centerName#</FONT></TD></TR>

<TR><TD> </TD></TR>

<TR>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Employee CID</B> </TD>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Transaction Type</B> </TD>
</TR><TR>

#confirmLines#

<TR><TD><INPUT TYPE=SUBMIT VALUE=" Confirm Transaction"></TD></TR>
</TABLE>
</TD></TR>


<TR><TD colspan = 5><FONT FACE="Arial, Helvetica" SIZE=3><A HREF="index.phtml">Back To Main</A></FONT></TD></TR>
</TABLE>
</FORM>
#include_footer#

</BODY>
</HTML>

