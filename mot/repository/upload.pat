<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">

<HTML>

<HEAD>
 <TITLE>Document Repository Upload</TITLE>
 <META NAME="Author"		CONTENT="paul@hr.emea.mot.com">
 <META NAME="DevDate"           CONTENT="19990202">
 <META NAME="TestedWith"        CONTENT="">
 <META NAME="TestDate"          CONTENT="">
 <META NAME="ReviewedBy"        CONTENT="">
 <META NAME="ReviewDate"        CONTENT="">
 <META NAME="ApprovedBy"        CONTENT="">
 <META NAME="ProdDate"          CONTENT="">
 <META NAME="History"		    CONTENT="">
 <META NAME="Description"       CONTENT="Form for selecting a file for upload to a document repository.">
 <META NAME="Keywords"          CONTENT="file,repository,upload">
 <META NAME="Notes"             CONTENT=""> 
</HEAD>

<BODY BGCOLOR="#FFFFFF" TEXT="#000000" LINK="#0000FF" VLINK="#000077">

<FORM NAME="uploadForm" ENCTYPE="multipart/form-data" METHOD=POST ACTION="index.cgi" TARGET="docRepMain">
  <INPUT TYPE=HIDDEN NAME="mode" VALUE="upload">
  <INPUT TYPE=HIDDEN NAME="dir" VALUE="#DIR#">
  <TABLE BORDER=0>
   <TR><TD ALIGN=LEFT><FONT FACE="Arial, Helvetica" size="4">Document Repository Upload</FONT><P></TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Arial, Helvetica" size="2"><IMG SRC="/images/uploadfile.gif" ALT="Upload File" BORDER=0 WIDTH=32 HEIGHT=32>&nbsp;File to upload:</FONT></TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Arial, Helvetica" size="2"><INPUT TYPE=FILE SIZE=30 NAME="file">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</FONT></TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Arial, Helvetica" size="2">Remote dir/file name:</FONT></TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Arial, Helvetica" size="2"><INPUT SIZE=30 NAME="newName" VALUE=""></FONT></TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Arial, Helvetica" SIZE="1"> * Do not specify a remote file name to leave it unchanged.</FONT></TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Arial, Helvetica" size="2">Notify <A HREF="?mode=listGroupMembers">group members</A> of this change?</FONT>&nbsp;&nbsp;<INPUT TYPE=CHECKBOX NAME="sendmail"></TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Arial, Helvetica" size="2">&nbsp;<br><INPUT TYPE=BUTTON VALUE="  Cancel  " onClick="#CANCEL_CODE#">&nbsp;<INPUT TYPE=SUBMIT VALUE="  Upload  "></FONT></TD></TR>
  </TABLE>
</FORM>

</BODY>

</HTML>
