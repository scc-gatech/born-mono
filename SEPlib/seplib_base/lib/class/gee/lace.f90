module lace  
! find PEF on interlaced data
  use createhelixmod 
  use bound
  use pef
  use cartesian
  implicit none
  contains
  function lace_pef( dd, jump, nd, center, gap, na)  result( aa)
    type( filter)                      :: aa
    integer,               intent( in) :: jump
    integer, dimension(:), intent( in) :: nd, center, gap, na
    real,    dimension(:), pointer     :: dd          ! input data
    integer, dimension(:), pointer     :: savelags    ! holding place
    integer, dimension( size( nd))     :: ii
    integer                            :: ih, nh, lag0, lag
    aa = createhelix( nd, center, gap, na)
    nh = size( aa%lag)
    savelags => aa%lag
    allocate( aa%lag( nh))      ! prepare interlaced helix
    call cart2line(  na, center, lag0)
    do ih = 1, nh  
! Sweep thru the filter.
      call line2cart( na, ih+lag0, ii)
      ii = ii - center
      ii(1) = ii(1)*jump  ! Interlace on 1-axis.
      call cart2line( nd,  ii+1, lag)
      aa%lag( ih) = lag - 1
    end do 
    call boundn( nd, nd, (/ na(1)*jump, na(2:) /), aa)  
    ! Define  aa.mis
    call find_pef( dd, aa, nh*2)                        
    ! Estimate aa coefs
    deallocate( aa%lag)
    aa%lag => savelags                ! Restore filter lags
  end function 
end module 
