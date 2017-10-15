module sep_cwp_mod
use, intrinsic :: iso_c_binding




interface 

  subroutine setvalf( a,n,b) bind(c,name="sepvalf")
    import
    integer(C_INT) :: n
    real(C_FLOAT), dimension(*) :: a
    real(C_FLOAT) :: b
  end subroutine
  
  subroutine setvali( a,n,b) bind(c,name="sepvali")
    import
    integer(C_INT) :: n
    integer(C_INT), dimension(*) :: a
    integer(C_INT) :: b
  end subroutine
  


end interface


end module

