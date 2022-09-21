
# $Id$

=head1 NAME

WDIG::Content::Cmd::_adv_platform_ops - library of functions for the advanced platform

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/20

=head1 SYNOPSIS

=head1 DESCRIPTION

Library of functions for the advanced platform

=head1 EXPORTS

update_app_files
update_config
update_db
restart_app

=head1 HISTORY

 $Log: _adv_platform_ops.pm,v $
 Revision 1.1  2003/06/30 02:48:04  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Content::Cmd::_adv_platform_ops;

use 5.006_001;
use strict;
use warnings;
use base qw(Exporter);

our @ISA;
our @EXPORT = qw(update_app_files update_config update_db restart_app);
our @EXPORT_OK = qw();
our $VERSION = '1.0';

######################################################################

use Cwd;
use File::Spec::Functions;
use File::Rsync;
use Log::Log4perl qw(get_logger);
use WDIG::Content::Cmd::Error qw(:try);
use WDIG::Content::Drop;
use WDIG::Content::URL_Map;
use WDIG::Content::User::DB;
use WDIG::Weblogic::Application;

my $Rsync_Defaults = { 
                       rsh=>'ssh -T -a -x',
                       archive=>0,
                       compress=>0,
		       perms=>1,
		       relative=>1,
		       'log-format'=>'%f',
		       debug=>0
		     };


my $logger = logger();


######################################################################
# update_app_files($drop,%options) - 
#

sub update_app_files
{
 my $drop = shift();
 my %opt = @_;

 my $sub_dir = 'app'; # here be magic!
 my $source = catdir($drop->source_local(), $sub_dir);

 my @files = map { /^$sub_dir.(.*)$/ } $drop->app_files();

 my $cwd = cwd();
 chdir($source) or throw WDIG::Content::Cmd::Error($source.": $!");
 
 my $rsync = new File::Rsync($Rsync_Defaults);

 if ( !$drop->app_name() or !$drop->domain_name() ) {
   throw WDIG::Content::Cmd::Error("No app params found for ".$drop->dest())
 }

 my $app = new WDIG::Weblogic::Application($drop->app_name(), $drop->domain_name(), %{$opt{weblogic}});

 throw WDIG::Content::Cmd::Error("No target servers for $app->{name}") if ( !$app->targets() );

 foreach my $server_name ( $app->targets() ) {

   my $server = new WDIG::Weblogic::Server($server_name, $drop->domain_name(), %{$opt{weblogic}});

   my $dest = $server->is_remote() ? $server->machine().':'.$app->path() : $app->path();

   $logger->info("cd $source; rsync ".join(" ", @files)." ".$dest);
   
   my $success = $rsync->exec(src=>\@files, dest=>$dest);
   
   throw WDIG::Content::Cmd::Error("Rsync error: ".join(" ",$rsync->err)) if ( !$success );
   
   $logger->debug("App files: \n".join("", $rsync->out));

 }

 chdir($cwd);

 return (1);
}


######################################################################
# update_config($drop,%options) - 
#

sub update_config
{
 my $drop = shift();
 my %opt = @_;

 my $source = $drop->source_local();
 my @files = $drop->config_files();

 my $cwd = cwd();
 chdir($source) or throw WDIG::Content::Cmd::Error($source.": $!");
 
 my $rsync = new File::Rsync($Rsync_Defaults);

 if ( !$drop->app_name() or !$drop->domain_name() ) {
   throw WDIG::Content::Cmd::Error("No app params found for ".$drop->dest())
 }

 my $app = new WDIG::Weblogic::Application($drop->app_name(), $drop->domain_name(), %{$opt{weblogic}});

 throw WDIG::Content::Cmd::Error("No target servers for $app->{name}") if ( !$app->targets() );

 foreach my $server_name ( $app->targets() ) {

   my $server = new WDIG::Weblogic::Server($server_name, $drop->domain_name(), %{$opt{weblogic}});

   my $dest = $server->is_remote() ? $server->machine().':'.$server->wl_home() : $server->wl_home();

   $logger->info("cd $source; rsync ".join(" ", @files)." ".$dest);
   
   my $success = $rsync->exec(src=>\@files, dest=>$dest);
   
   throw WDIG::Content::Cmd::Error("Rsync error: ".join(" ",$rsync->err)) if ( !$success );
   
   $logger->debug("Config files: \n".join("", $rsync->out));

 }

 chdir($cwd);

 return (1);
}



######################################################################
# update_db($drop,%options) - 
#

sub update_db
{
 my $drop = shift();
 my %opt = @_;

 my @db_files = $drop->db_files();
 $logger->error("DB FILES: ".join(",",@db_files));

 return (1);
}


######################################################################
# restart_app($drop,%options) - 
#

sub restart_app
{
 my $drop = shift();
 my %opt = @_;
 my $success = undef;

 if ( !$drop->app_name() or !$drop->domain_name() ) {
   throw WDIG::Content::Cmd::Error("No app params found for ".$drop->dest())
 }

 $logger->info("Restarting ".$drop->domain_name().":".$drop->app_name()."");

 try {
 
   my $app = new WDIG::Weblogic::Application($drop->app_name(), $drop->domain_name(), %{$opt{weblogic}});

   if ( $app->ping(feedback=>0) ) {

     $success = $app->restart();

     throw WDIG::Content::Cmd::Error("Unable to restart app") if ( !$success );
     
     notify_app_owners($drop->app_name(), $drop->domain_name(), 
                       subject=>"Restarted ".$drop->domain_name().':'.$drop->app_name(),
		       message=>"Please check the logs to verify successful startup." );

   } else {
   
     $logger->warn($drop->domain_name().":".$drop->app_name()." is not running");
     $success = 1;

   }

 } catch Error with {

   my $error = shift();
   notify_app_owners($drop->app_name(), $drop->domain_name(), 
                     subject=>"Error restarting ".$drop->domain_name().':'.$drop->app_name(),
		     message=>"Please check the logs for further details.\n\n".$error->text() );
   throw $error;

 };

 return ($success);
}


######################################################################
# notify_app_owners() -
#

sub notify_app_owners
{
 my $app_name = shift();
 my $domain_name = shift();
 my %mail_args = @_;

 my $user_db = new WDIG::Content::User::DB();

 $logger->debug("Send mail to app owners for $domain_name:$app_name");

 my @users = $user_db->search_by_app($app_name, $domain_name);

 foreach my $user ( @users ) {
   if ( $user->send_message(%mail_args) ) {
     $logger->error("Error sending mail to ".$user->name());
   } else {
     $logger->debug("Sent restart message to ".$user->name());
   }
 }

 return (1);
}


######################################################################
# logger() - Sets the log4perl logger with the specified catetgory
#

sub logger
{
 return ($logger = get_logger(shift()));
}


1;
__END__


