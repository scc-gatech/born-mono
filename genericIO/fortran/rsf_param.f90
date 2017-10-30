module rsf_param_mod
use param_func_mod
use rsf
implicit none
! procedure(getIntD), deferred,pass :: get_intD
type,extends (paramObj)::rsfParam
  contains
      procedure::get_int=>sGetInt
      procedure:: get_Float=>sGetFloat
      procedure:: get_String=>sGetString
      procedure:: get_Bool=>sGetBool
      procedure:: get_Ints=>sGetInts
      procedure:: get_Floats=>sGetFloats
      procedure::  err=>sError
      procedure:: init=>initSEP
end type

contains

subroutine sGetInt(struct,arg,val,def)
   class(rsfParam) :: struct
   character(len=*),intent(in) :: arg
   integer,intent(inout):: val
   integer, optional :: def
   call from_par(arg,val,def)
 end subroutine
 

subroutine sGetFloat(struct,arg,val,def)
   class(rsfParam) :: struct
   character(len=*),intent(in) :: arg
   real,intent(inout):: val
   real, optional :: def
 call from_par(arg,val,def)
 end subroutine
 
 
 subroutine sGetString(struct,arg,val,def)
   class(rsfParam) :: struct
   character(len=*),intent(in) :: arg
   character(len=*),intent(inout) ::val
   character(len=*), optional :: def
   integer :: ierr
 call from_par(arg,val,def)
 end subroutine


subroutine sGetBool(struct,arg,val,def)
   class(rsfParam) :: struct
   character(len=*),intent(in) :: arg
   logical,intent(inout)  :: val
   logical, optional :: def
 call from_par(arg,val,def)
 end subroutine
 
 
 subroutine sGetInts(struct,arg,val,def)
   class(rsfParam) :: struct
   character(len=*),intent(in) :: arg
   integer,intent(inout)  ::  val(:)
   integer, optional :: def(:)
 call from_par(arg,val,def)
 end subroutine

 subroutine sGetFloats(struct,arg,val,def)
   class(rsfParam) :: struct
   character(len=*) ,intent(in) :: arg
   real,intent(inout)  :: val(:)
   real,optional :: def(:)
   integer :: ierr 
   real :: tmp(10000)
 call from_par(arg,val,def)
 end subroutine
 

 
 subroutine sError(struct,arg)
   class(rsfParam) :: struct
   character(len=*),intent(in)  :: arg
   call sf_error(arg)
 end subroutine
   
   
 subroutine initSEP(struct)
   class(rsfParam) :: struct
   call sf_init()
 end subroutine
end module  

