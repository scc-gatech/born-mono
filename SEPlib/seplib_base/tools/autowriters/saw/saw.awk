BEGIN {
 inmain = 1
 doc = 0
 enddoc = 0
 init=1
 implstr= "\timplicit none"
 }
NF ==0 {
 print $0
 next
 }
$1 ~ /define|^\%/ {
 print $0
 next
 }
(init == 1 && /.*(subroutine|function)/) &&( ! (/^c.*(subroutine|function)/ || /.*#.*(subroutine|function)/)) {
 inmain = 0
 }
 (/^[#c]/ && /\%/ && enddoc==0 && inmain==1) {
 print $0
 enddoc=1
 init=0
 print "\t\n\tinteger hetch,fetch,getch,tetch,infd,outfd,headfd,auxpar"
 print "\texternal hetch,fetch,getch,tetch,auxpar"
 print "\tinteger lcrpos"
 print "\tinteger fsbrk,input,output,head,idum"
 print "\texternal fsbrk,input,output,head"
 print "\tinteger loc, locore"
 print "\tcharacter*1 basecore(1)"
 print "\tcommon/falloc/basecore\n"
 next
 }
/^c|^C/ {
 print $0
 next
 }
/[\t ]*#/ {
 print $0
 next
 }
inmain==1 && doc==0 {
 init=0
 if(( $1 $2 ~ /^integer|^real|^character|^doubleprecision|^complex|^external|^implicit|^logical|^data|^common|^intrinsic|^parameter|^options|^source/ ) || $0 ~ /^\ \ \ \ \ [^ 0]/) {
  print $0
  next
 }
 else {
  print "\tcall initpar()"
  doc=1
  print "\tcall doc(source)"
  print "\tcall noieeeinterupt()"
  print "\tidum=hetch('Gobbledegook','s','Lets get started.')"
  print "\tlcrpos=1 \n"
  }
    }
/.*(subroutine|function)/ &&( ! (/^c.*(subroutine|function)/ || /.*#.*(subroutine|function)/)) {
 init=0
 if( inmain ==0 )
  {
  print
  print implstr
  print "\tinteger fetch, hetch, tetch, getch, auxpar"
  print "\texternal fetch, hetch, tetch, getch, auxpar"
  }
 else {
  inmain = 0
  call = "\tcall " $2
  for(j=3; j<=NF; j++) {
   if("" != alloctab[$j])
    call = call "basecore(l" $j "-locore)"
   else
    call = call $j
   }
  print "\tlocore=fsbrk(lcrpos-1)"
  print "\tlocore=loc(basecore)-locore"
  print call
  print "\tcall exit(0)"
  print "\tend"
  subro = "\n\tsubroutine "
  for(i=2; i<=NF ; i++)
   subro=subro $i
  print subro
  print implstr
  print "\tinteger fetch, hetch, tetch, getch, auxpar"
  print "\texternal fetch, hetch, tetch, getch, auxpar"
  }
 next
 }
$1 ~ /^allocate$/ {
 init=0
 if( inmain==0 )
  print "Saw can't allocate in a subroutine"
 if( $3 ~ /^(integer|real|logical)/ )
        { esize=4 }
 else if( $3 ~ /^character/ )
 { esize=1 }
 else if( $3 ~ /^(complex|doubleprecision)/ )
        { esize=8 }
 if( $4 == "*" ) {
   esize=$5
   js=6
         }
 else
   js=4
 for( j=js ; j <=NF; j++) {
  if($j == "(" ) {
   inest = 1
   while(inest != 0 ) {
    j++
    if(j > NF) {
     print "SYNTAX ERROR: too few )'s"
     inest = 0
     }
    if( $j == ")" ) inest--
    if( $j == "(" ) inest++
    if( $j == "," )
     alloc = alloc ")*("
    else
     alloc = alloc $j
    }
   alloc = alloc ")*" esize
   print alloc
   print "\tlcrpos = 1 + (((lcrpos-1)+7)/8)*8"
   j++
   }
  else if($j != "," ) {
   alloctab[$j] = $j
   print "\tl" $j " = lcrpos"
   alloc = "\tlcrpos = lcrpos + (("
   }
  }
 next
 }
$1 ~ /^from$/ && ($2 ~ /^conj$/ || $2 ~ /^adj$/) {
        if ($2 ~ /^conj$/ )
             conjadj = "conj"
        else
             conjadj = "adj"
 init=0
 j = 4
 if( $j == "integer") type = "i"
 else if($j == "real") type = "r"
 else if($j == "character") type = "s"
 else if($j == "logical") type = "l"
 else if($j == "doubleprecision") type = "g"
 else type = "SYNTAX t: Check commas."
 j++
 while(j <= NF) {
  x = $j; j ++
                if( conjadj == "conj" )
   from = from "\t { \n if(conj==inv) {\n"
                else
                        from = from "\t { \n if(adj==inv) {\n"
  l1 = "\tif(hetch('" x "','" type "',"
  l2 = "\tif(tetch('" x "','" type "',"
  l4 = "\tif(hetch('" x "','" type "',"
  l5 = "\tif(tetch('" x "','" type "',"
                for(ie=j; ie<= NF; ie++) { # seek end of subfield.
                        if( $ie == "(" )
                                while( $ie != ")" && ie <= NF )
                                        ie++;
                        if( $ie == "," || $ie == ";" )
                                break;
                        }
  if($j != ":") from = from "SYNTAX ERR"
  j++; y = $j
  j++; yd = j
  while($j != ":") j++
  j++; z = $j
  j++; zd = j
  l1 = l1 y ").eq.0)\t"
  l2 = l2 z ").eq.0)\t"
  l4 = l4 z ").eq.0)\t"
  l5 = l5 y ").eq.0)\t"
  l1 = l1 "call erexit('hetch " x ":" y "')"
  l4 = l4 "call erexit('hetch " x ":" z "')"
  l3 = "\tcall putch('" z " = " x "','" type "'," z ")"
  l6 = "\tcall putch('" y " = " x "','" type "'," y ")"
  if($zd == "=") {
   l2 = l2 z
   for(i=zd; i<ie; i++)
    l2 = l2 $i
   }
  else
   l2=l2 "call erexit('tetch " x ":" z "')"
  if($yd == "=") {
   l5 = l5 y
   for(i=yd; $i!=":"; i++)
    l5 = l5 $i
   }
  else
   l5=l5 "call erexit('tetch " x ":" y "')"
  from = from l1 "\n" l2 "\n" l3 "\n\t}\nelse {\n"
  from = from l4 "\n" l5 "\n" l6 "\n\t}\n"
  j = ie+1
  }
 from = from " \n \t } "
 print from
 from = ""
 next
 }
$1 ~ /^from$/ {
 init=0
 if( $2 ~ /^history$/) place = "hetch"
 else if($2 ~ /^par$/) place = "getch"
 else if($2 ~ /^aux$/) place = "auxpar"
 else if($2 ~ /^either$/) place = "fetch"
 else if($2 ~ /^tetch$/) place = "tetch"
 else place = "SYNTAX ERRORl: Check commas."
 if($2 ~ /^aux$/) {
  j = 5
  file = ",'" $4 "'"
  }
 else {
  file = ""
  j = 4
  }
 if( $j ~ /^integer/) type = "i"
 else if($j ~ /^real/) type = "r"
 else if($j ~ /^character/) type = "s"
 else if($j ~ /^logical/) type = "l"
 else if($j ~ /^doubleprecision/) type = "g"
 else if($j ~ /^boolean/) type = "1"
 else type = "SYNTAX ERRORt: Check commas."
 j++
 while(j <= NF) {
  from = from "\tif(" place "('" $j "','" type "',"
                for(ie=j; ie<= NF; ie++) { # seek end of subfield.
                        if( $ie == "(" ) {
                                while( $ie != ")" && ie <= NF ) ie++;
                                }
                        if( $ie == "," || $ie == ";" ) break;
                        }
  x = $j
  j ++
  if($j == ":") {
  y=""
                        for(iey=j+1; iey<= NF; iey++) {
                                if( $iey == "(" ) {
                                        while( $iey != ")" && iey <= NF ) {
                                                iey++;
                                                y = y $iey
                                                }
                                        }
                                if( $iey == "," || $iey == ";" || $iey == "=" )
                                        break;
                                y = y $iey
                                }
   j = iey
   }
  else
   y = x
  from = from y file ").eq.0)\t"
  if($j == "=") {
   from = from y
   for(i=j; i<ie; i++)
    from = from $i
   }
  else
   from = from "call erexit('need " x ":" y "')"
  from = from "\n"
  if( place == "getch" )
  from=from "\tcall putch('From par: " y " = " x "','" type "'," y ")" "\n"
  if( place == "fetch" )
  from=from "\tcall putch('From either: " y " = " x "','" type "'," y ")" "\n"
  if( place == "auxpar" )
  from=from "\tcall putch('From aux(" $4 "):" y ":" x ": " $4 "_" x"','" type "'," y ")" "\n"
  j = ie+1
  }
 print from
 from = ""
 next
 }
$1 ~ /^to$/ {
 init=0
 if($2 ~ /^aux$/) {
 j = 5
 file = ",'" $4 "'"
 if( $j ~ /^integer/) type = "i"
 else if($j ~ /^real/) type = "r"
 else if($j ~ /^character/) type = "s"
 else if($j ~ /^logical/) type = "l"
 else if($j ~ /^doubleprecision/)type = "g"
 else type = "SYNTAX ERRORt: Check commas."
 j++
 while(j <= NF) {
                for(ie=j; ie<= NF; ie++) { # seek end of subfield.
                        if( $ie == "(" )
                                while( $ie != ")" && ie <= NF )
                                        ie++;
                        if( $ie == "," || $ie == ";" )
                                break;
                        }
  x = $j
  j ++
  if($j == ":") {
   j++
   if($j== "'")
    j++
   y = $j
   j++
   for(; j<ie ; j++) {
    if($j != "'")
     y = y " " $j
    }
   extra = y " = "
   }
  else {
   y = x
   extra = ""
   }
 print "\tcall auxputch('" extra x "','" type "'," y file ")"
  j = ie+1
  }
  }
 else {
 j = 4
 if( $j ~ /^integer/) type = "i"
 else if($j ~ /^real/) type = "r"
 else if($j ~ /^character/) type = "s"
 else if($j ~ /^logical/) type = "l"
 else if($j ~ /^doubleprecision/)type = "g"
 else type = "SYNTAX ERRORt: Check commas."
 j++
 while(j <= NF) {
                for(ie=j; ie<= NF; ie++) { # seek end of subfield.
                        if( $ie == "(" )
                                while( $ie != ")" && ie <= NF )
                                        ie++;
                        if( $ie == "," || $ie == ";" )
                                break;
                        }
  x = $j
  j ++
  if($j == ":") {
   j++
   if($j== "'")
    j++
   y = $j
   j++
   for(; j<ie ; j++) {
    if($j != "'")
     y = y " " $j
    }
   extra = y " = "
   }
  else {
   y = x
   extra = ""
   }
  print "\tcall putch('" extra x "','" type "'," y ")"
  j = ie+1
  }
 }
 next
 }
  {
 print
 }
