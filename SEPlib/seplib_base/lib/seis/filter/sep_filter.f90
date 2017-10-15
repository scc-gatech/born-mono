module sep_filter_mod
use, intrinsic :: iso_c_binding




interface
  subroutine mksinc(ar,n,f,ar2) bind(c,name="mksinc")
    import
    integer(C_INT),value :: n
    real(C_FLOAT),value :: f
    real(C_FLOAT),dimension(*) :: ar,ar2
  end subroutine

  subroutine toep(n,ar1,ar2,ar3,ar4) bind(c,name="toep")
    import
    integer(C_INT),value :: n
    real(C_FLOAT),dimension(*) :: ar1,ar2,ar3,ar4
  end subroutine

end interface


end module

