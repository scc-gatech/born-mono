module mis_mod  
  use tcai1
  use mask1
  use cgstep_mod
  use solver_smp_mod
!  use mtcai1
implicit none
  contains
! fill in missing data on 1-axis by minimizing power out of a given filter.
  subroutine mis1 ( niter, mm, aa)
    integer,             intent (in)      :: niter   
    ! number of iterations
    real, dimension (:), pointer          :: aa      
    ! roughening filter
    real, dimension (:), intent (in out)  :: mm      
    ! in - data with zeroes
! out - interpolated
    real, dimension (:),allocatable       :: zero    ! filter output
    logical, dimension(:), pointer        :: msk
    integer                               :: stat
!    real, dimension (:),allocatable       :: dd
    allocate(zero(size(mm)+size(aa)))
    zero = 0.
    allocate( msk(size(mm)))
!    allocate(  dd(size(mm)+size(aa)))
! solve    F    m = 0  w/ J
    msk=(mm.eq.0.)
    call mask1_init(msk)
    call tcai1_init(aa)
    call solver_smp(m=mm,d=zero,Fop=tcai1_lop,stepper=cgstep,niter=niter&
      &,m0=mm,Jop=mask1_lop)
! solve  (F J) m = d
!    call mtcai1_init(aa,msk)                 #       F(I-J)
!    stat = mtcai1_lop(.false.,.false.,mm,dd) #       F(I-J) m
!    dd = - dd                                # d = - F(I-J) m
!    msk=(mm==0.); call mask1_init(msk)       #           J
!    call solver_smp(m=mm,d=dd,Fop=mtcai1_lop,stepper=cgstep,niter=niter,m0=mm)
    call cgstep_close ()
    deallocate(zero)
  end subroutine 
end module 
