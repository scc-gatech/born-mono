module sep_cwp_mod
use, intrinsic :: iso_c_binding






interface 
  integer function npfao(a,b) bind(c,name="npfao")
    import
    integer(C_INT),value :: a,b
  end function

  subroutine pfamcc( a,b,c,d,e,ar) bind(c,name="pfamcc")
    import
    integer(C_INT),value :: a,b,c,d,e
    complex(C_FLOAT), dimension(*) :: ar
  end subroutine
  
  subroutine pfacc( a,b,ar) bind(c,name="pfacc")
    import
    integer(C_INT),value :: a,b
    complex(C_FLOAT), dimension(*) :: ar
  end subroutine
  
  subroutine pfarc( a,b,ar1,ar2) bind(c,name="pfarc")
    import
    integer(C_INT),value :: a,b
    real(C_FLOAT), dimension(*) :: ar1
    complex(C_FLOAT), dimension(*) :: ar2
  end subroutine
  
    subroutine pfacr( a,b,ar1,ar2) bind(c,name="pfacr")
    import
    integer(C_INT),value :: a,b
    real(C_FLOAT), dimension(*) :: ar2
    complex(C_FLOAT), dimension(*) :: ar1
  end subroutine
  

end interface


end module

