module gdecon_mod  
! gapped deconvolution.  input=yy  output=rr
  use icaf1
  use cgstep_mod
  use solver_mod
  implicit none
  contains
  subroutine gdecon( na, yy, rr, niter, gap)
    integer,                intent( in)  :: na, niter, gap
    real,    dimension( :), pointer      :: yy
    real,    dimension( :), intent( out) :: rr
    real,    dimension( na)              :: aa 
    ! prediction-error filter
    logical, dimension( na)              :: mm ! filter mask
    real,    dimension(:),allocatable    :: dd ! data
    allocate(dd(size(yy)))
    dd = 0.                                    ! initialize data
    aa = 0.
    aa( 1) = 1.                       ! initialize filter
    mm = .false.
    mm( :gap) = .true.           ! initialize mask
    call icaf1_init( yy, 1)                    
    ! initialize convolution
    call solver( icaf1_lop, cgstep, aa, dd, niter,x0= aa, res= rr, known=&
      & mm)
    call cgstep_close()
    deallocate(dd)
  end subroutine 
end module 
