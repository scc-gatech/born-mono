module c_param_mod
use param_func_mod
use generic_c_stubs
implicit none
! procedure(getIntD), deferred,pass :: get_intD
type,extends (paramObj)::cParam
  contains
        procedure:: init=>initC

      procedure::get_int=>sGetInt
      procedure ::get_Float=>sGetFloat
      procedure:: get_String=>sGetString
      procedure ::get_Bool=>sGetBool
      procedure:: get_Ints=>sGetInts
      procedure:: get_Floats=>sGetFloats
      procedure::  err=>sError
end type

contains


subroutine initC(struct)
  class(CParam) :: struct
end subroutine
subroutine sGetInt(struct,arg,val,def)
   class(CParam) :: struct
   character(len=*) ,intent(in):: arg
   integer,intent(inout):: val
   integer,optional :: def
   integer :: x
   x=0
   if(present(def)) then
     x=1;
   end if

   call getParamIntC(trim(struct%myIOName)//C_NULL_CHAR,trim(arg)//C_NULL_CHAR,val,x);
   
 end subroutine
 
 subroutine sGetFloat(struct,arg,val,def)
   class(cParam) :: struct
   character(len=*),intent(in) :: arg
   real,intent(inout) :: val
   real, optional :: def
   integer :: x
   x=0
   if(present(def)) then
     x=1;
   end if
   call getParamFloatC(trim(struct%myIOName)//C_NULL_CHAR,trim(arg)//C_NULL_CHAR,val,x);
 end subroutine



subroutine sGetString(struct,arg,val,def)
   class(cParam) :: struct
   character(len=*),intent(in) :: arg
   character(len=*), intent(inout) :: val
   character(len=*), optional :: def
   integer :: x
   x=0
   if(present(def)) then
      x=1;
   end if
   call getParamStringC(trim(struct%myIOName)//C_NULL_CHAR,trim(arg)//C_NULL_CHAR,val,x);
   call c2forstr2(val)
 end subroutine
 
 subroutine sGetBool(struct,arg,val,def)
   class(cParam) :: struct
   character(len=*),intent(in) :: arg
   logical ,intent(inout):: val
   logical,optional :: def
   integer :: x,xuse
   x=0
   if(present(def)) then
     x=1
     xuse=0;
     if(def) xuse=1;
   end if

   call getParamBoolC(trim(struct%myIOName)//C_NULL_CHAR,trim(arg)//C_NULL_CHAR,xuse,x);
   val=.false.
   if(xuse==1) val=.true.
 end subroutine


 subroutine sGetInts(struct,arg,val,def)
   class(cParam) :: struct
   character(len=*) ,intent(in):: arg
   integer, dimension(:),intent(inout):: val
   integer, optional :: def(:)
      integer :: x
   x=0
   if(present(def)) x=1
   call getParamIntsC(trim(struct%myIOName)//C_NULL_CHAR,trim(arg)//C_NULL_CHAR,val,x);

 end subroutine
 
 
 
 subroutine sGetFloats(struct,arg,val,def)
   class(cParam) :: struct
   character(len=*),intent(in) :: arg
   real ,intent(inout):: val(:)
   real,optional ::  def(:)
    integer :: x
   x=0
   if(present(def)) x=1
   call getParamFloatsC(trim(struct%myIOName)//C_NULL_CHAR,trim(arg)//C_NULL_CHAR,val,x);
 end subroutine
 
 subroutine sError(struct,arg)
   class(cParam) :: struct
   character(len=*),intent(in) :: arg
   call cerror(trim(struct%myIOName)//C_NULL_CHAR,trim(arg)//C_NULL_CHAR)
 end subroutine
   
   

end module  

