/* 
   Modifications:

dave 8/22/94 Merged in changes for cray (word addressing not byte)
martin 10/1/95 added support for GNU g77

*/
BEGIN { user = "";  lude = ""; init = ""; inasub =0;  initemp =0; endtemp=1
	maxntemp = 50; impli=0;
/* initializations */
#if defined(CRAY)
        print "#$DIR NOBOUNDS"
#define ADDRSIZE 8
#else
#define ADDRSIZE 1
#endif

#if defined(CONVEX) || defined(CM2) || ( (defined(DEC3100) || defined(LINUX) || defined(DECALPHA)) && defined(DECFC) ) || defined(HP700) || defined(CRAY) || defined( SGI ) || defined(gnu) || defined(DECALPHA)
	implstr= "\timplicit none"
#else
#ifdef SUN
		implstr= "\timplicit undefined (a-z) "
#else
		implstr= "\timplicit undefined (a-z) "
#endif 
#endif
	implstr= "\timplicit none"

}

$1 == "subroutine" {     /* renew variables */
	initemp= 0
	ntemp=0
	inasub=1
	impli=0
	lude = ""
	user = ""
	init = ""
	decl = ""

	print 

	ludeargs =$2 "tempall"
	for( i=3 ; i<NF; i++ ){
	ludeargs = ludeargs " " $i 
	}
	userargs = ludeargs
	next
		}
/*check if implicit is already in the input file*/
$1 == "implicit"	{
	impli=1
		}
$1 == "logical" || $1 == "integer" || $1 == "real" || $1 == "complex" || $1 == "doubleprecision" || $1 == "common"  || $1 == "character" || $1 == "parameter" {
	if( !impli && inasub )	{
		print implstr
		impli =1
				}
		
	decl = decl "\n" $0 
	print
	next
	}

$1 == "temporary" {
	if(!inasub)
		print "ERROR: Cannot allocate temporary arrays here"
	if(! initemp)	{
	initemp=1
	endtemp=0
     /* initialize declaration */
#if ADDRSIZE == 8 
	init = "\treal*8 tempall(1)" "\n"
#else
	init = "\tcharacter*1 tempall(1)" "\n"
#endif
	init = init "\tcommon /satalloc/tempall" "\n"
	init = init "\tinteger fortalloc, fortfree" "\n"
	init = init "\tinteger ttempall("maxntemp")" "\n"
	print init
			}

	$1 = ""
	user= user $0 "\n\t"

	if( $2 ~ /^(integer|real|logical)/ )
#ifdef CRAY
        { esize=8  }
#else
        { esize=4  }
#endif
	else if( $2 ~ /^character/ )
	{ esize=1  }
	else if( $2 ~ /^(complex|doubleprecision)/ )
#ifdef CRAY
        { esize=16  }
#else
        { esize=8  }
#endif

	if( $3 == "*" ) { 
#ifdef CRAY
                 esize=$4
                 if( $2 !~ /^character/ && esize == 4 ){ esize = 8 }
#else
                 esize=$4
#endif
		 js=5
	        } 
	else    {
		 js=3
		}

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
					size = size ")*("
				else
					size = size $j
				}
	alloc = "\t" offset " = fortalloc(tempall,(" size "*" esize ")" "\n"
	alloc = alloc "\tif(" offset " .eq. 1)"
	alloc = alloc " call erexit('fortalloc error') \n"

#if ADDRSIZE == 8 
	alloc =  alloc"\t" offset " = " offset "/8" "\n"
#endif
	print alloc 
			j++
			}
		else if($j != "," ) {
	ntemp++
	if ( ntemp > maxntemp)	{
		print "ERROR: ntemp > maxntemp"
				}
	size = ""
	offset = "ttempall(" ntemp ")"
	ludeargs = ludeargs",tempall("offset")"
	variablename = $j
	userargs = userargs","variablename 
			}
		}
	next
	
	}
!($1 == "temporary") && (initemp == 1) && (endtemp == 0)  {
	endtemp=1
	inasub=0
	ludeargs = ludeargs ")"
	lude = lude "\tcall " ludeargs "\n"
	while( ntemp > 0 ) {
		offset = "ttempall(" ntemp ")"
		lude = lude "\tif(fortfree(tempall(" offset ")) .eq. 1)"
		lude = lude " call erexit('fortfree error') \n"
		ntemp--
		}
	lude = lude "\treturn" "\n" "\tend" "\n"
	print lude 
	ntemp = 0
	userargs = userargs ")"
	user = "\tsubroutine " userargs "\n"implstr"\n" decl "\n\t" user
	print user 
	print $0 
	next
	}
{
	print 
	next
	}
END { }
