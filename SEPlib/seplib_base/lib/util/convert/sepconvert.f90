module sep_convert_mod

use, intrinsic :: iso_c_binding

  interface  createfloat
  integer function createfloat_f(a,b,c) bind(c,name="createfloat")
    import
    integer,value :: a
    real(C_FLOAT),dimension(*), intent(inout) :: b,c
  end function
  ! module procedure createfloat_s
  end interface
  
  
  interface getbackint
  integer function getbackint_f(a,b,c) bind(c,name="getbackint")
    import
    integer,value :: a
    integer(C_INT),dimension(*), intent(inout) :: b,c
  end function
  ! module procedure getbackint_s
  end interface
  
  interface reed_char_real
  integer function reed_char_real_f(a,b,c) bind(c,name="reed_char_real")
    import
    character(C_char),dimension(*) :: a
    integer(C_INT),value :: b
    real(C_FLOAT), dimension(*) :: c
  end function
  module procedure reed_char_real_s
  end interface
  
 interface rite_char_real
 module procedure rite_char_real_s
 integer function rite_char_real_f(a,b,c) bind(c,name="rite_real_char")
    import
    character(C_char),dimension(*) :: a
    integer(C_INT),value :: b
    real(C_FLOAT), dimension(*) :: c
  end function

end interface

contains

  integer function createfloat_s(a,b,c)
    integer :: a,d
    real,dimension(:), intent(inout) :: b,c
    createfloat_s=createfloat_f(a,b,c)
  end function
  
 integer function  getbackint_s(a,b,c)
    integer :: a,d
    integer,dimension(:), intent(inout) :: b,c
    getbackint_s=getbackint_f(a,b,c)
 end function
 
 integer function reed_char_real_s(a,b,c) 
    character(len=*) :: a
    integer :: b,d
    real,dimension(:) :: c
    reed_char_real_s=reed_char_real_f(a,b,c)
 end function
 
 integer function  rite_char_real_s(a,b,c) 
    character(len=*) :: a
    integer :: b,d
    real,dimension(:) :: c
    rite_char_real_s=rite_char_real_f(a,b,c)
 end function

end module

