s/^\/\*!50001 //
/^\/\*/d
/^--/d
/^SET/d
y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/
s/`//g
s/ default null//
s/int(11)/integer/
s/^  key /  index /
s/ engine.*;$/;/
s/index \(\w\+\) (\1)/index (\1)/
s/constraint [a-z0-9_]\+ //
s/^\(  index \)[a-z0-9_]\+ /\1/
s/ algorithm=undefined \*\/$//
