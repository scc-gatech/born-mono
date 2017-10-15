module invint  
! invint -- INVerse INTerpolation in 1-D.
  use lint1
  use tcai1
  use cgstep_mod
  use solver_reg_mod
  implicit none
  contains
  subroutine invint1( niter, coord, dd, o1, d1, aa, mm, eps, mmov)
    integer,               intent (in)   :: niter               
    ! iterations
    real,                  intent (in)   :: o1, d1, eps         
    ! axis, scale
    real, dimension (:),   pointer       :: coord, aa           
    ! aa is filter
    real, dimension (:),   intent (in)   :: dd                  
    ! data
    real, dimension (:),   intent (out)  :: mm                  
    ! model
    real, dimension (:,:), intent (out)  :: mmov                
    ! movie
    integer                              :: nreg                
    ! size of A m
    nreg = size( aa) + size( mm)                                
    ! transient
    call lint1_init( o1, d1, coord )                            
    ! interpolation
    call tcai1_init( aa)                                        
    ! filtering
    call solver_reg( m=mm, d=dd, Fop=lint1_lop, stepper=cgstep, niter=niter&
      &,         Aop=tcai1_lop, nAop = nreg, eps = eps,mmov = mmov,verb=.true.)
    call cgstep_close( )
  end subroutine 
end module 
