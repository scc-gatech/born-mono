module sep_param_mod
use param_func_mod
use sep_mod
implicit none
! procedure(getIntD), deferred,pass :: get_intD
type,extends (paramObj)::sepParam
  contains
      procedure::get_int=>sGetInt
      procedure ::get_Float=>sGetFloat
      procedure:: get_String=>sGetString
      procedure ::get_Bool=>sGetBool
      procedure:: get_Ints=>sGetInts
      procedure:: get_Floats=>sGetFloats
      procedure::  err=>sError
      procedure:: init=>initSEP
end type

contains

subroutine sGetInt(struct,arg,val,def)
   class(sepParam) :: struct
   character(len=*),intent(in) :: arg
   integer,intent(inout) :: val
   integer,optional :: def
   integer :: ierr 
   
   ierr=getch(arg,"d",val)
   if(ierr==0 .and. present(def)) then
     val=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   end if
 end subroutine
 
 subroutine sGetFloat(struct,arg,val,def)
   class(sepParam) :: struct
   character(len=*),intent(in) :: arg
   real,intent(inout) :: val
   real, optional :: def
   integer :: ierr
   ierr=getch(arg,"d",val)
   if(ierr==0 .and. present(def)) then
     val=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   end if
 end subroutine



subroutine sGetString(struct,arg,val,def)
   class(sepParam) :: struct
   character(len=*) ,intent(in):: arg
   character(len=*), intent(inout) ::val
   character(len=*), optional :: def
   integer :: ierr 
   ierr=getch(arg,"s",val)
   if(ierr==0 .and. present(def)) then
     val=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   end if
 end subroutine
 
 subroutine sGetBool(struct,arg,val,def)
   class(sepParam) :: struct
   character(len=*),intent(in) :: arg
   logical ,intent(inout):: val
   logical, optional :: def
   integer :: ierr
   ierr=getch(arg,"l",val)
      if(ierr==0 .and. present(def)) then
     val=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   end if
 end subroutine


 subroutine sGetInts(struct,arg,val,def)
   class(sepParam) :: struct
   character(len=*) ,intent(in):: arg
   integer, dimension(:),intent(inout):: val
   integer, optional :: def(:)
   integer :: ierr 
   integer :: tmp(10000)
   ierr=getch(arg,"d",tmp)
   if(ierr==0 .and. present(def)) then
     val(1:size(def))=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   else
     val(1:ierr)=tmp(1:ierr)
   end if

 end subroutine
 
 
 
 subroutine sGetFloats(struct,arg,val,def)
   class(sepParam) :: struct
   character(len=*),intent(in) :: arg
   real,intent(inout) :: val(:)
   real,optional ::  def(:)
   integer :: ierr
   real :: tmp(1000)
   ierr=getch(trim(arg),"f",tmp)
   if(ierr==0 .and. present(def)) then
     val(1:size(def))=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   else
     val(1:ierr)=tmp(1:ierr)
   end if
 end subroutine
 
 subroutine sError(struct,arg)
   class(sepParam) :: struct
   character(len=*) ,intent(in):: arg
   call seperr(arg)
 end subroutine
   
   
 subroutine initSEP(struct)
   class(sepParam) :: struct
   call initPar()
 end subroutine
end module  

