module mkwallwt  
! N-dimensional wall weights
  use patch
  implicit none
  contains
  subroutine wallwtn( n, nwall, nwind, windwt, wallwt)
    integer, dimension( :), pointer      :: n, nwall, nwind
    real,    dimension( :), intent( in)  :: windwt
    real,    dimension( :), intent( out) :: wallwt
    integer                              :: ipatch, stat
    wallwt = 0.
    call patch_init( n, nwall, nwind)
    do ipatch = 1, product( n)
      stat = patch_lop( .true., .true., wallwt, windwt)
      call patch_close ()
    end do 
    where( wallwt /= 0.)
      wallwt = 1. / wallwt
end where
  end subroutine 
end module 
