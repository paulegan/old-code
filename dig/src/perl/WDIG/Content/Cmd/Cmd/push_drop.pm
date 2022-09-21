
# $Id$

=head1 NAME

WDIG::Content::Cmd::push_drop - Command implementation for promoting a drop

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/15

=head1 SYNOPSIS

 use WDIG::Content::Cmd;

 my $cmd = new WDIG::Content::Cmd('push_drop');

 my $drop = new WDIG::Content::Drop($drop_xml);

 my $success = $cmd->exec($input);

 print $cmd->output();

=head1 DESCRIPTION

Copies the drop from the source to the destination.

=head1 EXPORTS

Nothing

=head1 HISTORY

 $Log: push_drop.pm,v $
 Revision 1.3  2003/09/03 23:20:32  pegan
 enabled push & process methods
 tidied up some little bugs

 Revision 1.2  2003/08/26 22:36:38  pegan
 added the process_content() methods

 Revision 1.1  2003/06/30 02:48:04  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Content::Cmd::push_drop;

use 5.006_001;
use strict;
use warnings;
use base qw(WDIG::Content::Cmd);

our @ISA;
our $VERSION = '1.0';

######################################################################

use File::Spec::Functions qw(catfile);
use WDIG::File::Cmd;
use WDIG::Content::Drop;
use WDIG::Content::Cmd::Error qw(:try);
use WDIG::Content::Cmd::_adv_platform_ops;
use WDIG::Pluto::RpcClient;
use WDIG::DB::SqlInterpreter;


######################################################################
# exec($input) - Creates a drop from the input and executes the 
# push_drop command.
#

sub exec
{
 my $cmd = shift();
 my $input = shift() || $cmd->{input};
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$cmd->{opt}}, %opt );

 throw WDIG::Content::Cmd::Error("No input") if ( !$input );

 my $drop;
 my $invalid_files = {};
 try {
   local $SIG{__WARN__} = sub { return WDIG::Content::Drop::file_list_warn_handler($invalid_files,@_); };
   $drop = new WDIG::Content::Drop($input);
 } catch Error with {
   my $error = shift();
   throw WDIG::Content::Cmd::Error("Unable to create drop: ".$error->text());
 };

 $cmd->{logger}->warn("invalid files in drop: ", join(",",keys(%$invalid_files))) if ( keys(%$invalid_files) );

 $success = $cmd->push_content($drop);

 $success = $cmd->process_content($drop);

 if ( $drop->is_advanced_platform() ) {
   $success = $cmd->process_adv_content($drop, %opt);
 }

 $cmd->{logger}->info("push_drop successful");
 
 $cmd->{output} = $drop;

 return ($success);
}


######################################################################
# push_content($drop,%options) - Copies the files from the source
# to the specified destination.
#

sub push_content
{
 my $cmd = shift();
 my $drop = shift();
 my %opt = @_;
 my $success = undef;

 if ( !$drop->file_list() ) {
   $cmd->{logger}->info("No files in drop");
   return (1);
 }

 $cmd->{logger}->error("No local dir for ".$drop->source()) if ( !$drop->source_local() );
 $cmd->{logger}->error("No dest root for ".$drop->dest()) if ( !$drop->dest_root() );

 $cmd->{logger}->info("cd ".$drop->source_local()."; cp ".
                       join(" ", $drop->file_list())." ".$drop->dest_root()."\n");

 cp([ $drop->file_list() ], $drop->dest_root(), wd=>$drop->source_local(), 
 			   		    preserve_path=>1, 
 			   		    preserve_mode=>1)
      or $cmd->{logger}->warn("Error copying files: $!");

 return ($success);
}


######################################################################
# process_content($drop,%options) - Processes the drop for akamai
# hitbox and other such stuff.
#

sub process_content
{
 my $cmd = shift();
 my $drop = shift();
 my %opt = @_;
 my $success = 1;
 
 %opt = ( %{$cmd->{opt}}, %opt );

 my $config = {};
 $cmd->apply_defaults($opt{process_content}, $config);

 my @commands = sort {$a->{order} <=> $b->{order}} @{$config->{command}||[]};
 foreach my $cmd ( @commands ) {
   $cmd->{content} =~ s/(^\s*|\s*$)//g;
   my $cmd_path = ( split(/\s+/,$cmd->{content}) )[0];
   throw WDIG::Content::Cmd::Error("Command not found: $cmd_path") if ( !-x $cmd_path );
   $cmd->{content} = '"'.$cmd->{content}.'"';
 }

 foreach my $file ( $drop->file_list() ) {

   $file = catfile($drop->dest_root(), $file);

   foreach my $command ( @commands ){

     if ($file =~ /$command->{match}/i){
       my $cmd_str = eval($command->{content});
       my $output = "";
       $success = open(P, "$cmd_str 2>&1 |") or throw WDIG::Content::Cmd::Error("$cmd_str: $!");
       while(<P>) {
         $output .= "\t$_";
       }
       close(P);
       $cmd->{logger}->info("$cmd_str :\n$output");
     }

   }

 }

 return ($success);
}


######################################################################
# process_adv_content($drop,%options) - Process the files of an 
# advanced platform drop.
#

sub process_adv_content
{
 my $cmd = shift();
 my $drop = shift();
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$cmd->{opt}}, %opt );

 $cmd->{logger}->info("Processing advanced drop");

 if ( !$drop->app_name() or !$drop->domain_name() ) {
   throw WDIG::Content::Cmd::Error("No app params found for ".$drop->dest())
 }
  
 my $app = new WDIG::Weblogic::Application($drop->app_name(), $drop->domain_name(), %{$opt{weblogic}}, log_conf_file=>$opt{log_conf_file});
 my $pluto = new WDIG::Pluto::RpcClient(%{$opt{pluto}});

 $cmd->process_db_content($drop, rpc_client=>$pluto, app=>$app, %opt) if ( $drop->db_files() );
 
 try {
  
   $success = $app->refresh();
   
 } catch Error with {
 
   my $error = shift();
   
   my $message = {
       subject => "Error deploying drop to ".$drop->domain_name().':'.$drop->app_name(),
       message => "The following error occured deploying the latest drop to ".$drop->domain_name().':'.$drop->app_name()."\n".
                  "The erroneous server will have been shutdown.\n".
                  "Please check the logs for further details.\n\n".$error->text()
   };

   $pluto->create_user_alert(projectId=>$drop->project_id(), dropVersion=>$drop->version(), actionRequired=>"true", %$message) if ( $drop->project_id() );
   $cmd->email_alert(%$message);

   throw $error;
 
 };

 if ( $drop->project_id() and $success =~ /RESTART$/ ) {
   $pluto->create_user_alert(projectId=>$drop->project_id(), dropVersion=>$drop->version(), 
       subject => "".$drop->domain_name().':'.$drop->app_name()." restarted",
       message => "The ".$drop->domain_name().':'.$drop->app_name()." servers were successfully restarted.\n".
                  "Please check the logs for further details.\n\n$success"
   );
 }

 $cmd->{logger}->debug("success: $success");

 return ($success);
}


######################################################################
# process_db_content($drop,%options) - Runs the DB files included
# in the drop.  Caveat: The app must be running in order to determine
# the db connection properties.
#

sub process_db_content
{
 my $cmd = shift();
 my $drop = shift();
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$cmd->{opt}}, %opt );

 my $app = $opt{app} || new WDIG::Weblogic::Application($drop->app_name(), $drop->domain_name(), %{$opt{weblogic}}, log_conf_file=>$opt{log_conf_file});
 my $pluto = $opt{rpc_client} || new WDIG::Pluto::RpcClient(%{$opt{pluto}});

 try {
  
   my $pool_config = $app->jdbc_config();

   throw WDIG::Content::Cmd::Error("No JDBC pool found") if ( !keys(%$pool_config) );
   throw WDIG::Content::Cmd::Error("More than 1 JDBC pool found: ".join(",",keys(%$pool_config))) if ( keys(%$pool_config) > 1 );

   my $isql = new WDIG::DB::SqlInterpreter(%{$opt{db}}, %{$pool_config->{(keys(%$pool_config))[0]}} );

   if ( $opt{db}->{backup_cmd} ) {
     $success = $isql->exec($opt{db}->{backup_cmd});
     $cmd->{logger}->debug("db backup: ".$isql->output());
   }

   foreach my $script ( $drop->db_files() ) {
     $script = catfile($drop->source_local(), $script);
     $success = $isql->exec($script);
     $cmd->{logger}->debug("$script: ".$isql->output());
   }

 } catch Error with {
 
   my $error = shift();
   
   my $message = {
       subject => "Error running DB script for ".$drop->domain_name().':'.$drop->app_name(),
       message => "An error occured deploying the latest drop to ".$drop->domain_name().':'.$drop->app_name()."\n".
                  "The DB script threw the following exception.\n".
                  "The deployment process has been halted.\n\n".$error->text()
   };

   $pluto->create_user_alert(projectId=>$drop->project_id(), dropVersion=>$drop->version(), actionRequired=>"true", %$message) if ( $drop->project_id() );
   $cmd->email_alert(%$message);

   throw $error;
 
 };

 if ( $drop->project_id() and $success ) {
   $pluto->create_user_alert(projectId=>$drop->project_id(), dropVersion=>$drop->version(), 
       subject => "DB script successful for ".$drop->domain_name().':'.$drop->app_name()."",
       message => "The following script(s) were run successfully for ".$drop->domain_name().':'.$drop->app_name().
                  "\n\n".join("\n", $drop->db_files())
   );
 }

 return ($success);
}


1;
__END__


