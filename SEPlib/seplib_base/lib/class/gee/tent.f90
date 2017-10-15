module tent  
! triangle tent weights in N dimensions
  use cartesian
  implicit none
  contains
  subroutine tentn ( nwind, center, a, windwt)
    integer, dimension (:), intent (in)  :: nwind, center, a
    real,    dimension (:), intent (out) :: windwt
    integer, dimension( size( nwind))    :: start, end, x
    real,    dimension( size( nwind))    :: mid, wid
    integer                               :: i
    start= 1+a-center
    end= nwind-center+1         ! a is beginning of tent
    mid=  (end+start)/2.
    wid= (end-start+1.)/2.
    do i = 1, size( windwt)
      call line2cart(  nwind, i, x)
      if ( all( x >= start) .and. all( x <= end)) then
        windwt( i) = product( max( 0., 1. - abs((x-mid)/wid)))
      else
        windwt( i) = 0.
      end if
    end do
  end subroutine 
end module 
