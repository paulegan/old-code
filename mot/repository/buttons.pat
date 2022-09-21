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
<FORM NAME="mkdirForm" METHOD=POST ACTION="index.cgi" onSubmit="return getDirName(this);">
<TABLE BORDER=0 cellpadding=0 cellspacing=0>
<TR>
<TD WIDTH=80 ALIGN=center VALIGN=top>
<INPUT TYPE=HIDDEN NAME="mode" VALUE="mkdir">
<INPUT TYPE=HIDDEN NAME="dir" VALUE="#DIR#">
<INPUT TYPE=HIDDEN NAME="newDir" VALUE="">
<INPUT TYPE=IMAGE SRC="/images/newfolder.gif" ALT="Create New Folder" BORDER=0 WIDTH=32 HEIGHT=32 onMouseOver="window.status='Create New Folder'; return true;"><br>
<A HREF="javascript:if (getDirName(document.mkdirForm)) document.mkdirForm.submit();" onMouseOver="window.status='Create New Folder'; return true;"><FONT SIZE="1" FACE="MS Sans Serif">New&nbsp;Folder</FONT></A>
</TD>
<TD WIDTH=80 ALIGN=center VALIGN=top>
<A HREF="index.cgi?dir=#DIR#&mode=showUploadForm" #NEW_WIN_CODE# onMouseOver="window.status='Upload File'; return true;"><IMG SRC="/images/uploadfile.gif" ALT="Upload File" BORDER=0 WIDTH=32 HEIGHT=32></A><br>
<A HREF="index.cgi?dir=#DIR#&mode=showUploadForm" #NEW_WIN_CODE# onMouseOver="window.status='Upload File'; return true;"><FONT SIZE="1" FACE="MS Sans Serif">Upload&nbsp;File</FONT></A>
</TD>
<TD WIDTH=80 ALIGN=center VALIGN=top>
<A HREF="mailto:kmccar01@email.mot.com?subject=#REP_NAME# Delete File"><IMG SRC="/images/delete.gif" ALT="Delete File" BORDER=0 WIDTH=32 HEIGHT=32></A><br>
<A HREF="mailto:kmccar01@email.mot.com?subject=#REP_NAME# Delete File"><FONT SIZE="1" FACE="MS Sans Serif">Delete&nbsp; File</FONT></A>
</TD>
<TD WIDTH=80 ALIGN=center VALIGN=top>
<A HREF="index.cgi?mode=listGroupMembers" #NEW_WIN_CODE# onMouseOver="window.status='Show Group Members'; return true;"><IMG SRC="/images/group.gif" ALT="Show Group Members" BORDER=0 WIDTH=32 HEIGHT=32></A><br>
<A HREF="index.cgi?mode=listGroupMembers" #NEW_WIN_CODE# onMouseOver="window.status='Show Group Members'; return true;"><FONT SIZE="1" FACE="MS Sans Serif">Group<br>Members</br></FONT></A>
</TD>
<TD WIDTH=80 ALIGN=center VALIGN=top>
<A HREF="index.cgi?mode=help" #NEW_WIN_CODE# onMouseOver="window.status='Show Help'; return true;"><IMG SRC="/images/help.gif" ALT="Show Help" BORDER=0 WIDTH=32 HEIGHT=32></A><br>
<A HREF="index.cgi?mode=help" #NEW_WIN_CODE# onMouseOver="window.status='Show Help'; return true;"><FONT SIZE="1" FACE="MS Sans Serif">Help</FONT></A>
</TD>
</TR>
</TABLE>
</FORM>
