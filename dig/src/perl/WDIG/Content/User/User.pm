
# $Id$

=head1 NAME

WDIG::Content::User - Object representation of a user

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/27

=head1 SYNOPSIS

 use WDIG::Content::User;

 my $user1 = new WDIG::Content::User($name, $email, $passwd, @urls);

 my $user_xml = "$user1";

 my $user2 = new WDIG::Content::User($user_xml);

 my $user3 = $user2->new();

=head1 DESCRIPTION

Object representation of a content user.  The following methods
are implemented.

=over 4

=item new()

Creates a new User object.  If passed more than one argument 
C<new($name, $email, $passwd, @urls)> is expected.  If only one 
argument is specified it must be either a reference to another
User object or an argument that XML::Simple will accept, such
as an XML string or a file name - e.g. C<new("user.xml")>.
If called as an instance method no argument need be given
and the current object will be copied, C<$user->new()>.

=item as_xml()

Converts the user to XML and returns that string.  Evaluation
of a User object in string context will call this method.

=item name($name)

Returns and optionally sets the user name.

=item passwd($passwd)

Returns and optionally sets the user's password.

=item email($email)

Returns and optionally sets the user's email address.

=item url_list(@url_list)

Returns and optionally sets the list of URLs for which 
the user has access.  

=back

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: User.pm,v $
 Revision 1.1  2003/06/30 02:39:00  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Content::User;

use 5.006_001;
use strict;
use warnings;

our @ISA = qw();
our $VERSION = '1.0';

######################################################################

use Carp;
use URI;
use XML::Simple qw(:strict);
use Mail::Sendmail qw(sendmail %mailcfg);

use overload '""' => 'as_xml';

$mailcfg{from} = 'weblogic@intlappsdev';


######################################################################
# new($user||$user_xml) / new($name,$email,$passwd,@urls) - 
# User object constructor; returns new object.
#

sub new
{
 my $self = shift();
 my $arg = shift();
 my $package = ref($self) || $self;

 $arg = $self if ( !$arg and ref($self) );

 my $user = {};
 bless($user, $package);

 if ( @_ ) {
 
   $user->name($arg);
   $user->email(shift());
   $user->passwd(shift());
   $user->url_list(@_);
   
 } elsif ( UNIVERSAL::isa($arg, $package) ) {

   $user->name($arg->name());
   $user->email($arg->email());
   $user->passwd($arg->passwd());
   $user->url_list($arg->url_list());

 } elsif ( $arg ) {
 
   my $data;

   croak("Unable to parse '$arg' as XML: $@\n")
     if ( ! eval { $data = XMLin($arg, ForceArray=>['file'], KeyAttr=>{}) } );

   $user->name($data->{name});
   $user->email($data->{email});
   $user->passwd($data->{password});
   $user->url_list($data->{url});
   
 } else {

   croak('Usage: new($user||$user_xml) or new($name,$email,$passwd,@urls)');

 }


 return ($user);
}


######################################################################
# as_xml() - converts the data structure to XML and returns that string.
#

sub as_xml
{
 my $user = shift();
 my $xml = undef;

 croak("Unable to convert to XML: $@\n")
   if ( ! eval { $xml = XMLout($user->{data}, RootName=>"user", KeyAttr=>{}) } );

 return ($xml);
}


######################################################################
# name($name) - Returns and optionally sets the user name
#

sub name
{
 my $user = shift();
 my $name = shift();
 
 croak("Cannot use null string as username") if ( defined($name) and !$name );

 return ($user->{data}->{name} = $name || $user->{data}->{name});
}


######################################################################
# passwd($passwd) - Returns and optionally sets the user's password
#

sub passwd
{
 my $user = shift();
 my $passwd = shift();
 
 return ($user->{data}->{passwd} = $passwd || $user->{data}->{passwd} || "");
}


######################################################################
# email($email) - Returns and optionally sets the user's email address
#

sub email
{
 my $user = shift();
 my $email = shift();
 
 return ($user->{data}->{email}) if ( !defined($email) );

 if ( $email and $email !~ /$Mail::Sendmail::address_rx/ ) {
   confess("Invalid email address");
 }

 $user->{data}->{email} = $email || "";

 return ($user->{data}->{email});
}


######################################################################
# url_list(@url_list) - Returns and optionally sets the list of URLs
# the user has access to.  ?? Implement access control here ??
#

sub url_list
{
 my $user = shift();
 my @list = ref($_[0]) ? @{$_[0]} : @_;

 foreach my $url ( @list ) {

   $url = new URI($url);
   croak("Invalid URL '$url'") unless ( $url and $url->scheme() and $url->scheme() =~ /^(ftp|http)$/ );
   push( @{$user->{data}->{url}}, $url->as_string() );

 }

 my @url_list = $user->{data}->{url} ? @{$user->{data}->{url}} : ();

 return (@url_list);
}


######################################################################
# send_message() - Send an email to the user
#

sub send_message()
{
 my $user = shift();
 my %args = @_;

 my $success = sendmail(to=>$user->email(), %args)
       or warn($Mail::Sendmail::error);

 return ($success)
}



1;
__END__


