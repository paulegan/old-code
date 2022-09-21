<HTML>

<HEAD>
 <TITLE> Transaction Centre Tracking System </TITLE>
 <META NAME="Author"            CONTENT="atwohi01@email.mot.com">
 <META NAME="DevDate"           CONTENT="19991116">
 <META NAME="TestedWith"        CONTENT="">
 <META NAME="TestDate"          CONTENT="">
 <META NAME="ReviewedBy"        CONTENT="">
 <META NAME="ReviewDate"        CONTENT="">
 <META NAME="ApprovedBy"        CONTENT="">
 <META NAME="ProdDate"          CONTENT="">
 <META NAME="Description"       CONTENT="Tool to track  HR transactions">
 <META NAME="Keywords"          CONTENT="transactions,tracking">
 <META NAME="Notes"             CONTENT="">
</HEAD>

#include_header#
#include_leftbar#

<TABLE WIDTH="550" BORDER=0>
<TR><TD colspan = 3><IMG SRC="/images/tcts11.gif"></TD></TR>
<TR><TD colspan = 4><FONT FACE="Arial, Helvetica" SIZE=4 COLOR="#333366"><BR>Welcome</FONT></TD></TR>
<TR><TD colspan = 4><FONT FACE="Arial, Helvetica" SIZE=2>Welcome to the Transaction Centre Tracking System - Version 2.2<BR></FONT></TD></TR> 
<TR><TD valign = top align = left>
    <TABLE WIDTH="550" BORDER=0 CELLPADDING=4 CELLSPACING=0 BGCOLOR="#FFFFCC">
    <TR><TD BGCOLOR="#333366"><FONT FACE="Arial, Helvetica" SIZE=3 COLOR="#FFFFFF"><B>Options</B></FONT></TD></TR>
    <TR><TD><FONT FACE="Arial, Helvetica" SIZE=3>

<!-- #tcts_start# --  
        <A HREF="new.phtml">Create new transactions</A><BR>
        <A HREF="update.phtml?option=reject">Reject a transaction<A><BR>
        <A HREF="update.phtml?option=close">Close a transaction<A><BR>
	<A HREF="update.phtml?option=reopen">Reopen a transaction<A><BR>
  -- #tcts_end# -->

<!-- #admin_start# --  
	<A HREF="batch.phtml">Create a batch of transactions</A><BR>
  -- #admin_end# -->

<!-- #tcts_start# --  
        <A HREF="move.phtml">Integrate a batch into the TCTS</A><BR>
  -- #tcts_end# -->

<!-- #su_start# --  
	<A HREF="update.phtml?option=delete">Delete a transaction<A><BR>
  -- #su_end# -->

<!-- #su_admin_start# --  
        <A HREF="list.phtml">View transactions</A><BR>
  -- #su_admin_end# -->

    </FONT></TD></TR>
    </TABLE>
    </TD>

</TR>
<TR><TD>&nbsp;</TD></TR>
</TABLE>

 #include_footer#

</BODY>
</HTML>
