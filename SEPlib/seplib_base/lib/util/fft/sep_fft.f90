module sep_fft_mod
use, intrinsic :: iso_c_binding

interface 
  subroutine cefft(x,lx,isign,scale) bind(c,name="cefft")
    import
    integer(C_INT),value ::  lx,isign
    complex(C_FLOAT),dimension(*) :: x
    real(C_FLOAT),value :: scale
  end subroutine

  subroutine cvfft(x,lx,nx,isign,scale) bind(c,name="cvfft")
    import
    integer(C_INT),value ::  lx,isign,nx
    complex(C_FLOAT),dimension(*) :: x
    real(C_FLOAT) ,value:: scale
  end subroutine

    subroutine refft(x,lx,nx,isign,scale) bind(c,name="refft")
    import
    integer(C_INT),value ::  lx,isign,nx
    real(C_FLOAT),dimension(*) :: x
    real(C_FLOAT),value :: scale
  end subroutine
  
  subroutine rvfft(x,lx,nx,isign,scale) bind(c,name="rvfft")
    import
    integer(C_INT),value ::  lx,isign,nx
    real(C_FLOAT),dimension(*) :: x
    real(C_FLOAT),value :: scale
  end subroutine



end interface


end module

