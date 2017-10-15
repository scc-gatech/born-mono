/*
edit history
1985	jon	wrote saw.awk
1986	stew	added self doc, double precision and other stuff
7-23-86	jon	commented out creation of infd.
7-23-86 clem   added { } around conj conditionals
7-25-86	jon	"from conj"  now produces "if(conj==inv)"
7-25-86	jon	restored initializing hetch removed by stew.
7-26-86	jon	erexits of tetch now point to tetch, not getch.
7-29-86 jon	introduce "from tetch"
7-30-86 jon	noticed "to history: character" bug 'literal' vrs variable.
3-07-87 stew    added sreed() and srite() to predeclared functions
07-87 biondo    added reed() and rite() to predeclared functions
		commented sreed() and srite()
		changed for fortran
		changed return in stop at end main program
		added auxputch
11-87 Biondo	updated to saw changes (allocation with fsbrk)
08-88 Biondo	Made it working with both fortran and ratfor
		Changed self-documetation
11-88 Biondo    Introduced variable init for allowing comments at 
		beginning of subroutines.
12-88 Biondo    Commented reed() and rite() because of Sun's problems.
1-89 Biondo     Added Stew's improvements
9-16-89 Biondo	Added "from ... boolean" and some Stew's improvements.
10-28-90 jon	Squeezed superfluous blanks from call doc, 72 column problem.
04-04-91 W. Bauske 
		It appears sawf removes blanks literals in 'from par:'
		statements. ofile=' ' becomes ofile='', a null string.
		ofile=' a ' becomes 'a', the wrong answer. I'm not an
		awk expert and don't have a fix at this time...
8-2-92 Martin   added case for implicit none for DEC fortran compiler
                define DEFFC when compiling
8-1-93 jon  	"adj" is now a synonym for "conj"
8-1-93 jon+martin  	from par:   integer n1:n2=3; n3=min(1,10)
8-2-93 martin  	        from adj:   integer n1:n2=3; n3=min(1,10)
8-3-93 martin  	        to history: integer n1:n2=3; n3=min(1,10)
                                                   ^         ^    only () not ( () )
 		
8-22-94 dave  	merged in changes for cray (word addressing rather than byte) 
5-7-95  stew    workaround seploc() botch with statement function def for CRAY
9-8-95  martin  added support for GNU g77
12-15-96 Biondo  added support for SGI
7-12-97 stew    changed SGI incantation to f77 -64 defaults
11-99   stew    added LINUX to the list remove # lines
*/



BEGIN {
	inmain = 1
	doc = 0
	enddoc = 0
	init=1

#if defined(CRAY)
        print "#$DIR NOBOUNDS"
#define ADDRSIZE 8	
#else
#define ADDRSIZE 1
#endif

#if defined(CONVEX) || defined(CM2) || ((defined(DEC3100) || defined(DECALPHA)) && defined(DECFC)) || defined (HP700) || defined(CRAY) || defined(CM5) || defined(gnu) || defined(SGI)  || defined(LINUX)
	implstr= "\timplicit none"
#else
#ifdef SUN
		implstr= "\timplicit none"
#else
		implstr= "\timplicit undefined (a-z)"
#endif /* SUN */
#endif /* CONVEX */

	}

/* pass empty lines unchanged */
NF ==0  {
	print $0
	next
	}

$1 ~ /define|^\%/  {
	print $0
	next
	}

/* end if main if we encounter subroutine or function declaration */
(init == 1  && /.*(subroutine|function)/) &&( ! (/^c.*(subroutine|function)/ || /.*#.*(subroutine|function)/)) {
	inmain = 0
	}

/*find end of sel-documentation*/
 (/^[#c]/ && /\%/ && enddoc==0 && inmain==1)  {
	print $0
	enddoc=1
	init=0
	print "\t\n\tinteger hetch,fetch,getch,tetch,infd,outfd,headfd,auxpar"
	print "\texternal hetch,fetch,getch,tetch,auxpar"
	print "\tinteger lcrpos"

#if defined(DEC3100)  || defined(DECALPHA) || defined(RS6000) || defined(HP700)
	print "\tinteger fsbrk,seploc,locore,input,output,head,idum"
	print "\texternal fsbrk,seploc,input,output,head"
#else
	print "\tinteger fsbrk,input,output,head,idum"
	print "\texternal fsbrk,input,output,head"
#if defined(SGI)
	print "\tinteger*8 loc, locore"
#else
	print "\tinteger loc, locore"
#endif
#endif 

#if ADDRSIZE == 8
 	print "\treal*8 basecore(1)"
#else
#if defined(DEC3100) || defined(DECALPHA) || defined(RS6000) 
	print "\tinteger*1 basecore(1)"
#else
	print "\tcharacter*1 basecore(1)"
#endif
#endif

#ifdef CM2
	print "%CMF$LAYOUT BASECORE(:SERIAL)"
#endif /* CM2 */

	print "\tcommon/falloc/basecore\n"

	next
	}

/* allow fortran comments lines to pass unchanged */
/^c|^C/ {
	print $0
	next
	}
/* allow ratfor comments lines to pass unchanged */
/[\t ]*#/ {
	print $0
	next
	}

inmain==1 && doc==0 	{
	init=0
/* allow declarations and continuation lines  to pass unchanged */
	if(( $1 $2 ~ /^integer|^real|^character|^doubleprecision|^complex|^external|^implicit|^logical|^data|^common|^intrinsic|^parameter|^options|^source/ ) || $0 ~ /^\ \ \ \ \ [^ 	0]/) 	{
		print $0
		next
	}
	else	{
		print "\tcall initpar()"

		doc=1
/*		print "\tcall doc( source )"*/
		print "\tcall doc(source)"
		print "\tcall noieeeinterupt()"
		print "\tidum=hetch('Gobbledegook','s','Lets get started.')"
		print "\tlcrpos=1 \n"
		}
				}

/*$1 ~ /^subroutine|^function/  {*/
/.*(subroutine|function)/ &&( ! (/^c.*(subroutine|function)/ || /.*#.*(subroutine|function)/)) {
	init=0
	if( inmain ==0 )
		{
		print 
#ifndef F90
		print implstr

		print "\tinteger fetch, hetch, tetch, getch, auxpar"
		print "\texternal fetch, hetch, tetch, getch, auxpar"
/*		print "\tinteger reed, rite"*/
#endif
		}


	else {
		/*
		 * generate a subroutine call in main program,   *
		 * substituting dynamically allocated arrays for *
		 * appropriate arguments			 *
		 */

		inmain = 0
		
		call = "\tcall " $2
		for(j=3; j<=NF; j++) {
			
			if("" != alloctab[$j])
				call = call "basecore(l" $j "-locore)"
			else
				call = call $j
			}
#if ADDRSIZE == 8
		print "\tlocore=fsbrk((lcrpos-1)*8)"
#else
		print "\tlocore=fsbrk(lcrpos-1)"
#endif

#if defined(DEC3100) || defined(DECALPHA) || defined(RS6000) || defined(HP700)
		print "\tlocore=seploc(basecore)-locore"
#else
		print "\tlocore=loc(basecore)-locore"
#endif
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
/*		print "\tinteger sreed, srite"*/
/*		print "\tinteger reed, rite"*/
		}
	next
	}

/* allocate : real p ( n , m )  */

$1 ~ /^allocate$/ {
	init=0
	if( inmain==0 )
#ifdef F90
		print $0
		next
#else
		print "Saw can't allocate in a subroutine"
#endif

	if( $3 ~ /^(integer|real|logical)/ )
#ifdef CRAY
        { esize=8  }
#else
        { esize=4  }
#endif
	else if( $3 ~ /^character/ )
	{ esize=1  }
	else if( $3 ~ /^(complex|doubleprecision)/ )
#ifdef CRAY
        { esize=16  }
#else
        { esize=8  }
#endif

	/* saw.lex turns 'integer*4' into 'integer * 4', etc. */

	if( $4 == "*" ) { 
		 esize=$5
#ifdef CRAY
                 if( $3 !~ /^character/ && esize == 4 ){ esize = 8 }
#endif
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
#if ADDRSIZE == 8
			alloc = alloc "/8"
			print alloc
#else
			print alloc
			print "\tlcrpos = 1 + (((lcrpos-1)+7)/8)*8"
#endif
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

$1 ~ /^from$/ && ($2 ~ /^conj$/  || $2 ~ /^adj$/) {
        if ($2 ~ /^conj$/ )
             conjadj = "conj"
        else
             conjadj = "adj"
	init=0
	j = 4
	if(     $j == "integer")	type = "i"
	else if($j == "real")		type = "r"
	else if($j == "character")	type = "s"
	else if($j == "logical")	type = "l"
	else if($j == "doubleprecision")	type = "g"
	else 				type = "SYNTAX t: Check commas."
	j++
	while(j <= NF) {
		x = $j;	j ++
                if( conjadj == "conj" )
			from = from "\t { \n if(conj==inv) {\n"
                else
                        from = from "\t { \n if(adj==inv) {\n"
		l1 = "\tif(hetch('" x "','" type "',"
		l2 = "\tif(tetch('" x "','" type "',"
		l4 = "\tif(hetch('" x "','" type "',"
		l5 = "\tif(tetch('" x "','" type "',"

                for(ie=j; ie<= NF; ie++) {      # seek end of subfield.
                        if( $ie == "(" )
                                while( $ie != ")" && ie <= NF )
                                        ie++;
                        if( $ie == "," || $ie == ";" )
                                break;
                        }

		if($j != ":") from = from "SYNTAX ERR"
		j++;	y = $j
		j++;	yd = j
		while($j != ":") j++
		j++;	z = $j
		j++;	zd = j
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
$1 ~ /^from$/ 	{			
	init=0
	if(     $2 ~ /^history$/)	place = "hetch"
	else if($2 ~ /^par$/)		place = "getch"
	else if($2 ~ /^aux$/)		place = "auxpar"
	else if($2 ~ /^either$/)	place = "fetch"
	else if($2 ~ /^tetch$/)		place = "tetch"
	else				place = "SYNTAX ERRORl: Check commas."
	if($2 ~ /^aux$/) {
		j = 5
		file = ",'" $4 "'"
		}
	else {
		file = ""
		j = 4
		}
	if(     $j ~ /^integer/)	type = "i"
	else if($j ~ /^real/)		type = "r"
	else if($j ~ /^character/)	type = "s"
	else if($j ~ /^logical/)	type = "l"
	else if($j ~ /^doubleprecision/) type = "g"
	else if($j ~ /^boolean/)	type = "1"
	else 				type = "SYNTAX ERRORt: Check commas."
	j++
	while(j <= NF) {
		from = from "\tif(" place "('" $j "','" type "',"



                for(ie=j; ie<= NF; ie++) {      # seek end of subfield.
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
$1 ~ /^to$/ 	{	
	init=0
	if($2 ~ /^aux$/) {
	j = 5
	file = ",'" $4 "'"
	if(     $j ~ /^integer/)	type = "i"
	else if($j ~ /^real/)		type = "r"
	else if($j ~ /^character/)	type = "s"
	else if($j ~ /^logical/)	type = "l"
	else if($j ~ /^doubleprecision/)type = "g"
	else 				type = "SYNTAX ERRORt: Check commas."
	j++
	while(j <= NF) {
                for(ie=j; ie<= NF; ie++) {      # seek end of subfield.
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
	print "\tcall auxputch('" extra  x "','" type "'," y  file ")"
		j = ie+1
		}
		}
	else {
	j = 4
	if(     $j ~ /^integer/)	type = "i"
	else if($j ~ /^real/)		type = "r"
	else if($j ~ /^character/)	type = "s"
	else if($j ~ /^logical/)	type = "l"
	else if($j ~ /^doubleprecision/)type = "g"
	else 				type = "SYNTAX ERRORt: Check commas."
	j++
	while(j <= NF) {
                for(ie=j; ie<= NF; ie++) {      # seek end of subfield.
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
		print "\tcall putch('" extra  x "','" type "'," y ")"
		j = ie+1
		}
	}
	next
	}
/* default print the record */
 	{
	print 
	}
