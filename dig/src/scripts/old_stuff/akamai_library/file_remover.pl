#!/usr/local/bin/perl

open (FH, "more @ARGV |");
@lines = <FH>;
close (FH);

foreach $line ( @lines) {
	print "deleting $line";
	#$line =~ s/ /\ /g;
	chop $line;
	$macommandealacon="rm -Rf \"".$line."\"";
	system $macommandealacon ;
}

