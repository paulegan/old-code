#!/usr/local/bin/perl

####################################################################
#
#  Original Author:  paul.egan@internal.ozemail.com.au
#  Created:          20000217
#
#  Author:           $Author$
#
#  Revision: 	     $Id$
#  Locked:   	     $Locker:  $
#  State:    	     $State: Exp $
#
#  Description:      
#
#  History:	     $Log: Flow.pm,v $
#  History:	     Revision 1.1  2000/02/18 11:13:28  pegan
#  History:	     An initial release
#  History:	
# 
#  CVS tagid @(#)$Name:  $ 
#  CVS rcsid @(#)$Id$
#
####################################################################

package Flow;

$Flow::VERSION = do { my @r = (q$Revision: 1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r };

$defaultTemplateDir = "./templates";
$stateDir = "stateDB";
$brandParam = "ISP";

use CGI qw(:cgi);
use CGI::Carp qw(fatalsToBrowser);
use CGI::Persistent; 
use HTML::Template;


####################################################################
# new(flow,defaultPage,templateDir) - instansiates a new Flow object
# from the specified flow.
#
sub new
{
 my $pkg = shift();
 my $class = ref($pkg) || $pkg;
 my $self = {};

 bless($self, $class);

 $self->{'flow'} = shift();
 $self->{'defaultPage'} = shift();
 $self->{'templateDir'} = shift() || $defaultTemplateDir;
 $self->{'pageNumber'} = -1;
 $self->{'cgi'} = new CGI::Persistent($stateDir);

 return ($self);
}# new


####################################################################
# displayCurrentPage() - this method does the main body of the work.
# Determines which page to display, calls the branch function, parses
# the page template with the CGI values & any referenced dataModule
# methods, and dumps the result to the browser.
#
sub displayCurrentPage
{
 my $self = shift();
 my $page;
 my $template;
 my %param;

 ( $page = $self->{'cgi'}->path_info() ) =~ s/^\///;
 $page = $self->{'defaultPage'} if (!$page);

 if (${${$self->{'flow'}}{$page}}{'branchFunc'}){
   &{${${$self->{'flow'}}{$page}}{'branchFunc'}}($self->{'flow'},$page,$self->{'cgi'});
 }

 $templateFile = $self->getTemplate($page) || die("Template for $page does not exist");

 $template = new HTML::Template(filename => $templateFile,
                                associate => $self->{'cgi'},
                                die_on_bad_params => 0,
                                vanguard_compatibility_mode => 1);

 for my $dataModule ( $self->getDataModules($template) ){
   eval("use $dataModule;");
   my $mod = $dataModule->new($self->{'cgi'});
   for my $method ( $self->getDMMethods($template,$dataModule) ){
     $param{"$dataModule.$method"} = $mod->$method();
   }
 }

 $param{'NEXT'} = $self->{'cgi'}->script_name()."/".${${$self->{'flow'}}{$page}}{'next'};
 $param{'BACK'} = $self->{'cgi'}->script_name()."/".${${$self->{'flow'}}{$page}}{'prev'};

 $template->param(%param);

 print $self->{'cgi'}->header();
 print $template->output();

 $self->{'cgi'}->delete_all() if (!${${$self->{'flow'}}{$page}}{'next'});

 return (1);
}# displayCurrentPage


####################################################################
# getDataModules(template) - returns a list of dataModules
# referenced within the template.
#
sub getDataModules
{
 my $self = shift();
 my $template = shift();
 my @mods;

 for my $param ( $template->param() ){
   push(@mods,$1) if ( ($param =~ /(\w+)\..*/) && !grep(/$1/,@mods));
 }

 return (@mods);
}# getDataModules


####################################################################
# getDMMethods(template,module) - returns the list of methods of
# the specified module referenced in the template
#
sub getDMMethods
{
 my $self = shift();
 my $template = shift();
 my $module = shift();
 my @methods;

 for my $param ( $template->param() ){
   push(@methods,$1) if ( ($param =~ /$module\.(\w+)/) && !grep(/$1/,@methods));
 }
 
 return (@methods);
}# getDMMethods


####################################################################
# getTemplate(page) - finds the appropriate template file name for 
# the given page name.
#
sub getTemplate
{
 my $self = shift();
 my $page = shift() || $self->{'defaultPage'};
 my $template = ${${$self->{'flow'}}{$page}}{'template'}.".html";

 if ( -r $self->{'templateDir'}."/".$self->{'cgi'}->param($brandParam)."/".$template){
   return ($self->{'templateDir'}."/".$self->{'cgi'}->param($brandParam)."/".$template);
 }elsif ( -r $self->{'templateDir'}."/".$template){
   return ($self->{'templateDir'}."/".$template);
 }else{
   return (0);
 }

}# getTemplate

1;
