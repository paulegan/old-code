<SCRIPT LANGUAGE="JavaScript">
  <!--
  window.name = "docRepMain";  // Used by upload window to return!

  function getDirName(form)
  {
   var dir = prompt("Enter name of directory to create:","");
   if ( dir==null )
     return false;
   else
     form.newDir.value = dir;
   return true;
  }// getDirNameform
  // -->
</SCRIPT>

<TABLE BORDER=0 WIDTH=90%><TR>

 <FORM NAME="mkdirForm" METHOD=POST ACTION="index.cgi" onSubmit="return getDirName(this);">
 <TD WIDTH=60 VALIGN=CENTER>
  <INPUT TYPE=HIDDEN NAME="mode" VALUE="mkdir">
  <INPUT TYPE=HIDDEN NAME="currentDir" VALUE="#DIR#">
  <INPUT TYPE=HIDDEN NAME="newDir" VALUE="">
  <CENTER>
   <INPUT TYPE=IMAGE SRC="/images/newDir.gif" ALT="Create New Dir" BORDER=0 WIDTH=45 HEIGHT=45 onMouseOver="window.status='Create new dir.'; return true;">
   <A HREF="javascript:if (getDirName(document.mkdirForm)) document.mkdirForm.submit();" onMouseOver="window.status='Create new dir.'; return true;"><FONT SIZE=-1 FACE="Helvetica,Arial">New&nbsp;Dir</FONT></A>
  </CENTER>
 </TD>
 </FORM>

<TD WIDTH=60 VALIGN=CENTER>
 <A HREF="new_index.cgi?dir=#DIR#&mode=showUploadForm" TARGET="uploadWin" onClick="window.open('','uploadWin','width=500,height=250,resizable=1,scrollbars=yes');" onMouseOver="window.status='Upload a file.'; return true;"><IMG SRC="/images/upload.gif" ALT="Upload Icon" BORDER=0 WIDTH=45 HEIGHT=45></A>
 <A HREF="new_index.cgi?dir=#DIR#&mode=showUploadForm" TARGET="uploadWin" onClick="window.open('','uploadWin','width=500,height=250,resizable=1,scrollbars=yes');" onMouseOver="window.status='Upload a file.'; return true;"><FONT SIZE=-1 FACE="Helvetica,Arial">Upload</FONT></A>
</TD>
<TD WIDTH=100%>&nbsp;</TD> 
</TR></TABLE>
