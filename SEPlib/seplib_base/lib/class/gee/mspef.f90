module mspef  
! Find multi-scale prediction-error filter (helix magic)
  use mshconest
  use cgstep_mod
  use solver_smp_mod
  implicit none
  contains
  subroutine find_pef( yy, aa, niter)
    integer,             intent( in)              :: niter
    real, dimension( :), pointer                  :: yy
    type( msfilter)                               :: aa
    integer                                       :: is
    real, allocatable              :: dd(:,:),ee(:)
    allocate(dd( size( yy), size( aa%lag, 2)))
    allocate(ee(size(dd)))
    do is = 1, size( dd, 2)
      dd( :,is) = -yy
    end do 
    ee=reshape( dd,(/size(dd)/))
    call mshconest_init( yy, aa)
    call solver_smp(m=aa%flt, d=ee, Fop=mshconest_lop, stepper=cgstep,&
      & niter=niter, m0=aa%flt)
    call cgstep_close()
    deallocate(dd,ee)
  end subroutine 
end module 
