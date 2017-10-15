module lolace  
! find Local PEF on interlaced data
  use createhelixmod 
  use bound
  use pef
  use cartesian
  use patch        
  implicit none
  contains
  subroutine lolace_pef( wall, npatch, nwall, nwind, jump, center, gap&
    &, na, aa)
    type( filter), dimension (:)       :: aa
    integer,               intent( in) :: jump
    integer, dimension(:), pointer     :: npatch, nwall, nwind
    integer, dimension(:), intent (in) :: center, gap, na
    real,    dimension(:), intent( in) :: wall        ! input data
    real,    dimension(:), pointer     :: windata
    integer, dimension(:), pointer     :: savelags    ! holding place
    integer, dimension( size( na))     :: ii
    integer                            :: ih, nh, lag0, lag, ip, stat,&
      & np
    aa (1) = createhelix( nwind, center, gap, na)
    np = product (npatch)
    nh = size( aa(1)%lag)
    savelags => aa(1)%lag
    allocate( aa(1)%lag( nh))                           
    ! interlaced helix
    call cart2line(  na, center, lag0)
    do ih = 1, nh  
! Sweep thru filter.
      call line2cart( na, ih+lag0, ii)
      ii = ii - center
      ii(1) = ii(1)*jump  ! Interlace on 1-axis.
      call cart2line( nwind,  ii+1, lag)
      aa(1)%lag( ih) = lag - 1
    end do 
    call boundn( nwind, nwind, (/ na(1)*jump, na(2:) /), aa (1))  
    ! Define  mis
    allocate( windata( product( nwind)))   ! a patch
    call patch_init( npatch, nwall, nwind)
    do ip = 1, np  
! do all patches
      if (ip > 1) then
        allocate (aa(ip)%flt (nh))
        aa(ip)%flt = 0.
        aa(ip)%lag => aa(1)%lag
        aa(ip)%mis => aa(1)%mis
      end if
      stat = patch_lop( .false., .false., wall, windata)      
      ! get a patch
      call find_pef( windata, aa(ip), niter=nh)               
      ! find PEF
      call patch_close()
    end do 
    deallocate (aa(1)%lag)
    do ip = 1, np  
      aa(ip)%lag => savelags
    end do 
    deallocate( windata)
  end subroutine 
end module 
