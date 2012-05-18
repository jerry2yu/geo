#!/usr/bin/perl

$data_file="StreetSuffixes";

open(DAT, $data_file) || die("Could not open file!");
@raw_data=<DAT>;
close(DAT);


foreach $raw_line(@raw_data)
{
$new_line= qr//;
 # ($w_name,$crowd_re,$fav_move)=split(/\|/,$wrestler);
 #  print "When $w_name is in the ring, the crowd might $crowd_re when the $fav_move is used.";
   print "$raw_line";
 }

