#!/local/bin/perl
# Package for date handling
#
#     Copyright (c) 1994 Harald T. Alvestrand
#
#     The X Consortium, and any party obtaining a copy of these files from
#     the X Consortium, directly or indirectly, is granted, free of charge, a
#     full and unrestricted irrevocable, world-wide, paid up, royalty-free,
#     nonexclusive right and license to deal in this software and
#     documentation files (the "Software"), including without limitation the
#     rights to use, copy, modify, merge, publish, distribute, sublicense,
#     and/or sell copies of the Software, and to permit persons who receive
#     copies from any such party to do so.  This license includes without
#     limitation a license to do the foregoing actions under any patents of
#     the party supplying this software to the X Consortium.
#
# Use the PERL std library timelocal - older had his own
require "timelocal.pl";
package DATE;
#               J   F   M   A   M   J   J   A   S   O   N   D
@daysinmonth = (31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
# Find timezone (now) by comparing gmtime to localtime
# for a fixed time that is not on a month boundary
$sometime = 150000;
@gmtime = gmtime($sometime);
@localtime = localtime($sometime);
$timezone = $localtime[2]  - $gmtime[2];
# Day changes.
if ($localtime[3] == $gmtime[3]) {
   # do nothing
} elsif ($localtime[3] == $gmtime[3] + 1) {
   $timezone += 24;
} elsif ($localtime[3] == $gmtime[3] - 1) {
   $timezone -= 24;
} else {
   $gm = join(":", @gmtime);
   $lt = join(":", @localtime);
   die "Problems with timezone: comparing $gm to $lt\n";
}



sub leapyear {
   local($y) = @_;
   local($leap4, $leap100, $leapyear);
   $leap4 = $y / 4;
   $leap100 = $y / 100;
   if (int($leap4) == $leap4 && int($leap100) != $leap100) {
      $leapyear = 1;
   }
   $leapyear;
}

sub weekno {
# Given a time_tm, return week number
   local($tm) = @_;
   local($year, $weekno, $dummy);
   ($dummy, $dummy, $dummy,
    $dummy, $dummy, $year,
    $dummy, $yday, $dummy) = localtime($tm);
   $weekno = int(($yday - &firstdayfirstweek($year) + 7) / 7);
   $weekno;
}


sub firstdayfirstweek {
# Return first day of week 1 of any year
   local($y) = @_;
   local($ret);
   # Get time of January 1, 0.0.0.0
   # Note that DST is never in effect on Jan 1...
   local($firsttime) = &main'timelocal(0, 0, 0, 1, 0, $y, 0, 0, 0);
   local(@firstday) = localtime($firsttime);
   local($wday) = $firstday[6];
   # Rule works for some years.....89 to 92 tested, they all hit branch 2...
   if ($wday > 3) {
      $ret = 8 - $wday;
   } else {
      $ret = 1 - $wday;
   }
   $ret;
}
 
sub firstinweek {
# Return timevalue for 2 hours into the day
# (to avoid DST troubles)
   local($weekno, $year) = @_;
   # 2 hours into this year
   local($time) = &main'timelocal(0, 0, 2, 0, 0, $year, 0, 0, 0);
   # Add number of days since start of year
   $time += (($weekno - 1) * 7 + &firstdayfirstweek($year) + 1) * 60 * 60 * 24;
   $time;
}

1; # Everything is fine
