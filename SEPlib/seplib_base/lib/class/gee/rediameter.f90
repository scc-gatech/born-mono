module rediameter  
! convert a helix filter from one size data to another
  use helix
  use box
  use unbox
  implicit none
  contains
  subroutine rediametern( nold, nnew, aa, na, center)
    integer, dimension (:), intent (in) :: nold, nnew       
    ! old and new data
    integer, dimension (:), intent (in) :: na, center
    type( filter)                       :: aa
    real, dimension(:), allocatable     :: cube
    allocate ( cube( product( na)))
    call   boxn( nold, center, na, aa, cube)
    aa = unboxn( nnew, center, na,     cube)
    deallocate ( cube)
  end subroutine 
end module 
