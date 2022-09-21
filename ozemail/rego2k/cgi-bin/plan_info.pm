
package plan_info;

$planFile = "./plans.txt";

sub new
{
 my $pkg = shift();
 my $class = ref($pkg) || $pkg;
 my $self = {};
 my $cgi = shift();

 bless($self, $class);

 $self->{'plans'} = [];

 $self->getPlanInfo($cgi);

 return ($self);
}# new


sub getPlanInfo
{
 my $self = shift();
 my $cgi = shift();

 open(planFile,"<$planFile") || die("Could not open plan file. $!");
 while (<planFile>){
   my $plan = {};
   ($$plan{'name'},$$plan{'descr'}) = split(/:/);
   $$plan{'checked'} = ($cgi->param('plan') eq $$plan{'name'})?"CHECKED":"";
   push(@{$self->{'plans'}},$plan);
 }
 close(planFile);

 return (1);
}# getPlanInfo


sub list
{
 my $self = shift();
 return ($self->{'plans'});
}# list


sub count
{
 my $self = shift();
 return ($#{$self->{'plans'}}+1);
}# count

1;
