<HTML>

<HEAD>
 <TITLE> TCTS | Batch Details</TITLE>
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

<TR bgcolor="#333366"><TD colspan = 3><FONT FACE="Arial, Helvetica" SIZE=4 color="#FFFFFF">Batch Details</TD></TR>


<TR bgcolor = "#ffffcc"><TD colspan = 3>
<TABLE WIDTH = 550 cols=3>

<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Batch ID :</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#batch_id#</FONT></TD><TD>&nbsp;</TD></TR>

<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#date#</FONT></TD><TD>&nbsp;</TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Approved Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#approved_date#</FONT></TD><TD>&nbsp;</TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Effective Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#effective_date#</FONT></TD><TD>&nbsp;</TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>HR Administrator:</B> </FONT></TD><TD colspan=2><FONT FACE="Arial, Helvetica" SIZE=3>#adminCID#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Transaction Center:</B> </FONT></TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#center#</FONT></TD></TR>

<TR><TD> </TD></TR>

<TR>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Trans ID</B></TD>
    <TD colspan="2"><FONT FACE="Arial, Helvetica" SIZE=3><B>Employee CID</B> </TD>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Transaction Type</B> </TD>
</TR><TR>

#confirmLines#

</TABLE>
</TD></TR>

</TABLE>


<P>
    <FORM METHOD=POST ENCTYPE="application/x-www-form-urlencoded" ACTION="move.phtml">
    <INPUT TYPE = HIDDEN NAME ="batchID" VALUE="#batch_id#">
    <INPUT TYPE = HIDDEN NAME ="center" VALUE="#center_code#">
    <INPUT TYPE = HIDDEN NAME ="stage" VALUE="move">    
    <FONT FACE="Arial, Helvetica" SIZE=3><INPUT TYPE=SUBMIT VALUE="Integrate Batch"></FONT>
    </FORM>
</P>

<P>
    <FORM METHOD=POST ENCTYPE="application/x-www-form-urlencoded" ACTION="move.phtml">
    <INPUT TYPE = HIDDEN NAME ="batchID" VALUE="#batch_id#">
    <INPUT TYPE = HIDDEN NAME ="center" VALUE="#center_code#">
    <INPUT TYPE = HIDDEN NAME ="stage" VALUE="reject">    
    <FONT FACE="Arial, Helvetica" SIZE=3>Rejection Reason: <BR>
    <FONT FACE="Arial, Helvetica" SIZE=3><SELECT NAME="reason">
            <OPTION VALUE=""></OPTION>
            #reject_reasons#
            </SELECT>
</P>
<P><FONT FACE="Arial, Helvetica" SIZE=3><INPUT TYPE=SUBMIT VALUE="Reject Batch"></FONT></P>    
    </FORM>

<P><A HREF="index.phtml"> Main Page </A></P>

#include_footer#

</BODY>
</HTML>

