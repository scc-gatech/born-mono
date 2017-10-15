module print  
  use box
  use helix
  use cartesian
  implicit none
  contains
  subroutine printn( nd, center, na, aa)
    integer, dimension (:), intent( in) :: nd, center, na    ! (ndim)
    type( filter),          intent( in) :: aa
    real,    dimension( :), allocatable :: filt 
    integer, dimension (size (nd))      :: ii, ii0 
    integer                             :: ia, i
    allocate( filt (product (na)))
    call boxn( nd, center, na, aa, filt) 
    write (0,*) "-----------------------------------------"
    ii0 = 1
    do ia = 1, size(filt) - na(1)+1, na(1)
      call line2cart(  na, ia, ii)
      do i=1, count( ii /= ii0) - 1
        write (0,*) "+++++++++++++++++++++++++++++++++++++++++"
      end do 
      write (0, "(10f7.3)") filt( ia:ia+na(1)-1)
      ii0 = ii
    end do 
    write (0,*) "-----------------------------------------"
    deallocate ( filt)
  end subroutine 
end module 
