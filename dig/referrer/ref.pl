#!/usr/bin/perl -w

######################################################################
#
# ID:		$Id$
#
# Author:	paul.egan@disney.com
# 		2001/12/04
#
# Last update:	$Author$
# 		$Date$
#
# Description:	
#
# Log:		$Log$
# Log:		Revision 1.2  2001/12/06 16:03:15  pegan
# Log:		Now works as CGI
# Log:
# Log:		Revision 1.1  2001/12/05 14:41:22  pegan
# Log:		Initial revision
# Log:
#
######################################################################

use strict;
use DBI;
use CGI(':standard');

#########################
# Config:

my @IndexPages = ('index.cgi','index.jsp','index.html','home.html',
		    'down.html','index.htm','index.shtml',
		    'redirectsurl.html','redirectsurl.htm');

my $HTML = 'ref.html';

my $DB_dsn = "DBI:mysql:referrer";
my $DB_user = "root";
my $DB_passwd = "myc166dah";

my $MaxResults = 50;

my $SQL_host = "
 SELECT ref_host AS ref, SUM(count) AS count
   FROM *TABLE*
  WHERE year = *YEAR* *SUBQ*
    AND url = '*URL*'
  GROUP BY ref_host
  ORDER BY count desc
  LIMIT $MaxResults;";

my $SQL_url = "
 SELECT CONCAT(ref_host,ref_url) AS ref, SUM(count) AS count
   FROM *TABLE*
  WHERE year = *YEAR* *SUBQ*
    AND url = '*URL*'
  GROUP BY ref_host,ref_url
  ORDER BY count desc
  LIMIT $MaxResults;";

#########################
# Initialization:

$SIG{__DIE__} = sub { die(header("text/plain")."\n".$_[0]."\n"); };

my $indeces = join("|",@IndexPages);


#########################
# Main:

my $results;

if (param('site') && param('url') && param('year')){

  my $sql = param('showURL')?$SQL_url:$SQL_host;
  my $url = param('url');

  $url =~ s/($indeces)$//;
  $url .= "/" if ($url =~ /^[^\.]*[^\/]$/);

  $sql =~ s/\*URL\*/$url/;
  $sql =~ s/\*TABLE\*/param('site')/e;
  $sql =~ s/\*YEAR\*/param('year')/e;

  if (param('month') && param('day')){
    $sql =~ s/\*SUBQ\*/"AND month = ".param('month')." AND day = ".param('day')/e;
  }elsif (param('month')){
    $sql =~ s/\*SUBQ\*/"AND month = ".param('month')/e;
  }else{
    $sql =~ s/\*SUBQ\*//;
  }

  $results = getResults($sql);
}

showPage($results);

exit(0);


######################################################################
# getResults() -

sub getResults
{
 my $sql = $_[0];

 my $db = DBI->connect($DB_dsn,$DB_user,$DB_passwd);
 die("Couldn't connect to database\n") if (!$db);

 my $s = $db->prepare($sql);
 die("Couldn't execute SQL\n") if (!$s || !$s->execute());

 my $results = "";
 while (my ($ref,$count) = $s->fetchrow_array()){
   my $text = $ref;
   $text = substr($text,0,100)."..." if (length($text)>100);
   $results .= Tr( td($count) . td(a( {href=>"http://$ref"},$text)) )."\n";
 }

 $s->finish();
 $db->disconnect();

 return ($results);
}


######################################################################
# showPage() -

sub showPage
{
 my $tableBody = $_[0] || "";
 my $page;

 local $/ = undef;
 open(F,"<$HTML") || die("Could not open HTML : $!\n");
   $page = <F>;
 close(F);

 $page =~ s/<!-- TABLE BODY -->/$tableBody/;

 print header().$page;

 return(1);
}
