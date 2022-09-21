
# $Id$

=head1 NAME

WDIG::Content::Drop::Advanced - Drop sub-class for advanced platform drops

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/27

=head1 SYNOPSIS

 use WDIG::Content::Drop::Advanced;

 my $drop = new WDIG::Content::Drop($src, $dst, @file_list);

 if ( $drop->is_advanced_platform() ) {
 
   my $app_name = $drop->app_name();

   restart_app($app_name) if ( $drop->requires_restart() );

 }

=head1 DESCRIPTION

Implements the following additional methods for drops destined
for the advanced platform.

=over 4

=item app_name()

Returns the name of the app associcated with this drop.

=item domain_name()

Returns the domain name for the app associcated with this drop.

=item app_files()

Returns the list of files that are part of the application EAR.

=item config_files()

Returns the list of config files within the drop.

=item content_files()

Returns the list of content files within the drop.

=item db_files()

Returns the list of database files within the drop.

=item app_name()

Returns the list of documentation files in the drop.

=item source_files()

Returns the list of source code files in the drop.

=item app_name()

Returns true if the app will require a restart after
the drop files are deployed.

=back

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: Advanced.pm,v $
 Revision 1.1  2003/06/30 02:16:24  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

package WDIG::Content::Drop::Advanced;

use 5.006_001;
use strict;
use warnings;
use base qw(WDIG::Content::Drop);

our @ISA;
our $VERSION = '1.0';


######################################################################
# untaint_file_name() - advanced platform drops can have a "$" in the
# file name.
# 

sub untaint_file_name
{
 my $drop = shift();
 my $file_name = shift();
 my %opt = @_;

 if ( !$opt{valid_chars} ) {
   $opt{valid_chars} = '\$'.$WDIG::Content::Drop::Valid_Chars;
 }

 return ($drop->SUPER::untaint_file_name($file_name, %opt));
}


######################################################################
# app_name() - returns the name of the app associcated with this drop.
# 

sub app_name
{
 my $drop = shift();
 $drop->{adv}->{app_name} = 
   $drop->map()->to_param($drop->dest(), "app_name")
     if ( !$drop->{adv}->{app_name} );
 return ( $drop->{adv}->{app_name} );
}


######################################################################
# domain_name() - returns the domain name for the app associcated with 
# this drop
# .

sub domain_name
{
 my $drop = shift();
 $drop->{adv}->{domain_name} = 
   $drop->map()->to_param($drop->dest(), "domain_name")
     if ( !$drop->{adv}->{domain_name} );
 return ( $drop->{adv}->{domain_name} );
}


######################################################################
# app_files() - Returns the list of files that are part of the 
# application EAR.
# 

sub app_files
{
 my $drop = shift();
 return ( grep(/^app\//, $drop->file_list()) );
}


######################################################################
# config_files() - Returns the list of config files within the drop.
# 

sub config_files
{
 my $drop = shift();
 return ( grep(/^config\//, $drop->file_list()) );
}


######################################################################
# content_files() - Returns the list of content files within the drop.
# 

sub content_files
{
 my $drop = shift();
 return ( grep(/^content\//, $drop->file_list()) );
}


######################################################################
# db_files() - Returns the list of database files within the drop.
# 

sub db_files
{
 my $drop = shift();
 return ( grep(/^db\//, $drop->file_list()) );
}


######################################################################
# app_name() - Returns the list of documentation files in the drop.
# 

sub doc_files
{
 my $drop = shift();
 return ( grep(/^docs\//, $drop->file_list()) );
}


######################################################################
# source_files() - Returns the list of source code files in the drop.
# 

sub source_files
{
 my $drop = shift();
 return ( grep(/^src\//, $drop->file_list()) );
}


######################################################################
# app_name() - Returns true if the app will require a restart after
# the drop files are deployed.
# 

sub requires_restart
{
 my $drop = shift();

 foreach my $file ( $drop->file_list() ) {
   return (1) if 
     $file =~ /RESTART$/ or
     $file =~ /\.ejb$/ or
     $file =~ /\.jar$/ or
     $file =~ /(web|weblogic)\.xml$/ or
     $file =~ /^config\//;
 }
 return (0);
}


1;
__END__


