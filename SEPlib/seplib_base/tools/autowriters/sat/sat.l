%Start KEY
%option main
%%
^[ \t]*"to history"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"to aux"				{ ECHO ; BEGIN KEY ; }
^[ \t]*"from history"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"from par" 			{ ECHO ; BEGIN KEY ; }
^[ \t]*"from either"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"from tetch"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"from aux"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"from conj"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"allocate"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"subroutine"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"character"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"real"				{ ECHO ; BEGIN KEY ; }
^[ \t]*"complex"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"integer"			{ ECHO ; BEGIN KEY ; }
^[ \t]*"temporary"			{ ECHO ; BEGIN KEY ; }
<KEY>\n				{ ECHO ; BEGIN 0   ; }
^"%".*\n			{ /* pass % control lines untouched */ ECHO ; }
"double"[ \t]*"precision"		{printf("doubleprecision");}
"DOUBLE"[ \t]*"PRECISION"		{printf("doubleprecision");}
<KEY>\,[ \t]*\n 		{printf(" , ");}
<KEY>\_[ \t]*\n 		{; /* continuation lines (Stew change)*/}
<KEY>\,				{printf(" , ");}
<KEY>\'				{printf(" ' ");}
<KEY>\(				{printf(" ( ");}
<KEY>\)				{printf(" ) ");}
<KEY>\+				{printf(" + ");}
<KEY>\-				{printf(" - ");}
<KEY>\*				{printf(" * ");}
<KEY>\/				{printf(" / ");}
<KEY>\=				{printf(" = ");}
<KEY>\:				{printf(" : ");}
<KEY>\\\n			{;}
.				{ ECHO ; }
\n				{ ECHO ; }
%%
