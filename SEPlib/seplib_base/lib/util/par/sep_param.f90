module sep_param_mod
use, intrinsic :: iso_c_binding
use sep_func_mod
implicit none



 
interface 

 
 
 
  subroutine sep_close_lock(string) bind(c,name="sep_close_lock")
    import
    character(C_CHAR), dimension(*) :: string
  end subroutine 

  subroutine sep_open_lock(string) bind(c,name="sep_open_lock")
    import
    character(C_CHAR), dimension(*) :: string
  end subroutine 

end interface
end module
