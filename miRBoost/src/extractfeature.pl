#!/usr/bin/perl -w

use strict;
use Getopt::Long;

my $help;
my $input;
my $select;
my $output;
my $line;
my @atts;
my @args;

usage() if (($#ARGV<4) or !(GetOptions (
                                'help|?' => \$help,
                                'input=s' => \$input,
                                'select=s' => \$select,
                                'output=s' => \$output
                            ))
            or defined($help));

open(SEL, $select);
$line = <SEL>;
chomp($line);
@atts = split(" ", $line);
close(SEL);

open(IN, $input);
open(OUT, ">$output");
while ($line=<IN>) {
	chomp($line);
	@args = split("\t",$line);
	print OUT "$args[0] ";
	for (my $i=0; $i<@atts; $i++) {
		print OUT "$args[$atts[$i]] ";
	}
	print OUT "\n";
}
close(IN);
close(OUT);
exit;

sub usage {
    print "Unknown option: @_\n" if ( @_ );
    print <<enddocs
extractfeature.pl: extract selected features from all
Usage: perl extractfeature.pl [OPTION]
Options:
 -h,--help,-?
      help
 -i,--input=ALL_FEATURE_FILE
      input file with all features
 -o,--output=SELECTED_FEATURE_FILE
      output file with selected features
 -s,--select=SELECTED_INDEX_FILE
      list of selected feature indices
enddocs
;
    exit()
}
