module parcel  
  use classutil
  use patch
  implicit none
  integer,   private  :: nw 
  integer,   private  :: n 
  contains
  subroutine parcel_init ( nwin,nwall,nwind )
    integer, dimension (:), pointer :: nwin, nwall, nwind
    call patch_init (nwin, nwall, nwind)
    n = product (nwin)
    nw = product (nwind)
  end subroutine
  function parcel_lop ( adj, add, wall, wind) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: wall,wind 
    call adjnull (adj,add,wall,wind )
    call parcel_lop2(adj,add,wall,wind )
    stat=0
  end function 
  subroutine parcel_lop2(adj,add,wall,wind)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: wall 
    real, dimension (nw, n)  :: wind 
    integer :: i, stat
    do i = 1, n  
      stat = patch_lop (adj, .true., wall, wind (:,i))
      call patch_close ()
    end do
  end subroutine 
  subroutine parcel_close()
  end subroutine 
end module 
