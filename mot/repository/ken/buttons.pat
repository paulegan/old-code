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

<TABLE BORDER=0><TR>
 <FORM NAME="mkdirForm" METHOD=POST ACTION="upload.cgi" onSubmit="return getDirName(this);">
 <TD WIDTH=80 ALIGN=Center VALIGN=CENTER>
  <INPUT TYPE=HIDDEN NAME="mode" VALUE="mkdir">
  <INPUT TYPE=HIDDEN NAME="dir" VALUE="#DIR#">
  <INPUT TYPE=HIDDEN NAME="newDir" VALUE="">
  <CENTER>
   <INPUT TYPE=IMAGE SRC="/images/icons/dir.gif" ALT="Create New Folder" BORDER=0 WIDTH=32 HEIGHT=32 onMouseOver="window.status='Create New Folder'; return true;"><br>
   <A HREF="javascript:if (getDirName(document.mkdirForm)) document.mkdirForm.submit();" onMouseOver="window.status='Create New Folder'; return true;"><FONT SIZE=1 FACE="MS Sans Serif">Create<br>a New Folder<br></FONT></A>
  </CENTER>
 </TD>
 </FORM>


 <TD WIDTH=80 ALIGN=Center VALIGN=CENTER>
  <A HREF="upload.cgi?dir=#DIR#&mode=showUploadForm" #NEW_WIN_CODE# onMouseOver="window.status='Upload a File.'; return true;"><IMG SRC="/images/icons/unknown.gif" ALT="Upload a File" BORDER=0 WIDTH=32 HEIGHT=32></A><br>
  <A HREF="upload.cgi?dir=#DIR#&mode=showUploadForm" #NEW_WIN_CODE# onMouseOver="window.status='Upload a File.'; return true;"><FONT SIZE=1 FACE="MS Sans Serif">Upload<br> a File<br></FONT></A>
 </TD>
</TR></TABLE>
<P>
<FONT FACE="Arial, Verdana" SIZE=1>To remove files or folders please email<br><A HREF="mailto:kmccar01@email.mot.com">kmccar01@email.mot.com</a><br>
