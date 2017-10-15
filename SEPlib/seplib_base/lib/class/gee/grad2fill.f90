module grad2fill  
! min r(m) = L J m + L known  where L is a lowcut filter.
  use igrad2
  use cgstep_mod
  use mask1
  use solver_smp_mod
  implicit none
  contains
  subroutine grad2fill2( niter, m1, m2, mm, mfixed)
    integer,                    intent (in)     :: niter, m1,m2
    logical, dimension (m1*m2), intent (in)     :: mfixed      
    ! mask for known
    real,    dimension (m1*m2), intent (in out) :: mm          
    ! model
    real,    dimension (m1*m2*2)                :: yy          
    ! lowcut output
    logical, dimension (:),     pointer         :: msk
    integer :: i
    write(0,*) "dyiun 1",size(mfixed)
    allocate(msk(size(mfixed)))
!write(0,*) "dyiun21",size(msk)
!   do i=1,size(msk){
!write(0,*) "what the ",i
!     msk(i)=.not. mfixed(i)
!}
    msk(:)=.not.mfixed(:)
    write(0,*) "dyiun 2"
    call mask1_init(msk)
    call igrad2_init(m1,m2)
    yy = 0.     ! initialize
    write(0,*) "dyiun 3"
    call solver_smp(m=mm, d=yy, Fop=igrad2_lop, stepper=cgstep, niter=niter&
      &, m0=mm, Jop=mask1_lop)
    write(0,*) "dyiun 4"
    call cgstep_close ()
  end subroutine 
end module 
