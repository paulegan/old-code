
# $Id$

=head1 NAME

WDIG::Content::Cmd::refresh_app - Command to refresh application content

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/15

=head1 SYNOPSIS

 use WDIG::Content::Cmd;

 my $cmd = new WDIG::Content::Cmd('refresh_app');

 my $success = $cmd->exec($input);

 print $cmd->output();

=head1 DESCRIPTION

Command to refresh application content

=head1 EXPORTS

Nothing

=head1 HISTORY

 $Log: refresh_app.pm,v $
 Revision 1.1  2003/06/30 02:48:04  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Content::Cmd::refresh_app;

use 5.006_001;
use strict;
use warnings;
use base qw(WDIG::Content::Cmd);

our @ISA;
our $VERSION = '1.0';

######################################################################

use Cwd;
use File::stat;
use File::Find;
use File::Touch;
use XML::Simple qw(:strict);
use WDIG::Content::Cmd::Error qw(:try);
use WDIG::Content::Cmd::_adv_platform_ops;


######################################################################
# exec($input) - Uses the input values to create a drop file list
# and processes this.
#

sub exec
{
 my $cmd = shift();
 my $input = shift() || $cmd->{input};
 my %opt = @_;
 my $success = undef;
 my $timestamp;
 my $app_name;
 my $domain_name;

 %opt = ( %{$cmd->{opt}}, %opt );

 throw WDIG::Content::Cmd::Error("No input") if ( !$input );

 if ( ref($input) eq 'ARRAY' ) {

   ( $app_name, $domain_name, $timestamp ) = @$input;

 } elsif ( ref($input) eq 'HASH' ) {

   ( $app_name, $domain_name, $timestamp ) = ( $input->{app_name}, $input->{domain_name}, $input->{timestamp} );

 } elsif ( $input !~ /^<.*>$/s ) {

   ( $app_name, $domain_name, $timestamp ) = split(/\s+/, $input);

 } else {

   my $data = {};
   throw WDIG::Content::Cmd::Error("Unable to parse '$input' as XML: $@")
     if ( ! eval { $data = XMLin($input, ForceArray=>{}, KeyAttr=>{}, ContentKey=>'timestamp') } );
   ( $app_name, $domain_name, $timestamp ) = ( $data->{app_name}, $data->{domain_name}, $data->{timestamp} );
   
 }

 throw WDIG::Content::Cmd::Error("Invalid input: $input")
     unless ( $app_name and $domain_name and $timestamp );

 $cmd->{logger}->info("Refreshing: $domain_name:$app_name with timestamp $timestamp");

 my $drop = $cmd->create_refresh_drop($app_name, $domain_name, $timestamp, %opt);

 $success = $cmd->process_drop($drop, %opt);

 $cmd->{logger}->debug($drop);

 $cmd->{output} = $drop;

 return ($success);
}


######################################################################
# create_refresh_drop($app_name,$domain_name,$timestamp,%opt) - 
# Creates a drop object for the app using the timestamp and URL_Map.
#

sub create_refresh_drop
{
 my $cmd = shift();
 my $app_name = shift();
 my $domain_name = shift();
 my $timestamp = shift();
 my %opt = @_;
 my $timestamp_file = undef;

 %opt = ( %{$cmd->{opt}}, %opt );

 my $url_map = new WDIG::Content::URL_Map($opt{url_map_file});

 my ( $dest ) = $url_map->search(app_name=>$app_name, domain_name=>$domain_name);

 throw WDIG::Content::Cmd::Error("Could not find URL for $domain_name:$app_name in ".$url_map->map_file()) if ( !$dest );
 
 my $source = $url_map->to_path_root($dest);

 my $wd = cwd();
 chdir($source) or throw WDIG::Content::Cmd::Error("$source: $!");

 $cmd->{logger}->debug("Source: $source");

 if ( $timestamp !~ /^\d+$/ ) {
   $timestamp_file = $timestamp;
   if ( !-e $timestamp_file ) {
     $cmd->{logger}->warn("$timestamp: file not found");
     touch($timestamp_file);
     $timestamp = 1;
   } else {
     $timestamp = stat($timestamp_file)->mtime();
   }
 }

 $cmd->{logger}->debug("Timestamp: ".localtime($timestamp));

 my @file_list = ();

 find( sub {
             push(@file_list, $File::Find::name)
	          if ( -f $_ and stat($_)->mtime() >= $timestamp );
	   } ,'.'); 

 touch($timestamp_file) if ( $timestamp_file );

 chdir($wd);

 $cmd->{logger}->debug("Files: ".join(",",@file_list));

 return (new WDIG::Content::Drop("file://$source", $dest, @file_list));
}


######################################################################
# process_drop($drop,%options) - Process each file type appropriately
#

sub process_drop
{
 my $cmd = shift();
 my $drop = shift();
 my %opt = @_;
 my $success = 1;

 %opt = ( %{$cmd->{opt}}, %opt );

 WDIG::Content::Cmd::_adv_platform_ops::logger(__PACKAGE__);

 throw WDIG::Content::Cmd::Error("Drop is not of advanced type!") if ( !$drop->is_advanced_platform() );

 if ( $drop->app_files() ) {
   $success = update_app_files($drop, %opt);
 }

 if ( $drop->config_files() ) {
   $success = update_config($drop, %opt);
 }

 if ( $drop->db_files() ) {
   $success = update_db($drop, %opt);
 }
 
 if ( $drop->requires_restart() ) {
   $success = restart_app($drop, %opt);
 }
 
 return ($success);
}


1;
__END__


