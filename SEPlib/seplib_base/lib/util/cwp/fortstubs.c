#include <sepmath.h>
#include <sulib.h>
#include <cfortran.h>

FCALLSCFUN2(INT,npfao,NPFAO,npfao,INT,INT)
FCALLSCSUB6(pfamcc,PFAMCC,pfamcc,INT,INT,INT,INT,INT,PFLOAT)
FCALLSCSUB3(pfacc,PFACC,pfacc,INT,INT,PFLOAT)
FCALLSCSUB4(pfarc,PFARC,pfarc,INT,INT,PFLOAT,PFLOAT)
FCALLSCSUB4(pfacr,PFACR,pfacr,INT,INT,PFLOAT,PFLOAT)
