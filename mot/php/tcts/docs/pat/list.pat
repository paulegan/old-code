<HTML>

<HEAD>
 <TITLE> TCTS | Transaction Details</TITLE>
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
<TD colspan = 3><FONT FACE="Arial, Helvetica" SIZE=4 color="#FFFFFF">Transaction Details</TD></TR>

<TR bgcolor = "#ffffcc"><TD>
    <TABLE WIDTH = 550>

    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Transaction ID: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#trans_id#</TD>
    </TR>
    
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Employee: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#emp_cid#</TD>
    </TR>
    
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>HR Administrator: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#hr_admin#</TD>
    </TR>
    
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Type: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#trans_type#</TD>
    </TR>
 
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Center: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#trans_center#</TD>
    </TR>
   
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Date Opened: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#trans_date#</TD>
    </TR>
    
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Date Approved: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#approved_date#</TD>
    </TR>
    
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Date Effective: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#effective_date#</TD>
    </TR>
    
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Status: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#status#</TD>
    </TR>
    
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Rejection Reason: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#reject_reason#</TD>
    </TR>
    
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Closed By: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#closed_by#</TD>
    </TR>
    
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>Date Closed: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>#close_date#</TD>
    </TR>    
    </TABLE>
</TD></TR>
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
