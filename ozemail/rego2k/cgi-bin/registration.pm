
package registration;

sub new
{
 my $pkg = shift();
 my $class = ref($pkg) || $pkg;
 my $self = {};
 my $cgi = shift();

 bless($self, $class);

 $self->{'cgi'} = $cgi;

 # Would actually do the registration here!!!

 return ($self);
}# new


# if we've got a name we'll consider it successful! 8-)
sub success
{
 my $self = shift();
 return ($self->{'cgi'}->param('name'));
}# success

sub error
{
 my $self = shift();
 return ("You did not enter your name!");
}# error

sub plan
{
 my $self = shift();
 return ($self->{'cgi'}->param('plan'));
}# plan

1;
