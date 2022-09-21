<!-- ------------- Start of left bar ------------- -->

<SCRIPT LANGUAGE="JavaScript">
<!--

 function preloadImage(img)
 {
  var i = new Image(); 
  i.src = img; 
  return(i); 
 }

 function doSearch()
 {
  if ( document.searchForm['NS-query'].value )
    document.searchForm.submit();
  else
    document.location = "/search";

  return false;
 }

 browserSupportsJSImages = 
  (((navigator.appName == "Netscape")			&& (parseInt(navigator.appVersion) >= 3 )) || 
   ((navigator.appName == "Microsoft Internet Explorer") && (parseInt(navigator.appVersion) >= 4 ))    ); 

 if (browserSupportsJSImages){ 
   search_G = preloadImage("/images/bar/search_G.gif");
   search_W = preloadImage("/images/bar/search_W.gif");
#PRELOADIMAGES#
 }
// -->

</SCRIPT>

#LINKSnIMAGES#

<BR>
<FORM NAME="searchForm" METHOD=POST ACTION="/search?NS-search-page=results" TARGET="_top">
 <INPUT TYPE=HIDDEN NAME="NS-search-type" VALUE="NS-boolean-query">
 <INPUT TYPE=HIDDEN NAME="NS-max-records" VALUE="$$NS-max-records">
 <INPUT TYPE=HIDDEN NAME="NS-collection"  VALUE="All">
 <INPUT TYPE=TEXT NAME="NS-query" SIZE=12>
 <BR>
 <A HREF="/search" onClick="return doSearch();" onMouseOver="if (browserSupportsJSImages) document['search'].src=search_W.src" onMouseOut="if (browserSupportsJSImages) document['search'].src=search_G.src"><IMG SRC="/images/bar/search_G.gif" NAME="search" ALT="Search" BORDER=0 HEIGHT=20 WIDTH=130></A>
</FORM>

</TD>

<!-- ------------- End of left bar ------------- -->

<TD ALIGN=LEFT>

<IMG SRC="/images/HR_EMEA_header.gif" ALT="HR EMEA" BORDER=0 HEIGHT=80 WIDTH=550><BR>
<P>