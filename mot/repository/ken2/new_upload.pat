<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">

<HTML>

<HEAD>
 <TITLE>Document Repository Upload</TITLE>
 <META NAME="Author"		CONTENT="paul@hr.emea.mot.com">
 <META NAME="IndexType"         CONTENT="">
 <META NAME="Abstract"          CONTENT="Form to upload a file to the doc repository.">
 <META NAME="DocDate"           CONTENT="980717">
 <META NAME="Group"             CONTENT="">
 <META NAME="CountryRelevance"  CONTENT="">
</HEAD>

<BODY BGCOLOR="#FFFFFF" TEXT="#000000" LINK="#0000FF" VLINK="#000077">

<FORM NAME="uploadForm" ENCTYPE="multipart/form-data" METHOD=POST ACTION="new_index.cgi" TARGET="docRepMain">
  <INPUT TYPE=HIDDEN NAME="mode" VALUE="upload">
  <INPUT TYPE=HIDDEN NAME="dir" VALUE="#DIR#">
  <TABLE BORDER=0>
   <TR><TD ALIGN=LEFT><FONT FACE="Helvetica,Arial">File to upload:</FONT></TD></TR>
   <TR><TD ALIGN=LEFT><INPUT TYPE=FILE SIZE=0 NAME="theFile">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Helvetica,Arial">Remote dir/file name:</FONT></TD></TD></TR>
   <TR><TD ALIGN=LEFT><INPUT SIZE=30 NAME="remoteLocation" VALUE=""></TD></TR>
   <TR><TD ALIGN=LEFT><FONT FACE="Helvetica,Arial" SIZE=-1> * Do not specify a remote file name to leave it unchanged.</FONT></TD></TR>
   <TR><TD ALIGN=RIGHT><FONT FACE="Fixed" SIZE=+1><BR>
    <INPUT TYPE=BUTTON VALUE="  Cancel  " onClick="top.close();">
    <INPUT TYPE=SUBMIT VALUE="  Upload  ">
   </FONT></TD></TR>
  </TABLE>
</FORM>

</BODY>

</HTML>
