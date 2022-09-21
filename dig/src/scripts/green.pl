#!/usr/bin/perl

#####################################################################
#
# green.pl
#
# This file contains perl routines to encrypt and escape a 
# green cookie - as well as the routines to unescape and decrypt.
# 
# Also included are the utility functions to encode and unencode 
# the commerce token mask by mapping integer values to HEX char 
# values.
#
# Functions:
#	getEncryptionKey
#	cryptXOR
#	hexEscape
#	hexUnEscape
#       encodeHexMask
#       decodeHexMask    
#       escapeUserInfo
#       unescapeUserInfo
#   
# Author: Tom Laramee
# Date: 10/7/98
# Perl verison: Version 5 build 108 for Windows NT 4.0
#
#####################################################################

$secretKey;




#####################################################################
#
# getEncryptionKey
#
# Quick and dirty simulation of an encryption key store.  The 
# parameter to pass is the index of the encryption key desired.  
#
# (Need 16 to test HEX byte value in green format) 
#
# The return value is the encryption string.
#
#####################################################################
sub getEncryptionKey
{
    local ($idx) = @_;
    local (@keys);
    @keys = (
	 "&n'+,#'/*rv{}qgq1w+/w#cdnr/+,{}r/*de}68kik,+,/*{*25+,/w2256{%+,/w#q#n+,/#",
	 "#n+,/+k#;*+,/'r:'d*'3,}{w+K w'K:'+}e#';dq#'lq#'+d'K#!/+k#;{l,+,/n{n+,/+xq",
	 "wknw''{[iwk{KK{nl]!/w{%'l##w#'i:{nl]'/*{q#'ld;r'}{nlwb!/*de}'cdsl;AQ}/?.;",
	 "ek;dci&bK'(q)-[w]*%n+r3#l,{}:24tca-O;m.vpbksearv790m1`11569iry#j&160l<|.",
	 "dxgjidrjfigurei?}{g/45c'${?};sotle9t958iojhgikeqerf790[.pp0nyq4%4P{}?:>s",
	 "plsfg45dbmo.8e.'Ph'nglui2wrbt,1`AERG67_m4ulhu_R'l47yeh_33737wag2D3wf.f,.",
	 "sahgklfaghf'klgafs)~';i\52T#'lfhj'lghal'kfgshalfhglkfdhg'hkl.u90p7i4Y#%U",
	 "-22&&d%80-78]);f(!c-1839++c,is/s,/x,y,b);k/--i(!(c-184)iY&^x131$#&*b46^x",
	 "11+(a+f)%80,'',(+a+f)/80+1,1+(f+a+f)%80,'n(f+a+f);k;{{{.hjyu786&(*_h/w2g",
	 "p(asi(bcb(ao#j&]57%ce167(beq(ag'[i!=[105)176't141n(0145170i1640511S!&!&E",
	 "-s;v=a[j*=v]-a[i],k=i<s,j+=(v=j<s&&(!k&&!!printf(2%c-(!l<<!j),??dsafv#2Q",
	 "[l^v?(l^j)12])&:M'$#&*Ydsuf98425htpe9$w[i]<s&&v&&v-iaeb09lgnm'|3321r42%#^",
	 "w;r=d;}f;;{B(3),y);!247b()||(w=q34tf(l))<1)ddit(0);if(w&1||w>M||nw>y)B(1)",
	 "*x,*y,m,t;S(d,v,f,_,a,b)l*v;{l c,v+100,j=d<u-1?a:-900,wizig06111[E/51~/4]",
	 "732kjhdf8jvm*f&^t-c-l{:>?~``akj'<fdsdf}ds236+|,3+!~=%&^,-12,-1,12,dafvuw1",
	 "};[E)~'8/2]{i*T#j[E)~'2*[i*T#iY&^x]]}{$j<{p>30};145ss170i164ccxasxsrv4y=-" 
	);

    if ( ($idx>=0) && ($idx<=15 ) )
    {
        return ( $keys[$idx] );
    }
    else
    {
        return ( $keys[0] );
    }
}


#####################################################################
#
# cryptXOR
#
# A Function to perform encryption.  The parameter to pass is the 
# string to be encrypted.  The encryption key  is global (for now).
#
# The return value is the encrypted string.
#
#####################################################################
sub cryptXOR
{
    local ($in) = @_;
    local ($iSecret) = length($secretKey);
    local (@chars) = split('',$in);
    local (@key) = split('',$secretKey);
    local ($ch,$sk,$c,$ret);
    local $p=0;

    foreach $ch (@chars) 
    {
        if ($p >= $iSecret) 
        {
            $p = 0;
        }

        $sk = @key[$p];
        $c = $ch ^ $sk;
        $p++;
	$ret .= $c;
    }
    return $ret;
}

#####################################################################
#
# hexEscape
#
# A function to perform HEX escaping of encrypted characters. 
# Each character in the input string is mapped to 2 HEX 
# characters of equivalent value.
#
# The parameter to pass is the string to be escaped.  The 
# return value is the HEX escaped string.
#
#####################################################################
sub hexEscape
{
    local ($in) = @_;
    local ($val1, $val2, $ch, $ret);

    foreach $ch (split('',$in))
    {
        $val1 = ord( $ch );            # get integer value of ch
	$val2 = sprintf("%2X",$val1);  # convert to HEX
	$val2 =~ s/ /0/g;              # replace ' ' with '0' 
	$ret .= $val2;                 # append to @ret
    }

    return $ret;
}


#####################################################################
#
# hexUnEscape
#
# 17:     # Convert %XX from HEX numbers to alphanumeric
# 18:     $key =~ s/%(..)/pack("c",hex($1))/ge;
# 19:     $val =~ s/%(..)/pack("c",hex($1))/ge;
#
# A function to perform unescaping of HEX characters
#
# The parameter to pass is the string to be unescaped.  The 
# return value is the original string (before it was escaped).
#
#####################################################################
sub hexUnEscape
{
    local ($in) = @_;
    local ($cLen) = length($in);
    local ($i, $val1, $ret );

    for ( $i=0; $i<$cLen; $i+=2 )
    {
        # Get 2 characters,  convert to HEX, pack into a char
        $val1 = pack( "c", hex(substr($in,$i,2)) );

        # APpend the char to the ret value
	$ret .= $val1;                 
    }

    return $ret;
}




#####################################################################
#
# encodeHexMask
#
# Given an ID,  and an old mask,  encode the ID in the mask 
# and return a new mask.
#
# The 2 parameters to pass in are the ID to encode and the previous 
# mask.  The function returns NULL on error or the new mask on 
# no error.
#
#####################################################################
sub encodeHexMask
{
    local( @in, @chars ) = @_;
    local( $cArgs, $cId, $cNeeded, $cLen, $cZeroes );
    local( $szOldMask, $szNewMask );
    local( $i, $n, $nOld, $ch );

    $cArgs     = @in;

    # Need 2 args...
    if ( 2 != $cArgs ) { return NULL; }

    # The ID is the 1st arg
    $cId = @in[0];

    # The mask is the 2nd arg
    $szOldMask = @in[1];


    # ID is invalid...
    if ( $cId<=0 ) { return NULL; }

    # How many digits are needed for the mask?
    $cNeeded = int( ( (($cId - 1) / 4) + 1 ) );

    # Reverse the old mask to begin the new one
    $szNewMask = reverse( $szOldMask );
    $cLen = length( $szNewMask );

    #
    # Case 1:  The mask is not large enough to encode the Id
    #
    if ( $cLen < $cNeeded ) {

        # Determine how many zeroes are needed 
        $cZeroes = int( ($cId - 1) / 4 - $cLen );

        # Pad the mask with zeroes
        for ( $i=0; $i<$cZeroes; $i++ ) { 
            $szNewMask .= "0"; 
        }

        # Add the approprate character to the mask 
        FU: {
            if ( 0==($cId%4) ) { $szNewMask .= '8'; last FU; }
            if ( 1==($cId%4) ) { $szNewMask .= '1'; last FU; }
            if ( 2==($cId%4) ) { $szNewMask .= '2'; last FU; }
            if ( 3==($cId%4) ) { $szNewMask .= '4'; last FU; }
        }
    }
    #
    # Case 2:  The mask is large enough to encode the Id
    #
    else
    {
        # Access individual characters of the new mask
        @chars = split('', $szNewMask );

        # Retreive character to be XORed with the new int mask value
        $n = $nOld = hex( $chars[ $cNeeded-1 ] );

        # XOR the old char with the new id's HEX value 
        BAR: {
            if ( 0==($cId%4) ) { $n |= 8; last BAR; }
            if ( 1==($cId%4) ) { $n |= 1; last BAR; }
            if ( 2==($cId%4) ) { $n |= 2; last BAR; }
            if ( 3==($cId%4) ) { $n |= 4; last BAR; }
        }

        # If it's changed,  set the new value in the mask
        if( $nOld != $n ) 
        {
            $ch = sprintf( "%X", $n );
            $chars[ $cNeeded-1 ] = $ch;
        }
    
        # Convert the character array to a string to return it
        $n=@chars;
        $szNewMask = "";

        for ( $i=0; $i<$n; $i++ ) {
            $szNewMask .= $chars[$i]; 
        }
    }

    # Need to reverse the mask before returning it
    $szOldMask = reverse( $szNewMask );

    return ( $szOldMask );
}


#####################################################################
#
# decodeHexMask
#
# Given an ID,  and a HEX mask,  determine if the ID has been 
# encoded in the mask or not.
#
# The 2 parameters to pass in are the ID to encode and the previous 
# mask.  The function returns NULL on error,  1 if the ID was 
# found,  0 if not.
#
#####################################################################
sub decodeHexMask
{
    local( @in, @chars ) = @_;
    local( $cArgs, $cId, $cNeeded, $cLen, $cIdHexMask, $cResult, $cVal );
    local( $ch );

    $cArgs     = @in;
    $cId       = @in[0];
    $szOldMask = @in[1];

    # Need 2 args...
    if ( 2 != $cArgs ) { return NULL; }

    # ID is invalid...
    if ( $cId<=0 ) { return NULL; }

    # How many digits are needed for the mask?
    $cNeeded = int( ( (($cId - 1) / 4) + 1 ) );
    $cLen = length( $szOldMask );

    # Not enough digits in mask to have encoded the ID 
    if ( $cLen<$cNeeded ) { return 0; }

    BAR: {	
        if ( 0==($cId%4) ) { $cIdHexMask |= 8; last BAR; }
        if ( 1==($cId%4) ) { $cIdHexMask |= 1; last BAR; }
        if ( 2==($cId%4) ) { $cIdHexMask |= 2; last BAR; }
        if ( 3==($cId%4) ) { $cIdHexMask |= 4; last BAR; }
    }
        
    # Need to access individual characters of the new mask

    # Grab the character to compare... read mask from right to left 
    $ch = $chars[ $cLen - $cNeeded ];

    # Convert it tto a decimal number
    $cVal = hex( $ch );

    # AND it with the Ids HEX mask digit
    $cResult = ( $cVal & $cIdHexMask);

    # printf ("decodeHexMask> id[%2d]  mask[%8s]  cLen[%d] cNeeded[%d]  ch[%d]  cVal[%2d]  cResult[%2d]\n", $cId, $szOldMask, $cLen, $cNeeded, $ch, $cVal, $cResult);
    if ( $cResult==$cIdHexMask ) {
        return 1;
    }
	
    return 0;
}



#####################################################################
#
# escapeUserInfo
#
# Given a string,  replace instances of [';', ',', '=', '%'] 
# with [ %3B, %2C, %3D, %25],  but only after the first occurance 
# of the "=" character.
#
# Char  Int  Hex
# ===============
#  =    61   3D
#  ,    44   2C
#  ;    59   3B
#  %    37   25
#  
# (there has to be an easier way to do this...)
#
#####################################################################
sub escapeUserInfo
{
    local ($in) = @_;
    local (@chars, @nv);
    local ($cLen, $i, $foundEquals, $ret );

    $foundEquals = false;
    $ret = "";

    @chars = split('',$in);
    $cLen = length($in);


    for ( $i=0; $i<$cLen; $i++ )
    {
	$val1 = ord( $chars[$i] );    # get integer value of ch

        if ( ($val1 eq 61) && (false eq $foundEquals) )
        {
           $foundEquals = true;
           $ret .= "=";
        }     
        else 
        {
            if ( (61 eq $val1) || (44 eq $val1) || (59 eq $val1) || (37 eq $val1) )
            {
                 BAR: 
                 {
                     if ( 61 eq $val1 ) { $ret .= "%3D"; last BAR; }
                     if ( 44 eq $val1 ) { $ret .= "%2C"; last BAR; }
                     if ( 59 eq $val1 ) { $ret .= "%3B"; last BAR; }
                     if ( 37 eq $val1 ) { $ret .= "%25"; last BAR; }
                 }
            }
            else
            {
                $ret .= $chars[$i];
            }
        }
    }

    if ( $foundEquals eq false ) 
    {
        return null;
    }
    else
    {
        return $ret;
    }
}




#####################################################################
#
# unescapeUserInfo
#
# Given a string,  replace instances of [ %3B, %2C, %3D, %25 ] 
# with [';', ',', '=', '%'] 
#
# Char  Int  Hex
# ===============
#  =    61   3D
#  ,    44   2C
#  ;    59   3B
#  %    37   25
#
# (there has to be an easier way to do this...)
#
#####################################################################
sub unescapeUserInfo
{
    local ($in) = @_;
    local (@chars);
    local ($cLen, $i, $j, $val, $ret);
  
    @chars = split('',$in);
    $cLen = length($in);
    $ret = "";

    for ( $i=0; ( ($i<$cLen) && ($chars[$i] ne '=') ); $i++ )
    {
          $ret .= $chars[$i];
    }

    if ( $i == $cLen )
    {
        return null;
    }

    $ret .= "=";

    for ( $j=$i+1; $j<$cLen; $j++ )
    {
	if ( $chars[$j] eq '%' ) 
        {
	    # Get 2 characters,  convert to HEX, pack into a char
            $val1 = pack( "c", hex(substr($in,$j+1,2)) );

            # Append the char to the ret value
	    $ret .= $val1;  
            $j += 2;
               
        }
        else
        {
            $ret .= $chars[$j];
        }
   }

    return $ret;
}


if ($ARGV[0] =~ /([0-9A-Fa-f]),([0-9A-F]+)/){
  $secretKey = getEncryptionKey(hex($1));
  print cryptXOR(hexUnEscape($2))."\n";
}else{
  $secretKey = getEncryptionKey(0);
  print "0,".hexEscape(cryptXOR($ARGV[0]))."\n";
}
exit(0);

#####################################################################
#
# execution begins here
#
#####################################################################
print "green.pl> begin\n";

$in = "ID=aa,bb;cc%dd=ff";
$out1 = escapeUserInfo($in);
$out2 = unescapeUserInfo($out1);
print "green.pl> Test [$in] [$out1] [$out2] \n";

$in = "foo";
$out1 = escapeUserInfo($in);
$out2 = unescapeUserInfo($out1);
print "green.pl> Test [$in] [$out1] [$out2] \n";

$in = "ID=aa,,,,,,bb;;;;;;cc%%%%%%dd=====ff";
$out1 = escapeUserInfo($in);
$out2 = unescapeUserInfo($out1);
print "green.pl> Test [$in] [$out1] [$out2] \n";

print "green.pl> \n\n";




#
# Hard wire user info
#
$userInfo = "ID=132541;TM=FF0010308;UN=tlaramee;";
print "green.pl> UserInfo is                             : [$userInfo]\n";


#
# Retrieve an encryption key
#
$keyIndex = 13;
$secretKey = &getEncryptionKey( $keyIndex );
print "green.pl> Encryption key is                       : [$secretKey]\n";


#
# Encrypt the user info 
#
$userInfoEnc = &cryptXOR($userInfo);
#print "green.pl> UserInfo encrypted is                   : [$userInfoEnc]\n";


#
# HEX escape the encrypted info 
#
$userInfoEncEsc = &hexEscape( $userInfoEnc );
print "green.pl> UserInfo encrypted and hex escaped is   : [$userInfoEncEsc]\n";


#
# Format a green
#
$green = sprintf( "%X,%s", ($keyIndex), ($userInfoEncEsc) );
print "green.pl> green formatted is                      : [$green]\n";



#
# extract the 2 tokens from the green
#
@green = split(',', $green);


#
# Locate the encryption key
#
$keyIndex = hex( $green[0] );
$secretKey = &getEncryptionKey( $keyIndex );
print "green.pl> Dencryption key is                      : [$secretKey]\n";


#
# un-scape the green info
#
$userInfoEncEsc = $green[1];
$userInfoEncUnEsc = &hexUnEscape( $userInfoEncEsc );
#print "green.pl> UserInfo encrypted and hex unescaped is : [$userInfoEncUnEsc]\n";


#
# decrypt the user info
#
$userInfoDeEnc = cryptXOR($userInfoEncUnEsc);
print "green.pl> UserInfo deencrypted is                 : [$userInfoDeEnc]\n";

print "green.pl> end\n";



#
# Hex encoding/decoding stuff
#
print "green.pl> \n";

for ( $i=1; $i<18; $i++ )
{
	$newMask = &encodeHexMask( $i, "" );
	$bEncoded1 = &decodeHexMask( $i, $newMask );

	$bDecoded2 = 0;
	$bDecoded3 = 0;

        # Try a couple of others for fun
	if ( $i>1 )  { $bEncoded2 = &decodeHexMask( $i-1, $newMask ); }
	if ( $i<18 ) { $bEncoded3 = &decodeHexMask( $i+1, $newMask ); }

	printf ("green.pl> Id: [%2d]   Mask: [%8s]  Encoded?: [%d] [%d] [%d] \n", $i, $newMask, $bEncoded1, $bEncoded2, $bEncoded3 );
}


#
# More HEX encoding/decoding stuff 
#
print "green.pl> \n";
$oldMask = "0";

for ( $i=1; $i<18; $i++ )
{
	$newMask = &encodeHexMask( $i, $oldMask );
	$bEncoded1 = &decodeHexMask( $i, $newMask );

	$bDecoded2 = 0;
	$bDecoded3 = 0;

        # Try a couple of others for fun
	if ( $i>1 )  { $bEncoded2 = &decodeHexMask( $i-1, $newMask ); }
	if ( $i<18 ) { $bEncoded3 = &decodeHexMask( $i+1, $newMask ); }

	printf ("green.pl> Id: [%2d]   Mask: [%8s]  Encoded?: [%d] [%d] [%d] \n", $i, $newMask, $bEncoded1, $bEncoded2, $bEncoded3 );

	$oldMask = $newMask;
}


exit 1;

