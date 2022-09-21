<HTML>

<HEAD>
 <TITLE> TCTS | Update Transaction</TITLE>
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

<FORM METHOD=POST ENCTYPE="application/x-www-form-urlencoded" NAME="nameForm" ACTION="update.phtml">
<INPUT TYPE = HIDDEN NAME="option" VALUE="#option#">
<INPUT TYPE = HIDDEN NAME="stage" VALUE="search">

<TABLE WIDTH="550" BORDER=0>
<TR><TD colspan = 3><IMG SRC="/images/tcts11.gif"></TD></TR>
<TR><TD colspan = 3><FONT FACE="Arial, Helvetica" SIZE=4 COLOR="#333366">
    Choose a Transaction to #option#
</FONT></TD></TR>
<TR><TD colspan = 3><FONT FACE="Arial, Helvetica" SIZE=3>Search for Transactions using one of the fields below.
<TR><TD valign = top align = left>
    <TABLE WIDTH="550" BORDER=0 CELLPADDING=4 CELLSPACING=0 BGCOLOR="#FFFFCC">
    
    <TR>
       <TD BGCOLOR="#333366" colspan = 2>
       <FONT FACE="Arial, Helvetica" SIZE=3 COLOR="#FFFFFF">
       <B>Search Fields</B></FONT>
       </TD>
    </TR>
               
    <TR>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>Transaction ID: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>
	<INPUT TYPE=TEXT NAME="transID" SIZE=20 VALUE="">
	</TD>
    </TR>
    
    <TR>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>HR Administrator: </TD>
	<TD><FONT FACE="Arial, Helvetica" SIZE=3>
	<SELECT NAME="adminCID"><OPTION value=></OPTION>
	#adminSelect#
	</SELECT></TD>
    </TR>
    
    <TR>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>Employee: </TD>
	<TD><FONT FACE="Arial, Helvetica" SIZE=3>
	<SELECT NAME="empCID" LENGTH=8><OPTION value=></OPTION>
	#empSelect#</SELECT>
	</TD>
    </TR>
    
    <TR>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>Center: </TD>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>
        <SELECT NAME="center" LENGTH=8><OPTION value=></OPTION>
        #centerSelect#</SELECT>
        </TD>
    </TR>
   
    <TR>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>Opened after: </TD>
	<TD><FONT FACE="Arial, Helvetica" SIZE=3>
	<INPUT TYPE=TEXT NAME="day" SIZE=2>/
	<INPUT TYPE=TEXT NAME="mth" SIZE=2>/
	<INPUT TYPE=TEXT NAME="year" SIZE=4> (dd/mm/yyyy)
	</TD>
    </TR>
    
    <TR>
        <TD><FONT FACE="Arial, Helvetica" SIZE=3>
	<INPUT TYPE=SUBMIT VALUE="Search">
	</TD>
	
        <TD>&nbsp;</TD>
    </TR>

    </TABLE>
    </TD>

</TR>
<TR><TD> &nbsp; </TD></TR>
</TABLE>
    
</FORM>
        
<P><A HREF="index.phtml"> Main Page </A></P>

#include_footer#;


</BODY>
</HTML>
