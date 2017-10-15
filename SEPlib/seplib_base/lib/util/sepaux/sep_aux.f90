module sep_aux_mod
use, intrinsic :: iso_c_binding
use sep






interface 
  subroutine parcatf(str,iv,str2) bind(c,name="parcat")
    import
    integer(C_INT),value :: iv
    character(C_CHAR), dimension(*) :: str,str2
  end subroutine
 integer function do_sep_loopf(str,p1,p2) bind(c,name="do_sep_loop")
  import
  character(C_CHAR),dimension(*) :: str
  integer(C_INT),dimension(*) :: p1,p2
 end function
 subroutine time_windowf(i,p1,p2,str) bind(c,name="time_window")
   import
   integer(C_INT),value :: i
   real(C_FLOAT) ,dimension(*) :: p1,p2
   character(C_CHAR),dimension(*) :: str
  end subroutine
 integer function init_loop_calcf(i1,p1,str,i2) bind(c,name="init_loop_calc")
  import
  character(C_CHAR),dimension(*) :: str
  integer(C_INT),dimension(*) :: p1
  integer(C_INT),value :: i1,i2
 end function
end interface

contains

  subroutine parcat(strin,in,strout)
   character(len=*) :: strin,strout
   integer :: in
    strout=strin//"          "
    call parcatf(trim(strin)//C_NULL_CHAR,in,strout)
    call c2forstr(strout)
  end subroutine
  integer function do_sep_loop(str,p1,p2)
    character(len=*) :: str
    integer,dimension(:)  :: p1,p2
     do_sep_loop=do_sep_loopf(trim(str)//C_NULL_CHAR,p1,p2)
  end function
  subroutine time_window(i,p1,p2,str)
    integer :: i
     real,dimension(:) ::  p1,p2
      character(len=*) :: str
      call time_windowf(i,p1,p2,trim(str)//C_NULL_CHAR)
   end subroutine
   integer function init_loop_calc(i1,p1,str,i2)
     character(len=*) :: str
     integer ::  i1,i2,p1(:)
     init_loop_calc=init_loop_calcf(i1,p1,trim(str)//C_NULL_CHAR,i2)
   end function



end module

