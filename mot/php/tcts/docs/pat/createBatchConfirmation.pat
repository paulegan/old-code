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

<BODY>

<script language=JavaScript>
<!--
if (window.print) 
  document.write('<P ALIGN=CENTER><A align="left" HREF="javascript:;" onClick="self.print()"><font face="Arial" size =1>Print this page</font></A></P>');
else if (navigator.appName.indexOf('Microsoft') !=-1)
  document.write('<P ALIGN=CENTER><A HREF="javascript:;" onClick="VBPrint()"><font face="Arial" size =1>Print this page</font></A></P>');
// -->
</script>
<script language=VBScript>
<!-- //
	Sub VBPrint()
  	On Error Resume Next
  	WBControl.ExecWB 6,1
	End Sub
// -->
</script>


<TABLE WIDTH="550" BORDER=0 CELLSPACING = 0 CELLLPADDING = 0>
<TR bgcolor="#333366"><TD colspan = 3><FONT FACE="Arial, Helvetica" SIZE=4 color="#FFFFFF">Transaction Inserted</TD></TR>

<TR bgcolor = "#ffffcc"><TD colspan = 3>
<TABLE WIDTH ="550" cols="4">

<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Batch ID :</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#batch_id#</FONT></TD></TR>

<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#date#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Approved Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#approved_date#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Effective Date:</B> </TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#effective_date#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>HR Administrator:</B> </FONT></TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#adminCID#</FONT></TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Sent Via:</B></FONT></TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#sending#</TD></TR>
<TR><TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Transaction Center:</B> </FONT></TD><TD><FONT FACE="Arial, Helvetica" SIZE=3>#center#</FONT></TD></TR>


<TR><TD> </TD></TR>

<TR>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Trans ID</B></TD>
    <TD colspan=2><FONT FACE="Arial, Helvetica" SIZE=3><B>Employee CID</B> </TD>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Transaction Type</B> </TD>
</TR><TR>

#confirmLines#

<TR><TD> &nbsp; </TD><TD> &nbsp; </TD></TR>

<TR><TD> <FONT FACE="Arial, Helvetica" SIZE=3><B>Number of Pages Sent:</B> </FONT></TD><TD><FONT FACE="Arial, Helvetica" SIZE=3> <U>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </U></FONT>  </TD></TR>

</TABLE>
</TD></TR>

</TABLE>

</BODY>
</HTML>

