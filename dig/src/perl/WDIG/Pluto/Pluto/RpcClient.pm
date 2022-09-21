
# $Id$

=head1 NAME

WDIG::Pluto::RpcClient - XML RPC interface to Pluto

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/08/14

=head1 SYNOPSIS

 use WDIG::Pluto::RpcClient;

 my $pluto = new WDIG::Pluto::RpcClient();

 my $project = $pluto->get_project_data($project_id);
 print $project->{name};
 
 my $user = $pluto->get_user_data($username);
 print $user->{mainContactEmail};


=head1 DESCRIPTION

Provides an RPC interface to certain Pluto functions.
The constructor takes a single option, C<server>,
which should specify the Pluto RPC server URL.

The following functions are implemented:

=over 4

=item create_user_alert(%user_alert)
 
Adds a user alert to the users' pluto mailbox.  The
hash should contain C<projectId>, C<subject> & C<message>
elements at a minumum.  In this case an alert is
created for all users associated with the last drop
of the specified project.  You can also provide 
C<dropVersion>, C<username> & C<actionRequired> elements 
in the hash. The return value should be 1 on success.
 
=item get_project_data($project_id)

Returns a hash containing properties and values
pertaining to the specified project.  The following
elements should be present.

 projectId                     
 name                          
 description                   
 siteName                      
 path                          
 developmentHostname           
 stagingHostname               
 liveHostname                  
 producer                      
 developer                     
 type                          
 continuous                    
 scheduledUpdateFrequency      
 archivingPeriod               
 businessUnitName              
 businessUnitContactDetails    
 dateCreated                   
 dateLiveTarget                

=item get_drop_data($project_id,$drop_version)
 
Returns a hash containing properties and values
pertaining to the specified drop. The following
elements should be present.
 
 projectId         
 version           
 projectName       
 dropName          
 description       
 status            
 archived          
 deploymentType    
 droppedDate       
 droppedBy         
 qaDate            
 qaBy              
 producerDate      
 producerBy        
 liveDeployDate    
 liveDeployBy      
 liveRemoveDate    
 liveRemoveBy      

=item get_last_drop($project_id)
 
As for get_drop_data() but the last drop for the
specified project is used.

=item get_user_data($username)
 
Returns a hash containing properties and values
pertaining to the specified user. The following
elements should be present.

 username                 
 mainContactName          
 mainContactEmail         
 secondaryContactName     
 secondaryContactEmail    
 role                     
 phone                    
 fax                      
 mobilePhoneNumber        
 postalAddress            
 company                  
 country                  
 regions                  
 usergroups               

=back

=head1 ERROR HANDLING

Each method will throw an Error::Simple on encountering
an exception.  This may be the message string from an
exception on the RPC server.

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: RpcClient.pm,v $
 Revision 1.1  2003/08/26 22:53:58  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

package WDIG::Pluto::RpcClient;

use 5.006_001;
use strict;
use warnings;

our @ISA = qw(Exporter);
our $VERSION = '1.0';

######################################################################

use Error;
use Log::Log4perl;
use Data::Dumper;
use RPC::XML::Client;

my $Default_Pluto_Server = 'http://pluto.disneyinternational.com/rpc';


######################################################################
# new(%options) - RpcClient constructor
#

sub new
{
 my $self = shift();
 my %opt = @_;
 my $package = ref($self) || $self;
 my $rpc = {};
 
 %{$rpc} = %opt;
 $rpc->{server} ||= $Default_Pluto_Server;
 $rpc->{logger} = Log::Log4perl->get_logger();
 
 bless($rpc, $package);
 
 return ($rpc);
}


######################################################################
# create_user_alert(%args) - adds a user alert to the users' pluto 
# mailbox.
#

sub create_user_alert
{
 my $rpc = shift();
 my %user_alert = @_;
 my $project;
 my $drop;
 my @user_list = ( $user_alert{username} );

 if ( !$user_alert{projectId} ) {
   throw Error::Simple("user alert must contain project_id element");
 }

 if ( !$user_alert{dropVersion} ) {
   $drop = $rpc->get_last_drop($user_alert{projectId});
   $user_alert{dropVersion} = $drop->{version};
 }

 if ( !$user_alert{username} ) {
   $project = $rpc->get_project_data($user_alert{projectId});
   $drop = $rpc->get_drop_data($user_alert{dropVersion}) if ( !$drop );
   @user_list = ( $project->{developer}, $drop->{droppedBy}, $drop->{qaBy}||0, $drop->{producerBy}||0 );
   { my %uniq; @uniq{@user_list}=(); @user_list=grep($_,keys(%uniq)); }
   $user_alert{projectName} = $project->{name};
   $user_alert{siteName} = $project->{siteName};
 }

 foreach my $user ( @user_list ) {
   $user_alert{username} = $user;
   $rpc->call("pluto.createUserAlert", \%user_alert);
 }

 return (1);
}


######################################################################
# get_drop_data($project_id,$drop_version) - gets some information
# on the specified drop.
#

sub get_drop_data
{
 my $rpc = shift();
 my $project_id = shift();
 my $drop_version = shift();
 my %args = @_;

 return ($rpc->call("pluto.getDropData", $project_id, $drop_version, %args));
}


######################################################################
# get_last_drop($project_id) - gets some info on the last drop made
# for the specified project.
#

sub get_last_drop
{
 my $rpc = shift();
 my $project_id = shift();
 my %args = @_;

 return ($rpc->call("pluto.getLastDrop", $project_id, %args));
}


######################################################################
# get_project_data($project_id) - gets project details for the
# specified project.
#

sub get_project_data
{
 my $rpc = shift();
 my $project_id = shift();
 my %args = @_;

 return ($rpc->call("pluto.getProjectData", $project_id, %args));
}


######################################################################
# get_user_data($usename) - gets details for the specified user.
#

sub get_user_data
{
 my $rpc = shift();
 my $user_id = shift();
 my %args = @_;

 return ($rpc->call("pluto.getProjectData", $user_id, %args));
}


######################################################################
# call($name, @args) - calls the named remote procedure with the given
# arguments and returns the result.  An Error is thrown if anything
# goes wrong.
#

sub call
{
 my $rpc = shift();
 my $name = shift();
 my @args = @_;

 $rpc->{client} = new RPC::XML::Client($rpc->{server}) if ( !$rpc->{client} );

 my $response = $rpc->{client}->send_request($name, @args);

 local ($Data::Dumper::Indent, $Data::Dumper::Terse) = (0, 1); 
 $rpc->{logger}->debug("$name(".Dumper(\@args).") = ".Dumper($response)) if ( $rpc->{logger}->is_debug() );

 if ( !ref($response) or $response->is_fault() ) {
    throw Error::Simple("RPC Error: ".(ref($response)?$response->string():$response));
 }

 return ($response->value());
}

1;
__END__
