##!/usr/bin/perl
# use ^ normally
#!/usr/local/bin/perl
# use ^ on FreeBSD
# Robert Sato
# May 15, 2019

use strict;
use warnings;
use Getopt::Long;

my $verbose;

GetOptions(
   "v" => \$verbose) or die "Incorrect usage!\n";

if($verbose) {
   print "verbose flag set\n";
}

my $date = localtime();
print "Output data: ";
print "Collected on $date\n";

open(SYSCTL_INFO, "sysctl -a | grep vm |");

while (my $line = <SYSCTL_INFO>) {
   my @split = split(': ', $line);
   my $value = $split[1];
   if ($line =~ /v_free_count/) {
      print "Number of pages in free queue\n" if($verbose);
      print "Free count = $value";
   }
   elsif ($line =~ /active_count/) {
      print "Number of pages in active queue\n" if($verbose);
      print "Active count = $value";
   }
   elsif ($line =~ /inactive_count/) {
      print "Number of pages in inactive queue\n" if($verbose);
      print "Inactive count = $value";
   }
   elsif ($line =~ /wire_count/) {
      print "Number of pages in wire queue\n" if($verbose);
      print "Wire count = $value";
   }
   elsif ($line =~ /laundry_count/) {
      print "Number of pages in laundry queue\n" if($verbose);
      print "Laundry count = $value";
   }
   elsif ($line =~ /pages_scanned/) {
      print "Number of pages scanned\n" if($verbose);
      print "Scanned count = $value";
   }
   elsif ($line =~ /freelist_odd/) {
      print "Number of odd pages added to the rear of freelist\n" if($verbose);
      print "Odd free count = $value";
   }
   elsif ($line =~ /freelist_even/) {
      print "Number of even pages added to the front of freelist\n" if($verbose);
      print "Even free count = $value";
   }
   elsif ($line =~ /random_counter/) {
      print "Number of random activity count pages initialized\n" if($verbose);
      print "Random activity set count = $value";
   }
   elsif ($line =~ /subtract_count/) {
      print "Number of div 2 - 1 pages added to the front of the active list\n" if($verbose);
      print "/2-1 count = $value";
   }
   elsif ($line =~ /random_count_greater/) {
      print "Number of pages added to front of the inactive queue bc r > 7" if($verbose);
      print "r > 7 count = $value";
   }
   elsif ($line =~ /random_count_less/) {
      print "Number of pages added to rear of the inactive queue bc r <= 7" if($verbose);
      print "r <= 7 count = $value";
   }
}
print "Done printing data\n";

close(SYSCTL_INFO);
