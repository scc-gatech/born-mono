module param_func_mod
  use axis_mod

  implicit none
  type,abstract  :: genericParamObj
    character(len=128) :: myIOName
    contains
      procedure(getInt), deferred :: get_int
      procedure(getFloat),deferred :: get_float
      procedure(getString),deferred :: get_String
      procedure(getBool),deferred :: get_bool
      procedure(getInts),deferred :: get_Ints
      procedure(getFloats),deferred :: get_floats
      procedure(error), deferred:: err
      procedure :: getAxisN=>get_axis_ndef_p
      procedure :: getAxisD=>get_axis_ddef_p
     
      generic:: getInt=>get_int
      generic:: getReal=>get_float
      generic:: getLogical=>get_bool
      generic:: getString=>get_string
      generic:: getReals=>get_floats
      generic:: getInts=>get_int
      generic:: getAxis=>getAxisN,getAxisD
  end type
   type,abstract,extends (genericParamObj) :: paramObj
        contains
        procedure(initBase), deferred :: init
   end type

  abstract interface 
    subroutine initBase(struct)
      import paramObj
      class(paramObj) :: struct
    end subroutine
    subroutine getint(struct,arg,val,def)
      import genericParamObj
      class(genericParamObj) :: struct
      integer,intent(inout) :: val
      integer, optional :: def
      character(len=*) ,intent(in):: arg
    end subroutine

    subroutine getfloat(struct,arg,val,def)
      import genericParamObj
          class(genericParamObj) :: struct
      real,intent(inout)  :: val
      real,optional :: def
      character(len=*) ,intent(in):: arg
    end subroutine
    
        subroutine getString(struct,arg,val,def)
      import genericParamObj
          class(genericParamObj) :: struct
      character(len=*) ,intent(in):: arg
      character(len=*) ,intent(inout) ::  val
      character(len=*),optional :: def
    end subroutine
    
   
    subroutine getbool(struct,arg,val,def)
      import genericParamObj
          class(genericParamObj) :: struct
      logical,intent(inout) ::val
      logical, optional :: def
      character(len=*),intent(in)  :: arg
    end subroutine


    subroutine getints(struct,arg,val,def)
      import genericParamObj
          class(genericParamObj) :: struct
      integer,dimension(:),intent(inout)  :: val
      integer,optional :: def(:)
      character(len=*),intent(in) :: arg
    end subroutine   
    
    
    subroutine getfloats(struct,arg,val,def)
      import genericParamObj
          class(genericParamObj) :: struct
      real,dimension(:),intent(inout)  :: val
      real, optional :: def(:)
      character(len=*) ,intent(in):: arg
    end subroutine

    subroutine error(struct,arg)
      import genericParamObj
          class(genericParamObj) :: struct
      character(len=*) ,intent(in):: arg
    end subroutine
  end  interface     
  
  
  contains
  
      
 
     function get_axis_ndef_p(struct,iax)
      class(genericParamObj) :: struct
      integer,intent(in) :: iax
      type(axis) :: get_axis_ndef_p,axD
      axd%n=1; axd%o=0; axd%d=1; axd%label=""
      get_axis_ndef_p=struct%getAxisD(iax,axd)
    end function
    
    function get_axis_ddef_p(struct,iax,axD)
      class(genericParamObj) :: struct
      integer,intent(in) :: iax
      type(axis) :: get_axis_ddef_p
      type(axis),intent(in):: axD
      character(len=10) :: str
      
       write(str,"(a,i1)") "n",iax
       call struct%getInt(str,get_axis_ddef_p%n,axD%n)
       write(str,"(a,i1)") "o",iax
       call struct%getReal(str,get_axis_ddef_p%o,axD%o)
       write(str,"(a,i1)") "d",iax
       call struct%getReal(str,get_axis_ddef_p%d,axD%d)
       write(str,"(a,i1)") "label",iax
       call struct%getString(str,get_axis_ddef_p%label,axD%label)
    end function
   
   
end module
