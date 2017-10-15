module fixbad  
! Given a PEF, find bad data and restore it.
  use mis2_mod
  use gee_quantile_mod
  use helicon
  implicit none
  contains
  subroutine fixbad1 (niter, aa, yy)
    integer,       intent (in)            :: niter
    type( filter), intent (in)      :: aa
    real,    dimension (:)            :: yy
    real,    dimension (size (yy))  :: rr
    logical, dimension (size (yy))  :: known
    integer                         :: stat
    call helicon_init( aa)
    stat = helicon_lop (.false., .false., yy, rr)
    rr = abs (rr)
    known = ( yy > 0.) .and. ( rr < 4. * gee_quantile( size(rr)/2, rr))
    call mis2 (niter, yy, aa, known, 2)
  end subroutine 
end module 
