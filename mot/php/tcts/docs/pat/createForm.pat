<HTML>

<HEAD>
 <TITLE> TCTS | New Transaction</TITLE>
 <META NAME="Author"            CONTENT="joel@hr.emea.mot.com">
 <META NAME="DevDate"           CONTENT="20000914">
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

<script language="JavaScript" type="text/javascript">

<!--
function addExtraLines(){
  val = Number(document.addForm.numOfLines.value);
  extra = Number(document.lineNumberForm.extraLines.value);
  if (extra > 0)
    document.addForm.numOfLines.value = val+extra;
  document.addForm.stage.value="form";
}
//-->
</script>

#include_header#
#include_leftbar#


<TABLE WIDTH="550" CELLPADDING = 0 CELLSPACING = 0>
<TR><TD colspan = 3><IMG SRC="/images/tcts11.gif"></TD></TR>
<TR><TD>&nbsp</TD></TR>
<TR bgcolor="#333366"><TD><FONT FACE="Arial, Helvetica" SIZE=4 color="#FFFFFF"> Add New Transaction</TD></TR>
<TR bgcolor = "#FFFFCC"><TD>

<FORM METHOD=POST ENCTYPE="application/x-www-form-urlencoded" NAME="addForm" ACTION="new.phtml">
<INPUT TYPE=HIDDEN NAME="stage" VALUE="confirm">
<INPUT TYPE=HIDDEN NAME="numOfLines" VALUE="#numOfLines#">

<TABLE WIDTH = 550 border = 0>
<TR>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Date:</B></FONT></TD>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><INPUT TYPE=TEXT NAME="day" VALUE="#day#" SIZE=2>/<INPUT TYPE=TEXT NAME="mth" SIZE=2 VALUE="#mth#">/<INPUT TYPE=TEXT NAME="year" VALUE="#year#" SIZE=4>(dd/mm/yyyy)</FONT></TD>
</TR>

<TR>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Approved Date:</B></FONT></TD>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><INPUT TYPE=TEXT NAME="approved_day" VALUE="#approved_day#" SIZE=2>/<INPUT TYPE=TEXT NAME="approved_mth" SIZE=2 VALUE="#approved_mth#">/<INPUT TYPE=TEXT NAME="approved_year" VALUE="#approved_year#" SIZE=4></FONT></TD>
</TR>

<TR>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Effective Date:</B></FONT></TD>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><INPUT TYPE=TEXT NAME="effective_day" VALUE="#effective_day#" SIZE=2>/<INPUT TYPE=TEXT NAME="effective_mth" SIZE=2 VALUE="#effective_mth#">/<INPUT TYPE=TEXT NAME="effective_year" VALUE="#effective_year#" SIZE=4></FONT></TD>
</TR>

<TR>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Submitted by:</B>&nbsp</FONT></TD>    
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><SELECT NAME="adminCID">
     #select_admin#
    </SELECT></FONT></TD> 
</TR>

<TR>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Transaction Center:</B>&nbsp</FONT></TD>
    <TD>
      <TABLE BORDER=0>
      <TR>
      <TD>
         <FONT FACE="Arial, Helvetica" SIZE=3>
	 <SELECT NAME="center">
         #select_center#
         </SELECT>
	 </FONT>
      </TD>
      <TD ALIGN=RIGHT>
      <INPUT TYPE=BUTTON VALUE="Update" onClick="document.addForm.stage.value='form';addForm.submit()">
      </TD>
      </TR>
      </TABLE>
   </TD>
</TR>

<TR>
   <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Employee CID</B></FONT></TD>
   <TD><FONT FACE="Arial, Helvetica" SIZE=3><B>Transaction Type </B></FONT></TD>
</TR>

#employee_form#

<TR>
    <TD><FONT FACE="Arial, Helvetica" SIZE=3>
        <INPUT TYPE=SUBMIT VALUE="Add Transactions">
	</FONT>
    </TD>
</TR>
</FORM>

<TR ALIGN="right">
    
    <TD COLSPAN = 2><FONT FACE="Arial, Helvetica" SIZE=3>
    <FORM NAME="lineNumberForm">
    <TABLE BORDER=0>
    <TR ALIGN=RIGHT>
    
        <TD ALIGN=RIGHT><FONT FACE="Arial, Helvetica" SIZE=3>
	      <INPUT TYPE="TEXT" NAME="extraLines" VALUE="1" size="2">
	    </FONT>
        </TD> 
	   
        <TD ALIGN=RIGHT><FONT FACE="Arial, Helvetica" SIZE=3>
	    <INPUT TYPE=BUTTON VALUE="Add Lines" onClick="addExtraLines();addForm.submit()">  
	    </FONT>
        </TD>

    </TR>
    </TABLE>
    </FORM>    
    </FONT>    
    </TD>
</TR>	

<TR ALIGN=RIGHT> 
	<TD>
	&nbsp;
	</TD>
   	<TD> 
	#remove_button# 
	</TD>
</TR>
</TABLE>

</TD></TR>
<TR><TD colspan = 5><FONT FACE="Arial, Helvetica" SIZE=3><A HREF="index.phtml">Back To Main</A></FONT></TD></TR>
</TABLE>


#include_footer#


</BODY>
</HTML>
