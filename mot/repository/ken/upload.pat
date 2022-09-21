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
 <META NAME="History"		CONTENT="">
 <META NAME="Description"       CONTENT="Form for selecting a file for upload to a document repository.">
 <META NAME="Keywords"          CONTENT="file,repository,upload">
 <META NAME="Notes"             CONTENT=""> 
</HEAD>

<BODY BGCOLOR="#C3C3C3" TEXT="#000000" LINK="#0000FF" VLINK="#000077">

#HEADER#

#LEFTBAR#

<FORM NAME="uploadForm" ENCTYPE="multipart/form-data" METHOD=POST ACTION="upload.cgi" TARGET="docRepMain">
  <INPUT TYPE=HIDDEN NAME="mode" VALUE="upload">
  <INPUT TYPE=HIDDEN NAME="dir" VALUE="#DIR#">
  <TABLE BORDER=0>
   <TR><TD ALIGN=LEFT><FONT FACE="Helvetica,Arial">File to upload:</FONT></TD></TR>
   <TR><TD ALIGN=LEFT><INPUT TYPE=FILE SIZE=30 NAME="file">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Helvetica,Arial">Remote dir/file name:</FONT></TD></TD></TR>
   <TR><TD ALIGN=LEFT><INPUT SIZE=30 NAME="newName" VALUE=""></TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Helvetica,Arial" SIZE=-1> * Do not specify a remote file name to leave it unchanged.</FONT></TD></TR>
   <TR><TD ALIGN=RIGHT><FONT FACE="Fixed" SIZE=+1><BR>
    <INPUT TYPE=BUTTON VALUE="  Cancel  " onClick="#CANCEL_CODE#">
    <INPUT TYPE=SUBMIT VALUE="  Upload  ">
   </FONT></TD></TR>
  </TABLE>
</FORM>

#FOOTER#

</BODY>

</HTML>
