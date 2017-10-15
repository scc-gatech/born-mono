module vrms2int_mod  
! Transform from RMS to interval velocity
  use causint_mod
  use weight 
  use mask1
  use cgstep_mod
  use solver_prc_mod
  implicit none
  contains
  subroutine vrms2int( niter, eps, weight, vrms, vint)
    integer,            intent( in)     :: niter     ! iterations
    real,               intent( in)     :: eps       
    ! scaling parameter
    real, dimension(:), intent( in out) :: vrms      
    ! RMS velocity
    real, dimension(:), intent( out)    :: vint      
    ! interval velocity
    real, dimension(:), pointer         :: weight    ! data weighting
    integer                             :: st,it,nt  
    logical, dimension( size( vint))    :: mask
    logical, dimension(:), pointer      :: msk
    real,    dimension( size( vrms))    :: dat ,wt
    real, dimension(:), pointer         :: wght
    nt = size( vrms)
    do it= 1, nt  
      dat( it) = vrms( it) * vrms( it) * it
      wt( it) = weight( it)*(1./it)           
      ! decrease weight with time
    end do 
    mask = .false.
    mask( 1) = .true.            ! constrain first point
    vint = 0.     
    vint( 1) = dat( 1)
    allocate(wght(size(wt)))
    wght=wt
    call weight_init(wght)
    allocate(msk(size(mask)))
    msk=.not.mask
    call mask1_init(msk)
    call solver_prc( m=vint, d=dat,Fop=causint_lop, stepper=cgstep,&
      & niter=niter,         Sop= causint_lop, nSop=nt, eps = eps&
      &,verb=.true.,Jop=mask1_lop,         p0=vint, Wop=weight_lop)
    call cgstep_close()
    st = causint_lop( .false., .false., vint, dat)
    do it= 1, nt
      vrms( it) = sqrt( dat( it)/it)
    end do 
    vint = sqrt( vint)
  end subroutine 
end module 
